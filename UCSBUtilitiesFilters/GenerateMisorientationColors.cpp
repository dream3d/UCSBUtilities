/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                               *
 * Copyright (c) 2015 The Regents of the University of California, Author: William Lenthe        *
 *                                                                                               *
 * This program is free software: you can redistribute it and/or modify it under the terms of    *
 * the GNU Lesser General Public License as published by the Free Software Foundation, either    *
 * version 2 of the License, or (at your option) any later version.                              *
 *                                                                                               *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     *
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     *
 * See the GNU Lesser General Public License for more details.                                   *
 *                                                                                               *
 * You should have received a copy of the GNU Lesser General Public License                      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.                         *
 *                                                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "GenerateMisorientationColors.h"

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedPathCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/Utilities/ColorTable.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/Math/MatrixMath.h"

#include "UCSBUtilitiesFilters/LaueOps/CubicOpsMisoColor.h"
#include "UCSBUtilitiesFilters/LaueOps/CubicLowOpsMisoColor.h"
#include "UCSBUtilitiesFilters/LaueOps/HexagonalOpsMisoColor.h"
#include "UCSBUtilitiesFilters/LaueOps/HexagonalLowOpsMisoColor.h"
#include "UCSBUtilitiesFilters/LaueOps/OrthoRhombicOpsMisoColor.h"
#include "UCSBUtilitiesFilters/LaueOps/TrigonalOpsMisoColor.h"
#include "UCSBUtilitiesFilters/LaueOps/TetragonalOpsMisoColor.h"
#include "UCSBUtilitiesFilters/LaueOps/TrigonalLowOpsMisoColor.h"
#include "UCSBUtilitiesFilters/LaueOps/TetragonalLowOpsMisoColor.h"
#include "UCSBUtilitiesFilters/LaueOps/TriclinicOpsMisoColor.h"
#include "UCSBUtilitiesFilters/LaueOps/MonoclinicOpsMisoColor.h"

#include "UCSBUtilities/UCSBUtilitiesVersion.h"
#include "UCSBUtilities/UCSBUtilitiesConstants.h"

#include "EbsdLib/Core/EbsdLibConstants.h"
#include "EbsdLib/Core/Quaternion.hpp"
#include "EbsdLib/LaueOps/LaueOps.h"

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/partitioner.h>
#endif

/* Create Enumerations to allow the created Attribute Arrays to take part in renaming */
enum createdPathID : RenameDataPath::DataID_t
{
  DataArrayID30 = 30,
  DataArrayID31 = 31,
};

/**
 * @brief The GenerateMisorientationColorsImpl class implements a threaded algorithm that computes the misorientation
 * colors for each element in a geometry.
 */
class GenerateMisorientationColorsImpl
{
public:
  GenerateMisorientationColorsImpl(FloatVec3Type referenceAxis, float refAngle, float* quats, int32_t* phases, uint32_t* crystalStructures, bool* goodVoxels, uint8_t* notSupported, uint8_t* colors)
  : m_ReferenceAxis(referenceAxis)
  , m_ReferenceAngle(refAngle)
  , m_Quats(quats)
  , m_CellPhases(phases)
  , m_CrystalStructures(crystalStructures)
  , m_GoodVoxels(goodVoxels)
  , m_NotSupported(notSupported)
  , m_MisorientationColor(colors)
  {
  }
  virtual ~GenerateMisorientationColorsImpl() = default;

