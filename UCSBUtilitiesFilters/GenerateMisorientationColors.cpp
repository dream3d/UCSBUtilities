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

#ifdef SIMPLib_USE_PARALLEL_ALGORITHMS
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/partitioner.h>
#include <tbb/task_scheduler_init.h>
#endif

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersWriter.h"

#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/Utilities/ColorTable.h"

#include "OrientationLib/SpaceGroupOps/SpaceGroupOps.h"

#include "UCSBUtilitiesFilters/SpaceGroupOps/CubicOpsMisoColor.h"
#include "UCSBUtilitiesFilters/SpaceGroupOps/CubicLowOpsMisoColor.h"
#include "UCSBUtilitiesFilters/SpaceGroupOps/HexagonalOpsMisoColor.h"
#include "UCSBUtilitiesFilters/SpaceGroupOps/HexagonalLowOpsMisoColor.h"
#include "UCSBUtilitiesFilters/SpaceGroupOps/OrthoRhombicOpsMisoColor.h"
#include "UCSBUtilitiesFilters/SpaceGroupOps/TrigonalOpsMisoColor.h"
#include "UCSBUtilitiesFilters/SpaceGroupOps/TetragonalOpsMisoColor.h"
#include "UCSBUtilitiesFilters/SpaceGroupOps/TrigonalLowOpsMisoColor.h"
#include "UCSBUtilitiesFilters/SpaceGroupOps/TetragonalLowOpsMisoColor.h"
#include "UCSBUtilitiesFilters/SpaceGroupOps/TriclinicOpsMisoColor.h"
#include "UCSBUtilitiesFilters/SpaceGroupOps/MonoclinicOpsMisoColor.h"

#include "UCSBUtilities/UCSBUtilitiesVersion.h"
#include "UCSBUtilities/UCSBUtilitiesConstants.h"

/**
 * @brief The GenerateMisorientationColorsImpl class implements a threaded algorithm that computes the misorientation
 * colors for each element in a geometry.
 */
class GenerateMisorientationColorsImpl
{
  public:
    GenerateMisorientationColorsImpl(FloatVec3_t referenceAxis, float refAngle, QuatF* quats, int32_t* phases, uint32_t* crystalStructures,
                                     bool* goodVoxels, uint8_t* notSupported, uint8_t* colors) :
      m_ReferenceAxis(referenceAxis),
      m_ReferenceAngle(refAngle),
      m_Quats(quats),
      m_CellPhases(phases),
      m_CrystalStructures(crystalStructures),
      m_GoodVoxels(goodVoxels),
      m_NotSupported(notSupported),
      m_MisorientationColor(colors)
    {}
    virtual ~GenerateMisorientationColorsImpl() {}

