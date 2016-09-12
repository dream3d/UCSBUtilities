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

#include "ClearDataMask.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"
#include "UCSBUtilities/UCSBUtilitiesVersion.h"


// Include the MOC generated file for this class
#include "moc_ClearDataMask.cpp"



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ClearDataMask::ClearDataMask() :
  AbstractFilter(),
  m_MaskArrayPath("", "", ""),
  m_Mask(nullptr)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ClearDataMask::~ClearDataMask()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ClearDataMask::setupFilterParameters()
{
  FilterParameterVector parameters;
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(SIMPL::TypeNames::Bool, 1, SIMPL::AttributeMatrixObjectType::Element);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Mask", MaskArrayPath, FilterParameter::RequiredArray, ClearDataMask, req));
  }
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ClearDataMask::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setMaskArrayPath( reader->readDataArrayPath("MaskArrayPath", getMaskArrayPath() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ClearDataMask::initialize()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ClearDataMask::dataCheck()
{
  setErrorCondition(0);

  QVector<size_t> cDims(1, 1);
  m_MaskPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<bool>, AbstractFilter>(this, getMaskArrayPath(), cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( nullptr != m_MaskPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  { m_Mask = m_MaskPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ClearDataMask::preflight()
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
void ClearDataMask::execute()
{
  setErrorCondition(0);
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  DataContainer::Pointer m = getDataContainerArray()->getDataContainer(m_MaskArrayPath.getDataContainerName());
  size_t totalPoints = m_MaskPtr.lock()->getNumberOfTuples();

  // get list of array names
  QString attrMatName = m_MaskArrayPath.getAttributeMatrixName();
  QList<QString> voxelArrayNames = m->getAttributeMatrix(attrMatName)->getAttributeArrayNames();

  // convert to list of pointers
  std::vector<IDataArray::Pointer> arrayList;
  for (QList<QString>::iterator iter = voxelArrayNames.begin(); iter != voxelArrayNames.end(); ++iter)
  {
    IDataArray::Pointer p = m->getAttributeMatrix(attrMatName)->getAttributeArray(*iter);
    arrayList.push_back(p);
  }

  int32_t numArrays = arrayList.size();

  for (size_t i = 0; i < totalPoints; i++)
  {
    if (!m_Mask[i])
    {
      for (int32_t j = 0; j < numArrays; j++)
      {
        int var = 0;
        arrayList[j]->initializeTuple(i, &var);
      }
    }
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ClearDataMask::newFilterInstance(bool copyFilterParameters)
{
  ClearDataMask::Pointer filter = ClearDataMask::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ClearDataMask::getCompiledLibraryName()
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ClearDataMask::getBrandingString()
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ClearDataMask::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ClearDataMask::getGroupName()
{ return SIMPL::FilterGroups::ProcessingFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ClearDataMask::getSubGroupName()
{ return SIMPL::FilterSubGroups::CleanupFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ClearDataMask::getHumanLabel()
{ return "Clear Data (Mask)"; }
