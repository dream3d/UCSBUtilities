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

#include <QtCore/QTextStream>

#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedPathCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/DataContainers/DataContainer.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"
#include "UCSBUtilities/UCSBUtilitiesVersion.h"

#include "FindModulusMismatch.h"

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/partitioner.h>

/* Create Enumerations to allow the created Attribute Arrays to take part in renaming */
enum createdPathID : RenameDataPath::DataID_t
{
  DataArrayID30 = 30,
  DataArrayID31 = 31,
};

#endif

class FindModulusMismatchImpl
{
  int32_t* m_Labels;
  float* m_Moduli;
  float* m_DeltaModuli;

public:
  FindModulusMismatchImpl(int32_t* labels, float* mod, float* deltaMod)
  : m_Labels(labels)
  , m_Moduli(mod)
  , m_DeltaModuli(deltaMod)
  {
  }

  virtual ~FindModulusMismatchImpl() = default;

  void generate(size_t start, size_t end) const
  {
    int feature1, feature2;
    float deltaE;

    for(size_t i = start; i < end; i++)
    {
      feature1 = m_Labels[2 * i];
      feature2 = m_Labels[2 * i + 1];
      deltaE = 0.0f;
      if(feature1 > 0 && feature2 > 0)
      {
        deltaE = m_Moduli[feature1] - m_Moduli[feature2];
        if(deltaE < 0)
        {
          deltaE = -deltaE;
        }
      }
      m_DeltaModuli[i] = deltaE;
    }
  }

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
  void operator()(const tbb::blocked_range<size_t>& r) const
  {
    generate(r.begin(), r.end());
  }
#endif
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindModulusMismatch::FindModulusMismatch()
: m_ModuliArrayPath(SIMPL::Defaults::ImageDataContainerName, SIMPL::Defaults::CellFeatureAttributeMatrixName, "ElasticModuli")
, m_SurfaceMeshFaceLabelsArrayPath(SIMPL::Defaults::TriangleDataContainerName, SIMPL::Defaults::FaceAttributeMatrixName, SIMPL::FaceData::SurfaceMeshFaceLabels)
, m_SurfaceMeshDeltaModulusArrayName("SurfaceMeshDeltaModulus")
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindModulusMismatch::~FindModulusMismatch() = default;
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindModulusMismatch::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(SIMPL::Defaults::AnyPrimitive, 1, AttributeMatrix::Category::Any);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Moduli", ModuliArrayPath, FilterParameter::RequiredArray, FindModulusMismatch, req));
  }
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(SIMPL::Defaults::AnyPrimitive, 2, AttributeMatrix::Category::Any);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("SurfaceMeshFaceLabels", SurfaceMeshFaceLabelsArrayPath, FilterParameter::RequiredArray, FindModulusMismatch, req));
  }

  parameters.push_back(SIMPL_NEW_DA_WITH_LINKED_AM_FP("SurfaceMeshDeltaModulus", SurfaceMeshDeltaModulusArrayName, SurfaceMeshFaceLabelsArrayPath, SurfaceMeshFaceLabelsArrayPath,
                                                      FilterParameter::CreatedArray, FindModulusMismatch));

  setFilterParameters(parameters);
}
// -----------------------------------------------------------------------------
void FindModulusMismatch::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSurfaceMeshDeltaModulusArrayName(reader->readString("SurfaceMeshDeltaModulusArrayName", getSurfaceMeshDeltaModulusArrayName()));
  setSurfaceMeshFaceLabelsArrayPath(reader->readDataArrayPath("SurfaceMeshFaceLabelsArrayPath", getSurfaceMeshFaceLabelsArrayPath()));
  setModuliArrayPath(reader->readDataArrayPath("ModuliArrayPath", getModuliArrayPath()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindModulusMismatch::dataCheckVoxel()
{
  clearErrorCode();
  clearWarningCode();

  std::vector<size_t> dims(1, 1);
  m_ModuliPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<float>>(this, getModuliArrayPath(), dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if(nullptr != m_ModuliPtr.lock()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_Moduli = m_ModuliPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindModulusMismatch::dataCheckSurfaceMesh()
{
  DataArrayPath tempPath;
  clearErrorCode();
  clearWarningCode();

  DataContainer::Pointer sm = getDataContainerArray()->getPrereqDataContainer(this, getSurfaceMeshFaceLabelsArrayPath().getDataContainerName());
  if(getErrorCode() < 0 || nullptr == sm.get())
  {
    return;
  }

  std::vector<size_t> dims(1, 2);
  m_SurfaceMeshFaceLabelsPtr =
      getDataContainerArray()->getPrereqArrayFromPath<DataArray<int32_t>>(this, getSurfaceMeshFaceLabelsArrayPath(), dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if(nullptr != m_SurfaceMeshFaceLabelsPtr.lock()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_SurfaceMeshFaceLabels = m_SurfaceMeshFaceLabelsPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */
  dims[0] = 1;
  tempPath.update(m_SurfaceMeshFaceLabelsArrayPath.getDataContainerName(), m_SurfaceMeshFaceLabelsArrayPath.getAttributeMatrixName(), getSurfaceMeshDeltaModulusArrayName());
  m_SurfaceMeshDeltaModulusPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<float>>(this, tempPath, 180.0, dims, "", DataArrayID31);
  if(nullptr != m_SurfaceMeshDeltaModulusPtr.lock()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_SurfaceMeshDeltaModulus = m_SurfaceMeshDeltaModulusPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindModulusMismatch::dataCheck()
{
  dataCheckVoxel();
  dataCheckSurfaceMesh();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindModulusMismatch::execute()
{
  clearErrorCode();
  clearWarningCode();
  dataCheckVoxel();
  if(getErrorCode() < 0)
  {
    return;
  }
  dataCheckSurfaceMesh();
  if(getErrorCode() < 0)
  {
    return;
  }

  int64_t numTriangles = m_SurfaceMeshFaceLabelsPtr.lock()->getNumberOfTuples();

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
  if(true)
  {
    tbb::parallel_for(tbb::blocked_range<size_t>(0, numTriangles), FindModulusMismatchImpl(m_SurfaceMeshFaceLabels, m_Moduli, m_SurfaceMeshDeltaModulus), tbb::auto_partitioner());
  }
  else
#endif
  {
    FindModulusMismatchImpl serial(m_SurfaceMeshFaceLabels, m_Moduli, m_SurfaceMeshDeltaModulus);
    serial.generate(0, numTriangles);
  }

  notifyStatusMessage("Completed");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer FindModulusMismatch::newFilterInstance(bool copyFilterParameters) const
{
  FindModulusMismatch::Pointer filter = FindModulusMismatch::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindModulusMismatch::getCompiledLibraryName() const
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindModulusMismatch::getBrandingString() const
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindModulusMismatch::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindModulusMismatch::getGroupName() const
{
  return SIMPL::FilterGroups::StatisticsFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid FindModulusMismatch::getUuid() const
{
  return QUuid("{a0b4c16f-bfb1-57cf-aba1-eb08b5486abb}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindModulusMismatch::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::CrystallographyFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindModulusMismatch::getHumanLabel() const
{
  return "Find Elastic Modulus Mismatch";
}

// -----------------------------------------------------------------------------
FindModulusMismatch::Pointer FindModulusMismatch::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<FindModulusMismatch> FindModulusMismatch::New()
{
  struct make_shared_enabler : public FindModulusMismatch
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString FindModulusMismatch::getNameOfClass() const
{
  return QString("FindModulusMismatch");
}

// -----------------------------------------------------------------------------
QString FindModulusMismatch::ClassName()
{
  return QString("FindModulusMismatch");
}

// -----------------------------------------------------------------------------
void FindModulusMismatch::setModuliArrayPath(const DataArrayPath& value)
{
  m_ModuliArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath FindModulusMismatch::getModuliArrayPath() const
{
  return m_ModuliArrayPath;
}

// -----------------------------------------------------------------------------
void FindModulusMismatch::setSurfaceMeshFaceLabelsArrayPath(const DataArrayPath& value)
{
  m_SurfaceMeshFaceLabelsArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath FindModulusMismatch::getSurfaceMeshFaceLabelsArrayPath() const
{
  return m_SurfaceMeshFaceLabelsArrayPath;
}

// -----------------------------------------------------------------------------
void FindModulusMismatch::setSurfaceMeshDeltaModulusArrayName(const QString& value)
{
  m_SurfaceMeshDeltaModulusArrayName = value;
}

// -----------------------------------------------------------------------------
QString FindModulusMismatch::getSurfaceMeshDeltaModulusArrayName() const
{
  return m_SurfaceMeshDeltaModulusArrayName;
}