    void convert(size_t start, size_t end) const
    {
      QVector<SpaceGroupOps::Pointer> ops;
      ops.push_back(HexagonalOpsMisoColor::New());

      ops.push_back(CubicOpsMisoColor::New());

      ops.push_back(HexagonalLowOpsMisoColor::New()); // Hex Low
      ops.push_back(CubicLowOpsMisoColor::New()); // Cubic Low
      ops.push_back(TriclinicOpsMisoColor::New()); // Triclinic
      ops.push_back(MonoclinicOpsMisoColor::New()); // Monoclinic

      ops.push_back(OrthoRhombicOpsMisoColor::New());// OrthoRhombic


      ops.push_back(TetragonalLowOpsMisoColor::New()); // Tetragonal-low
      ops.push_back(TetragonalOpsMisoColor::New());// Tetragonal-high

      ops.push_back(TrigonalLowOpsMisoColor::New()); // Trigonal-low
      ops.push_back(TrigonalOpsMisoColor::New());// Trigonal-High

      ops.push_back(OrthoRhombicOpsMisoColor::New()); // Axis OrthorhombicOps

      QuatF refQuat = {m_ReferenceAxis.x * sinf(m_ReferenceAngle), m_ReferenceAxis.y * sinf(m_ReferenceAngle), m_ReferenceAxis.z * sinf(m_ReferenceAngle), cosf(m_ReferenceAngle)};
      QuatF cellQuat = {0.0f, 0.0f, 0.0f, 1.0f};
      DREAM3D::Rgb argb = 0x00000000;

      bool missingGoodVoxels = false;
      if (NULL == m_GoodVoxels)
      {
        missingGoodVoxels = true;
      }
      int32_t phase = 0;
      size_t index = 0;
      for (size_t i = start; i < end; i++)
      {
        phase = m_CellPhases[i];
        index = i * 3;
        m_MisorientationColor[index] = 0;
        m_MisorientationColor[index + 1] = 0;
        m_MisorientationColor[index + 2] = 0;
        cellQuat = m_Quats[i];

        if (m_CrystalStructures[phase] != Ebsd::CrystalStructure::Cubic_High &&
            m_CrystalStructures[phase] != Ebsd::CrystalStructure::Hexagonal_High)
        {
          uint32_t idx = m_CrystalStructures[phase];
          if (idx == Ebsd::CrystalStructure::UnknownCrystalStructure)
          {
            idx = 12;
          }
          m_NotSupported[idx] = 1;
          m_MisorientationColor[index] = 0;
          m_MisorientationColor[index + 1] = 0;
          m_MisorientationColor[index + 2] = 0;

        }
        // Make sure we are using a valid Euler Angles with valid crystal symmetry
        else if( (missingGoodVoxels == true || m_GoodVoxels[i] == true)
                 && m_CrystalStructures[phase] < Ebsd::CrystalStructure::LaueGroupEnd )
        {
          argb = ops[m_CrystalStructures[phase]]->generateMisorientationColor(cellQuat, refQuat);
          m_MisorientationColor[index] = RgbColor::dRed(argb);
          m_MisorientationColor[index + 1] = RgbColor::dGreen(argb);
          m_MisorientationColor[index + 2] = RgbColor::dBlue(argb);
        }
      }
    }

#ifdef SIMPLib_USE_PARALLEL_ALGORITHMS
    void operator()(const tbb::blocked_range<size_t>& r) const
    {
      convert(r.begin(), r.end());
    }
#endif
  private:
    FloatVec3_t  m_ReferenceAxis;
    float m_ReferenceAngle;
    QuatF* m_Quats;
    int32_t* m_CellPhases;
    uint32_t* m_CrystalStructures;
    bool* m_GoodVoxels;
    uint8_t* m_NotSupported;
    uint8_t* m_MisorientationColor;
};