  void convert(size_t start, size_t end) const
  {
    QVector<LaueOps::Pointer> ops;
    ops.push_back(HexagonalOpsMisoColor::New());

    ops.push_back(CubicOpsMisoColor::New());

    ops.push_back(HexagonalLowOpsMisoColor::New()); // Hex Low
    ops.push_back(CubicLowOpsMisoColor::New());     // Cubic Low
    ops.push_back(TriclinicOpsMisoColor::New());    // Triclinic
    ops.push_back(MonoclinicOpsMisoColor::New());   // Monoclinic

    ops.push_back(OrthoRhombicOpsMisoColor::New()); // OrthoRhombic

    ops.push_back(TetragonalLowOpsMisoColor::New()); // Tetragonal-low
    ops.push_back(TetragonalOpsMisoColor::New());    // Tetragonal-high

    ops.push_back(TrigonalLowOpsMisoColor::New()); // Trigonal-low
    ops.push_back(TrigonalOpsMisoColor::New());    // Trigonal-High

    ops.push_back(OrthoRhombicOpsMisoColor::New()); // Axis OrthorhombicOps

    Quaternion<double> refQuat = {m_ReferenceAxis[0] * sinf(m_ReferenceAngle), m_ReferenceAxis[1] * sinf(m_ReferenceAngle), m_ReferenceAxis[2] * sinf(m_ReferenceAngle), cosf(m_ReferenceAngle)};
    Quaternion<double> cellQuat = {0.0f, 0.0f, 0.0f, 1.0f};
    SIMPL::Rgb argb = 0x00000000;

    bool missingGoodVoxels = false;
    if(nullptr == m_GoodVoxels)
    {
      missingGoodVoxels = true;
    }
    int32_t phase = 0;
    size_t index = 0;
    for(size_t i = start; i < end; i++)
    {
      phase = m_CellPhases[i];
      index = i * 3;
      m_MisorientationColor[index] = 0;
      m_MisorientationColor[index + 1] = 0;
      m_MisorientationColor[index + 2] = 0;
      cellQuat = Quaternion<double>(m_Quats[i * 4], m_Quats[i * 4 + 1], m_Quats[i * 4 + 2], m_Quats[i * 3]);

      if(m_CrystalStructures[phase] != EbsdLib::CrystalStructure::Cubic_High && m_CrystalStructures[phase] != EbsdLib::CrystalStructure::Hexagonal_High)
      {
        uint32_t idx = m_CrystalStructures[phase];
        if(idx == EbsdLib::CrystalStructure::UnknownCrystalStructure)
        {
          idx = 12;
        }
        m_NotSupported[idx] = 1;
        m_MisorientationColor[index] = 0;
        m_MisorientationColor[index + 1] = 0;
        m_MisorientationColor[index + 2] = 0;
      }
      // Make sure we are using a valid Euler Angles with valid crystal symmetry
      else if((missingGoodVoxels || m_GoodVoxels[i]) && m_CrystalStructures[phase] < EbsdLib::CrystalStructure::LaueGroupEnd)
      {
        argb = ops[m_CrystalStructures[phase]]->generateMisorientationColor(cellQuat, refQuat);
        m_MisorientationColor[index] = RgbColor::dRed(argb);
        m_MisorientationColor[index + 1] = RgbColor::dGreen(argb);
        m_MisorientationColor[index + 2] = RgbColor::dBlue(argb);
      }
    }
  }

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
  void operator()(const tbb::blocked_range<size_t>& r) const
  {
    convert(r.begin(), r.end());
  }
#endif
private:
  FloatVec3Type m_ReferenceAxis;
  float m_ReferenceAngle;
  float* m_Quats;
  int32_t* m_CellPhases;
  uint32_t* m_CrystalStructures;
  bool* m_GoodVoxels;
  uint8_t* m_NotSupported;
  uint8_t* m_MisorientationColor;
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
GenerateMisorientationColors::GenerateMisorientationColors()
: m_ReferenceAngle(0.0f)
, m_CellPhasesArrayPath("", "", "")
, m_QuatsArrayPath("", "", "")
, m_CrystalStructuresArrayPath("", "", "")
, m_GoodVoxelsArrayPath("", "", "")
, m_MisorientationColorArrayName(SIMPL::CellData::MisorientationColor)
, m_UseGoodVoxels(false)
{
  m_ReferenceAxis[0] = 0.0f;
  m_ReferenceAxis[1] = 0.0f;
  m_ReferenceAxis[2] = 1.0f;
  m_ReferenceAngle = 0.0f;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
GenerateMisorientationColors::~GenerateMisorientationColors() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMisorientationColors::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Reference Orientation Axis", ReferenceAxis, FilterParameter::Parameter, GenerateMisorientationColors));

  parameters.push_back(SIMPL_NEW_FLOAT_FP("Reference Orientation Angle (Degrees)", ReferenceAngle, FilterParameter::Parameter, GenerateMisorientationColors));
  QStringList linkedProps("GoodVoxelsArrayPath");
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Apply to Good Elements Only (Bad Elements Will Be Black)", UseGoodVoxels, FilterParameter::Parameter, GenerateMisorientationColors, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Element Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(SIMPL::TypeNames::Float, 4, AttributeMatrix::Category::Element);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Quaternions", QuatsArrayPath, FilterParameter::RequiredArray, GenerateMisorientationColors, req));
  }
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(SIMPL::TypeNames::Int32, 1, AttributeMatrix::Category::Element);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Phases", CellPhasesArrayPath, FilterParameter::RequiredArray, GenerateMisorientationColors, req));
  }
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(SIMPL::TypeNames::Bool, 1, AttributeMatrix::Category::Element);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Mask", GoodVoxelsArrayPath, FilterParameter::RequiredArray, GenerateMisorientationColors, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Ensemble Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(SIMPL::TypeNames::UInt32, 1, AttributeMatrix::Category::Ensemble);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Crystal Structures", CrystalStructuresArrayPath, FilterParameter::RequiredArray, GenerateMisorientationColors, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Element Data", FilterParameter::CreatedArray));
  parameters.push_back(
      SIMPL_NEW_DA_WITH_LINKED_AM_FP("Misorientation Colors", MisorientationColorArrayName, CellPhasesArrayPath, CellPhasesArrayPath, FilterParameter::CreatedArray, GenerateMisorientationColors));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMisorientationColors::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setUseGoodVoxels(reader->readValue("UseGoodVoxels", getUseGoodVoxels()));
  setMisorientationColorArrayName(reader->readString("MisorientationColorArrayName", getMisorientationColorArrayName()));
  setGoodVoxelsArrayPath(reader->readDataArrayPath("GoodVoxelsArrayPath", getGoodVoxelsArrayPath()));
  setCrystalStructuresArrayPath(reader->readDataArrayPath("CrystalStructuresArrayPath", getCrystalStructuresArrayPath()));
  setQuatsArrayPath(reader->readDataArrayPath("QuatsArrayPath", getQuatsArrayPath()));
  setCellPhasesArrayPath(reader->readDataArrayPath("CellPhasesArrayPath", getCellPhasesArrayPath()));
  setReferenceAxis(reader->readFloatVec3("ReferenceAxis", getReferenceAxis()));
  setReferenceAngle(reader->readValue("ReferenceAngle", getReferenceAngle()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMisorientationColors::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMisorientationColors::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  DataArrayPath tempPath;

  QVector<DataArrayPath> dataArrayPaths;

  std::vector<size_t> cDims(1, 1);
  m_CellPhasesPtr =
      getDataContainerArray()->getPrereqArrayFromPath<DataArray<int32_t>>(this, getCellPhasesArrayPath(), cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if(nullptr != m_CellPhasesPtr.lock())                                                                           /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_CellPhases = m_CellPhasesPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */
  if(getErrorCode() >= 0)
  {
    dataArrayPaths.push_back(getCellPhasesArrayPath());
  }

  cDims[0] = 4;
  m_QuatsPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<float>>(this, getQuatsArrayPath(), cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if(nullptr != m_QuatsPtr.lock())                                                                                  /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_Quats = m_QuatsPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */
  if(getErrorCode() >= 0)
  {
    dataArrayPaths.push_back(getQuatsArrayPath());
  }

  cDims[0] = 1;
  m_CrystalStructuresPtr =
      getDataContainerArray()->getPrereqArrayFromPath<DataArray<uint32_t>>(this, getCrystalStructuresArrayPath(), cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if(nullptr != m_CrystalStructuresPtr.lock()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_CrystalStructures = m_CrystalStructuresPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */

  cDims[0] = 3;
  tempPath.update(getCellPhasesArrayPath().getDataContainerName(), getCellPhasesArrayPath().getAttributeMatrixName(), getMisorientationColorArrayName());
  m_MisorientationColorPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<uint8_t>>(this, tempPath, 0, cDims, "", DataArrayID31);
  if(nullptr != m_MisorientationColorPtr.lock()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_MisorientationColor = m_MisorientationColorPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */

  // The good voxels array is optional, If it is available we are going to use it, otherwise we are going to create it
  cDims[0] = 1;
  if(getUseGoodVoxels())
  {
    // The good voxels array is optional, If it is available we are going to use it, otherwise we are going to create it
    cDims[0] = 1;
    m_GoodVoxelsPtr =
        getDataContainerArray()->getPrereqArrayFromPath<DataArray<bool>>(this, getGoodVoxelsArrayPath(), cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
    if(nullptr != m_GoodVoxelsPtr.lock())                                                                        /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
    {
      m_GoodVoxels = m_GoodVoxelsPtr.lock()->getPointer(0);
    } /* Now assign the raw pointer to data from the DataArray<T> object */
    if(getErrorCode() >= 0)
    {
      dataArrayPaths.push_back(getGoodVoxelsArrayPath());
    }
  }
  else
  {
    m_GoodVoxels = nullptr;
  }

  getDataContainerArray()->validateNumberOfTuples(this, dataArrayPaths);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMisorientationColors::execute()
{
  clearErrorCode();
  clearWarningCode();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  size_t totalPoints = m_CellPhasesPtr.lock()->getNumberOfTuples();

  // Make sure we are dealing with a unit 1 vector.
  FloatVec3Type normRefDir = m_ReferenceAxis; // Make a copy of the reference Direction

  MatrixMath::Normalize3x1(normRefDir[0], normRefDir[1], normRefDir[2]);
  // Create 1 of every type of Ops class. This condenses the code below
  UInt8ArrayType::Pointer notSupported = UInt8ArrayType::CreateArray(13, "_INTERNAL_USE_ONLY_NotSupportedArray", true);
  notSupported->initializeWithZeros();

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
  if(true)
  {
    tbb::parallel_for(tbb::blocked_range<size_t>(0, totalPoints),
                      GenerateMisorientationColorsImpl(normRefDir, m_ReferenceAngle, m_Quats, m_CellPhases, m_CrystalStructures, m_GoodVoxels, notSupported->getPointer(0), m_MisorientationColor),
                      tbb::auto_partitioner());
  }
  else
#endif
  {
    GenerateMisorientationColorsImpl serial(normRefDir, m_ReferenceAngle, m_Quats, m_CellPhases, m_CrystalStructures, m_GoodVoxels, notSupported->getPointer(0), m_MisorientationColor);
    serial.convert(0, totalPoints);
  }

  std::vector<LaueOps::Pointer> ops = LaueOps::GetAllOrientationOps();

  // Check and warn about unsupported crystal symmetries in the computation which will show as black
  for(size_t i = 0; i < notSupported->getNumberOfTuples() - 1; i++)
  {
    if(notSupported->getValue(i) == 1)
    {
      QString msg("The symmetry of ");
      msg.append(ops[i]->getSymmetryName()).append(" is not currently supported for misorientation coloring. Elements with this symmetry have been set to black");
      setWarningCondition(-5000, msg);
    }
  }

  // Check for bad voxels which will show up as black also.
  if(notSupported->getValue(12) == 1)
  {
    QString msg("There were elements with an unknown crystal symmetry due most likely being marked as 'a 'bad'. These elements have been colored black BUT black is a valid color for misorientation "
                "coloring. Please understand this when visualizing your data");
    setWarningCondition(-5001, msg);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer GenerateMisorientationColors::newFilterInstance(bool copyFilterParameters) const
{
  GenerateMisorientationColors::Pointer filter = GenerateMisorientationColors::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString GenerateMisorientationColors::getCompiledLibraryName() const
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString GenerateMisorientationColors::getBrandingString() const
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString GenerateMisorientationColors::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString GenerateMisorientationColors::getGroupName() const
{
  return SIMPL::FilterGroups::ProcessingFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid GenerateMisorientationColors::getUuid() const
{
  return QUuid("{7152790d-26a4-571a-8fef-493120eced6d}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString GenerateMisorientationColors::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::CrystallographyFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString GenerateMisorientationColors::getHumanLabel() const
{
  return "Generate Misorientation Colors";
}

// -----------------------------------------------------------------------------
GenerateMisorientationColors::Pointer GenerateMisorientationColors::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<GenerateMisorientationColors> GenerateMisorientationColors::New()
{
  struct make_shared_enabler : public GenerateMisorientationColors
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString GenerateMisorientationColors::getNameOfClass() const
{
  return QString("GenerateMisorientationColors");
}

// -----------------------------------------------------------------------------
QString GenerateMisorientationColors::ClassName()
{
  return QString("GenerateMisorientationColors");
}

// -----------------------------------------------------------------------------
void GenerateMisorientationColors::setReferenceAxis(const FloatVec3Type& value)
{
  m_ReferenceAxis = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type GenerateMisorientationColors::getReferenceAxis() const
{
  return m_ReferenceAxis;
}

// -----------------------------------------------------------------------------
void GenerateMisorientationColors::setReferenceAngle(float value)
{
  m_ReferenceAngle = value;
}

// -----------------------------------------------------------------------------
float GenerateMisorientationColors::getReferenceAngle() const
{
  return m_ReferenceAngle;
}

// -----------------------------------------------------------------------------
void GenerateMisorientationColors::setCellPhasesArrayPath(const DataArrayPath& value)
{
  m_CellPhasesArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath GenerateMisorientationColors::getCellPhasesArrayPath() const
{
  return m_CellPhasesArrayPath;
}

// -----------------------------------------------------------------------------
void GenerateMisorientationColors::setQuatsArrayPath(const DataArrayPath& value)
{
  m_QuatsArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath GenerateMisorientationColors::getQuatsArrayPath() const
{
  return m_QuatsArrayPath;
}

// -----------------------------------------------------------------------------
void GenerateMisorientationColors::setCrystalStructuresArrayPath(const DataArrayPath& value)
{
  m_CrystalStructuresArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath GenerateMisorientationColors::getCrystalStructuresArrayPath() const
{
  return m_CrystalStructuresArrayPath;
}

// -----------------------------------------------------------------------------
void GenerateMisorientationColors::setGoodVoxelsArrayPath(const DataArrayPath& value)
{
  m_GoodVoxelsArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath GenerateMisorientationColors::getGoodVoxelsArrayPath() const
{
  return m_GoodVoxelsArrayPath;
}

// -----------------------------------------------------------------------------
void GenerateMisorientationColors::setMisorientationColorArrayName(const QString& value)
{
  m_MisorientationColorArrayName = value;
}

// -----------------------------------------------------------------------------
QString GenerateMisorientationColors::getMisorientationColorArrayName() const
{
  return m_MisorientationColorArrayName;
}

// -----------------------------------------------------------------------------
void GenerateMisorientationColors::setUseGoodVoxels(bool value)
{
  m_UseGoodVoxels = value;
}

// -----------------------------------------------------------------------------
bool GenerateMisorientationColors::getUseGoodVoxels() const
{
  return m_UseGoodVoxels;
}
