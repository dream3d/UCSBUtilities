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

#include "FindBoundaryAreas.h"

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArrayCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"
#include "UCSBUtilities/UCSBUtilitiesVersion.h"

/* Create Enumerations to allow the created Attribute Arrays to take part in renaming */
enum createdPathID : RenameDataPath::DataID_t
{
  DataArrayID30 = 30,
  DataArrayID31 = 31,
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindBoundaryAreas::FindBoundaryAreas()
: m_SurfaceMeshTriangleAreasArrayPath(SIMPL::Defaults::TriangleDataContainerName, SIMPL::Defaults::FaceAttributeMatrixName, SIMPL::FaceData::SurfaceMeshFaceAreas)
, m_SurfaceMeshFeatureFaceIdsArrayPath(SIMPL::Defaults::TriangleDataContainerName, SIMPL::Defaults::FaceAttributeMatrixName, SIMPL::CellData::FeatureIds)
, m_SurfaceMeshBoundaryAreasArrayPath(SIMPL::Defaults::TriangleDataContainerName, SIMPL::Defaults::FaceFeatureAttributeMatrixName, "BoundaryAreas")
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindBoundaryAreas::~FindBoundaryAreas() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindBoundaryAreas::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  {
    DataArraySelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Triangle Areas", SurfaceMeshTriangleAreasArrayPath, FilterParameter::RequiredArray, FindBoundaryAreas, req));
  }
  {
    DataArraySelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Face Feature Ids", SurfaceMeshFeatureFaceIdsArrayPath, FilterParameter::RequiredArray, FindBoundaryAreas, req));
  }

  {
    DataArrayCreationFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_DA_CREATION_FP("Boundary Areas Array", SurfaceMeshBoundaryAreasArrayPath, FilterParameter::CreatedArray, FindBoundaryAreas, req));
  }

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindBoundaryAreas::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSurfaceMeshBoundaryAreasArrayPath(reader->readDataArrayPath("SurfaceMeshBoundaryAreasArrayPath", getSurfaceMeshBoundaryAreasArrayPath()));
  setSurfaceMeshFeatureFaceIdsArrayPath(reader->readDataArrayPath("SurfaceMeshFeatureFaceIdsArrayPath", getSurfaceMeshFeatureFaceIdsArrayPath()));
  setSurfaceMeshTriangleAreasArrayPath(reader->readDataArrayPath("SurfaceMeshTriangleAreasArrayPath", getSurfaceMeshTriangleAreasArrayPath()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindBoundaryAreas::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindBoundaryAreas::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  std::vector<size_t> dims(1, 1);
  m_SurfaceMeshFeatureFaceIdsPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<int32_t>>(this, getSurfaceMeshFeatureFaceIdsArrayPath(),
                                                                                                       dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if(nullptr != m_SurfaceMeshFeatureFaceIdsPtr.lock())                                                        /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_SurfaceMeshFeatureFaceIds = m_SurfaceMeshFeatureFaceIdsPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */

  m_SurfaceMeshTriangleAreasPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<double>>(this, getSurfaceMeshTriangleAreasArrayPath(),
                                                                                                     dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if(nullptr != m_SurfaceMeshTriangleAreasPtr.lock())                                                       /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_SurfaceMeshTriangleAreas = m_SurfaceMeshTriangleAreasPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */

  m_SurfaceMeshBoundaryAreasPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<double>>(this, getSurfaceMeshBoundaryAreasArrayPath(), 0.0, dims, "", DataArrayID31);
  if(nullptr != m_SurfaceMeshBoundaryAreasPtr.lock()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
    m_SurfaceMeshBoundaryAreas = m_SurfaceMeshBoundaryAreasPtr.lock()->getPointer(0);
  } /* Now assign the raw pointer to data from the DataArray<T> object */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindBoundaryAreas::execute()
{
  clearErrorCode();
  clearWarningCode();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  // get number of face features
  size_t numFaces = m_SurfaceMeshTriangleAreasPtr.lock()->getNumberOfTuples();
  size_t numFeatures = m_SurfaceMeshBoundaryAreasPtr.lock()->getNumberOfTuples();

  // initialize all areas as 0
  for(size_t i = 0; i < numFeatures; i++)
  {
    m_SurfaceMeshBoundaryAreas[i] = 0.0;
  }

  // loop over faces accumulating area
  for(size_t i = 0; i < numFaces; i++)
  {
    m_SurfaceMeshBoundaryAreas[m_SurfaceMeshFeatureFaceIds[i]] += m_SurfaceMeshTriangleAreas[i];
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer FindBoundaryAreas::newFilterInstance(bool copyFilterParameters) const
{
  FindBoundaryAreas::Pointer filter = FindBoundaryAreas::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindBoundaryAreas::getCompiledLibraryName() const
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindBoundaryAreas::getBrandingString() const
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindBoundaryAreas::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindBoundaryAreas::getGroupName() const
{
  return SIMPL::FilterGroups::SurfaceMeshingFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid FindBoundaryAreas::getUuid() const
{
  return QUuid("{0cdb2c7f-55cb-5fc7-9108-b0c6826bd803}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindBoundaryAreas::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::MiscFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindBoundaryAreas::getHumanLabel() const
{
  return "Find Face Feature Boundary Areas";
}

// -----------------------------------------------------------------------------
FindBoundaryAreas::Pointer FindBoundaryAreas::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<FindBoundaryAreas> FindBoundaryAreas::New()
{
  struct make_shared_enabler : public FindBoundaryAreas
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString FindBoundaryAreas::getNameOfClass() const
{
  return QString("FindBoundaryAreas");
}

// -----------------------------------------------------------------------------
QString FindBoundaryAreas::ClassName()
{
  return QString("FindBoundaryAreas");
}

// -----------------------------------------------------------------------------
void FindBoundaryAreas::setSurfaceMeshTriangleAreasArrayPath(const DataArrayPath& value)
{
  m_SurfaceMeshTriangleAreasArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath FindBoundaryAreas::getSurfaceMeshTriangleAreasArrayPath() const
{
  return m_SurfaceMeshTriangleAreasArrayPath;
}

// -----------------------------------------------------------------------------
void FindBoundaryAreas::setSurfaceMeshFeatureFaceIdsArrayPath(const DataArrayPath& value)
{
  m_SurfaceMeshFeatureFaceIdsArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath FindBoundaryAreas::getSurfaceMeshFeatureFaceIdsArrayPath() const
{
  return m_SurfaceMeshFeatureFaceIdsArrayPath;
}

// -----------------------------------------------------------------------------
void FindBoundaryAreas::setSurfaceMeshBoundaryAreasArrayPath(const DataArrayPath& value)
{
  m_SurfaceMeshBoundaryAreasArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath FindBoundaryAreas::getSurfaceMeshBoundaryAreasArrayPath() const
{
  return m_SurfaceMeshBoundaryAreasArrayPath;
}