// Include the MOC generated file for this class
#include "moc_GenerateMisorientationColors.cpp"



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
GenerateMisorientationColors::GenerateMisorientationColors() :
  AbstractFilter(),
  m_ReferenceAngle(0.0f),
  m_CellPhasesArrayPath("", "", ""),
  m_QuatsArrayPath("", "", ""),
  m_CrystalStructuresArrayPath("", "", ""),
  m_GoodVoxelsArrayPath("", "", ""),
  m_MisorientationColorArrayName(DREAM3D::CellData::MisorientationColor),
  m_UseGoodVoxels(false),
  m_CellPhases(NULL),
  m_Quats(NULL),
  m_CrystalStructures(NULL),
  m_GoodVoxels(NULL),
  m_MisorientationColor(NULL)
{
  m_ReferenceAxis.x = 0.0f;
  m_ReferenceAxis.y = 0.0f;
  m_ReferenceAxis.z = 1.0f;
  m_ReferenceAngle = 0.0f;

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
GenerateMisorientationColors::~GenerateMisorientationColors()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMisorientationColors::setupFilterParameters()
{
  FilterParameterVector parameters;
  parameters.push_back(FloatVec3FilterParameter::New("Reference Orientation Axis", "ReferenceAxis", getReferenceAxis(), FilterParameter::Parameter));
  parameters.push_back(DoubleFilterParameter::New("Reference Orientation Angle (Degrees)", "ReferenceAngle", getReferenceAngle(), FilterParameter::Parameter));
  QStringList linkedProps("GoodVoxelsArrayPath");
  parameters.push_back(LinkedBooleanFilterParameter::New("Apply to Good Elements Only (Bad Elements Will Be Black)", "UseGoodVoxels", getUseGoodVoxels(), linkedProps, FilterParameter::Parameter));
  parameters.push_back(SeparatorFilterParameter::New("Element Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(DREAM3D::TypeNames::Float, 4, DREAM3D::AttributeMatrixObjectType::Element);
    parameters.push_back(DataArraySelectionFilterParameter::New("Quaternions", "QuatsArrayPath", getQuatsArrayPath(), FilterParameter::RequiredArray, req));
  }
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(DREAM3D::TypeNames::Int32, 1, DREAM3D::AttributeMatrixObjectType::Element);
    parameters.push_back(DataArraySelectionFilterParameter::New("Phases", "CellPhasesArrayPath", getCellPhasesArrayPath(), FilterParameter::RequiredArray, req));
  }
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(DREAM3D::TypeNames::Bool, 1, DREAM3D::AttributeMatrixObjectType::Element);
    parameters.push_back(DataArraySelectionFilterParameter::New("Mask", "GoodVoxelsArrayPath", getGoodVoxelsArrayPath(), FilterParameter::RequiredArray, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Ensemble Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(DREAM3D::TypeNames::UInt32, 1, DREAM3D::AttributeMatrixObjectType::Ensemble);
    parameters.push_back(DataArraySelectionFilterParameter::New("Crystal Structures", "CrystalStructuresArrayPath", getCrystalStructuresArrayPath(), FilterParameter::RequiredArray, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Element Data", FilterParameter::CreatedArray));
  parameters.push_back(StringFilterParameter::New("Misorientation Colors", "MisorientationColorArrayName", getMisorientationColorArrayName(), FilterParameter::CreatedArray));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMisorientationColors::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setUseGoodVoxels(reader->readValue("UseGoodVoxels", getUseGoodVoxels() ) );
  setMisorientationColorArrayName(reader->readString("MisorientationColorArrayName", getMisorientationColorArrayName() ) );
  setGoodVoxelsArrayPath(reader->readDataArrayPath("GoodVoxelsArrayPath", getGoodVoxelsArrayPath() ) );
  setCrystalStructuresArrayPath(reader->readDataArrayPath("CrystalStructuresArrayPath", getCrystalStructuresArrayPath() ) );
  setQuatsArrayPath(reader->readDataArrayPath("QuatsArrayPath", getQuatsArrayPath() ) );
  setCellPhasesArrayPath(reader->readDataArrayPath("CellPhasesArrayPath", getCellPhasesArrayPath() ) );
  setReferenceAxis( reader->readFloatVec3("ReferenceAxis", getReferenceAxis() ) );
  setReferenceAngle( reader->readValue("ReferenceAngle", getReferenceAngle() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int GenerateMisorientationColors::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  SIMPL_FILTER_WRITE_PARAMETER(FilterVersion)
  SIMPL_FILTER_WRITE_PARAMETER(UseGoodVoxels)
  SIMPL_FILTER_WRITE_PARAMETER(MisorientationColorArrayName)
  SIMPL_FILTER_WRITE_PARAMETER(GoodVoxelsArrayPath)
  SIMPL_FILTER_WRITE_PARAMETER(CrystalStructuresArrayPath)
  SIMPL_FILTER_WRITE_PARAMETER(QuatsArrayPath)
  SIMPL_FILTER_WRITE_PARAMETER(CellPhasesArrayPath)
  SIMPL_FILTER_WRITE_PARAMETER(ReferenceAxis)
  SIMPL_FILTER_WRITE_PARAMETER(ReferenceAngle)
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMisorientationColors::dataCheck()
{
  setErrorCondition(0);
  DataArrayPath tempPath;

  QVector<DataArrayPath> dataArrayPaths;

  QVector<size_t> cDims(1, 1);
  m_CellPhasesPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<int32_t>, AbstractFilter>(this, getCellPhasesArrayPath(), cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_CellPhasesPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_CellPhases = m_CellPhasesPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
  if(getErrorCondition() >= 0) { dataArrayPaths.push_back(getCellPhasesArrayPath()); }

  cDims[0] = 4;
  m_QuatsPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<float>, AbstractFilter>(this, getQuatsArrayPath(), cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_QuatsPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_Quats = m_QuatsPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
  if(getErrorCondition() >= 0) { dataArrayPaths.push_back(getQuatsArrayPath()); }

  cDims[0] = 1;
  m_CrystalStructuresPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<uint32_t>, AbstractFilter>(this, getCrystalStructuresArrayPath(), cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_CrystalStructuresPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_CrystalStructures = m_CrystalStructuresPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */

  cDims[0] = 3;
  tempPath.update(getCellPhasesArrayPath().getDataContainerName(), getCellPhasesArrayPath().getAttributeMatrixName(), getMisorientationColorArrayName() );
  m_MisorientationColorPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<uint8_t>, AbstractFilter, uint8_t>(this, tempPath, 0, cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_MisorientationColorPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_MisorientationColor = m_MisorientationColorPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */

  // The good voxels array is optional, If it is available we are going to use it, otherwise we are going to create it
  cDims[0] = 1;
  if (getUseGoodVoxels() == true)
  {
    // The good voxels array is optional, If it is available we are going to use it, otherwise we are going to create it
    cDims[0] = 1;
    m_GoodVoxelsPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<bool>, AbstractFilter>(this, getGoodVoxelsArrayPath(), cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
    if( NULL != m_GoodVoxelsPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
    { m_GoodVoxels = m_GoodVoxelsPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
    if(getErrorCondition() >= 0) { dataArrayPaths.push_back(getGoodVoxelsArrayPath()); }
  }
  else
  {
    m_GoodVoxels = NULL;
  }

  getDataContainerArray()->validateNumberOfTuples<AbstractFilter>(this, dataArrayPaths);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMisorientationColors::preflight()
{
  setInPreflight(true);
  emit preflightAboutToExecute();
  emit updateFilterParameters(this);
  dataCheck();
  emit preflightExecuted();
  setInPreflight(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMisorientationColors::execute()
{
  setErrorCondition(0);
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  size_t totalPoints = m_CellPhasesPtr.lock()->getNumberOfTuples();

  // Make sure we are dealing with a unit 1 vector.
  FloatVec3_t normRefDir = m_ReferenceAxis; // Make a copy of the reference Direction

  MatrixMath::Normalize3x1(normRefDir.x, normRefDir.y, normRefDir.z);
  // Create 1 of every type of Ops class. This condenses the code below
  UInt8ArrayType::Pointer notSupported = UInt8ArrayType::CreateArray(13, "_INTERNAL_USE_ONLY_NotSupportedArray");
  notSupported->initializeWithZeros();

#ifdef SIMPLib_USE_PARALLEL_ALGORITHMS
  tbb::task_scheduler_init init;
  bool doParallel = true;
#endif

#ifdef SIMPLib_USE_PARALLEL_ALGORITHMS
  if (doParallel == true)
  {
    tbb::parallel_for(tbb::blocked_range<size_t>(0, totalPoints),
                      GenerateMisorientationColorsImpl( normRefDir, m_ReferenceAngle, reinterpret_cast<QuatF*>(m_Quats), m_CellPhases, m_CrystalStructures, m_GoodVoxels, notSupported->getPointer(0), m_MisorientationColor), tbb::auto_partitioner());

  }
  else
#endif
  {
    GenerateMisorientationColorsImpl serial( normRefDir, m_ReferenceAngle, reinterpret_cast<QuatF*>(m_Quats), m_CellPhases, m_CrystalStructures, m_GoodVoxels, notSupported->getPointer(0), m_MisorientationColor);
    serial.convert(0, totalPoints);
  }

  QVector<SpaceGroupOps::Pointer> ops = SpaceGroupOps::getOrientationOpsQVector();

  // Check and warn about unsupported crystal symmetries in the computation which will show as black
  for (size_t i = 0; i < notSupported->getNumberOfTuples() - 1; i++)
  {
    if (notSupported->getValue(i) == 1)
    {
      QString msg("The symmetry of ");
      msg.append(ops[i]->getSymmetryName()).append(" is not currently supported for misorientation coloring. Elements with this symmetry have been set to black");
      notifyWarningMessage(getHumanLabel(), msg, -5000);
    }
  }

  // Check for bad voxels which will show up as black also.
  if (notSupported->getValue(12) == 1)
  {
    QString msg("There were elements with an unknown crystal symmetry due most likely being marked as 'a 'bad'. These elements have been colored black BUT black is a valid color for misorientation coloring. Please understand this when visualizing your data");
    notifyWarningMessage(getHumanLabel(), msg, -5001);
  }

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer GenerateMisorientationColors::newFilterInstance(bool copyFilterParameters)
{
  GenerateMisorientationColors::Pointer filter = GenerateMisorientationColors::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString GenerateMisorientationColors::getCompiledLibraryName()
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString GenerateMisorientationColors::getBrandingString()
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString GenerateMisorientationColors::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString GenerateMisorientationColors::getGroupName()
{ return DREAM3D::FilterGroups::ProcessingFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString GenerateMisorientationColors::getSubGroupName()
{ return DREAM3D::FilterSubGroups::CrystallographyFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString GenerateMisorientationColors::getHumanLabel()
{ return "Generate Misorientation Colors"; }
