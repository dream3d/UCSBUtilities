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

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArrayCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"
#include "UCSBUtilities/UCSBUtilitiesVersion.h"

// Include the MOC generated file for this class
#include "moc_FindBoundaryAreas.cpp"



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindBoundaryAreas::FindBoundaryAreas() :
  AbstractFilter(),
  m_SurfaceMeshTriangleAreasArrayPath(SIMPL::Defaults::TriangleDataContainerName, SIMPL::Defaults::FaceAttributeMatrixName, SIMPL::FaceData::SurfaceMeshFaceAreas),
  m_SurfaceMeshFeatureFaceIdsArrayPath(SIMPL::Defaults::TriangleDataContainerName, SIMPL::Defaults::FaceAttributeMatrixName, SIMPL::CellData::FeatureIds),
  m_SurfaceMeshBoundaryAreasArrayPath(SIMPL::Defaults::TriangleDataContainerName, SIMPL::Defaults::FaceFeatureAttributeMatrixName, "BoundaryAreas"),
  m_SurfaceMeshTriangleAreas(NULL),
  m_SurfaceMeshFeatureFaceIds(NULL),
  m_SurfaceMeshBoundaryAreas(NULL)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindBoundaryAreas::~FindBoundaryAreas()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindBoundaryAreas::setupFilterParameters()
{
  FilterParameterVector parameters;

  {
    DataArraySelectionFilterParameter::RequirementType req;
    parameters.push_back(DataArraySelectionFilterParameter::New("Triangle Areas", "SurfaceMeshTriangleAreasArrayPath", getSurfaceMeshTriangleAreasArrayPath(), FilterParameter::RequiredArray, SIMPL_BIND_SETTER(FindBoundaryAreas, this, SurfaceMeshTriangleAreasArrayPath), SIMPL_BIND_GETTER(FindBoundaryAreas, this, SurfaceMeshTriangleAreasArrayPath), req));
  }
  {
    DataArraySelectionFilterParameter::RequirementType req;
    parameters.push_back(DataArraySelectionFilterParameter::New("Face Feature Ids", "SurfaceMeshFeatureFaceIdsArrayPath", getSurfaceMeshFeatureFaceIdsArrayPath(), FilterParameter::RequiredArray, SIMPL_BIND_SETTER(FindBoundaryAreas, this, SurfaceMeshFeatureFaceIdsArrayPath), SIMPL_BIND_GETTER(FindBoundaryAreas, this, SurfaceMeshFeatureFaceIdsArrayPath), req));
  }

  {
    DataArrayCreationFilterParameter::RequirementType req;
    parameters.push_back(DataArrayCreationFilterParameter::New("Boundary Areas Array", "SurfaceMeshBoundaryAreasArrayPath", getSurfaceMeshBoundaryAreasArrayPath(), FilterParameter::CreatedArray, SIMPL_BIND_SETTER(FindBoundaryAreas, this, SurfaceMeshBoundaryAreasArrayPath), SIMPL_BIND_GETTER(FindBoundaryAreas, this, SurfaceMeshBoundaryAreasArrayPath), req));
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
  setSurfaceMeshFeatureFaceIdsArrayPath(reader->readDataArrayPath("SurfaceMeshFeatureFaceIdsArrayPath", getSurfaceMeshFeatureFaceIdsArrayPath() ) );
  setSurfaceMeshTriangleAreasArrayPath(reader->readDataArrayPath("SurfaceMeshTriangleAreasArrayPath", getSurfaceMeshTriangleAreasArrayPath() ) );
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
  setErrorCondition(0);

  QVector<size_t> dims(1, 1);
  m_SurfaceMeshFeatureFaceIdsPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<int32_t>, AbstractFilter>(this, getSurfaceMeshFeatureFaceIdsArrayPath(), dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_SurfaceMeshFeatureFaceIdsPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_SurfaceMeshFeatureFaceIds = m_SurfaceMeshFeatureFaceIdsPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */

  m_SurfaceMeshTriangleAreasPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<double>, AbstractFilter>(this, getSurfaceMeshTriangleAreasArrayPath(), dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_SurfaceMeshTriangleAreasPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_SurfaceMeshTriangleAreas = m_SurfaceMeshTriangleAreasPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */

  m_SurfaceMeshBoundaryAreasPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<double>, AbstractFilter, double>(this, getSurfaceMeshBoundaryAreasArrayPath(), 0.0, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_SurfaceMeshBoundaryAreasPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_SurfaceMeshBoundaryAreas = m_SurfaceMeshBoundaryAreasPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindBoundaryAreas::preflight()
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
void FindBoundaryAreas::execute()
{
  setErrorCondition(0);
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  // get number of face features
  size_t numFaces = m_SurfaceMeshTriangleAreasPtr.lock()->getNumberOfTuples();
  size_t numFeatures = m_SurfaceMeshBoundaryAreasPtr.lock()->getNumberOfTuples();

  // initialize all areas as 0
  for (size_t i = 0; i < numFeatures; i++)
  {
    m_SurfaceMeshBoundaryAreas[i] = 0.0;
  }

  // loop over faces accumulating area
  for (size_t i = 0; i < numFaces; i++)
  {
    m_SurfaceMeshBoundaryAreas[m_SurfaceMeshFeatureFaceIds[i]] += m_SurfaceMeshTriangleAreas[i];
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer FindBoundaryAreas::newFilterInstance(bool copyFilterParameters)
{
  FindBoundaryAreas::Pointer filter = FindBoundaryAreas::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindBoundaryAreas::getCompiledLibraryName()
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindBoundaryAreas::getBrandingString()
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindBoundaryAreas::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindBoundaryAreas::getGroupName()
{ return SIMPL::FilterGroups::SurfaceMeshingFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindBoundaryAreas::getSubGroupName()
{ return SIMPL::FilterSubGroups::MiscFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindBoundaryAreas::getHumanLabel()
{ return "Find Face Feature Boundary Areas"; }

