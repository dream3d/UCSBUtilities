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

#include "CopyDataContainer.h"

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/DataContainers/DataContainer.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"
#include "UCSBUtilities/UCSBUtilitiesVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CopyDataContainer::CopyDataContainer()
: m_SelectedDataContainerName("")
, m_NewDataContainerName("")
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CopyDataContainer::~CopyDataContainer() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyDataContainer::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  {
    DataContainerSelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_DC_SELECTION_FP("Data Container to Copy", SelectedDataContainerName, FilterParameter::RequiredArray, CopyDataContainer, req));
  }

  parameters.push_back(SIMPL_NEW_DC_CREATION_FP("Copied Data Container", NewDataContainerName, FilterParameter::CreatedArray, CopyDataContainer));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyDataContainer::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedDataContainerName(reader->readDataArrayPath("SelectedDataContainerName", getSelectedDataContainerName()));
  setNewDataContainerName(reader->readDataArrayPath("NewDataContainerName", getNewDataContainerName()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyDataContainer::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyDataContainer::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  if(getNewDataContainerName().isEmpty())
  {
    QString ss = QObject::tr("The new Data Container name must be set");
    setErrorCondition(-11001, ss);
  }

  DataContainer::Pointer dc = getDataContainerArray()->getPrereqDataContainer(this, getSelectedDataContainerName());
  if(getErrorCode() < 0)
  {
    return;
  }

  DataContainer::Pointer dcNew = dc->deepCopy(getInPreflight());
  dcNew->setName(getNewDataContainerName().getDataContainerName());
  getDataContainerArray()->addOrReplaceDataContainer(dcNew);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyDataContainer::execute()
{
  clearErrorCode();
  clearWarningCode();
  dataCheck(); // calling the dataCheck will copy the array, so nothing is required here
  if(getErrorCode() < 0)
  {
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer CopyDataContainer::newFilterInstance(bool copyFilterParameters) const
{
  CopyDataContainer::Pointer filter = CopyDataContainer::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyDataContainer::getCompiledLibraryName() const
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyDataContainer::getBrandingString() const
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyDataContainer::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyDataContainer::getGroupName() const
{
  return SIMPL::FilterGroups::CoreFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid CopyDataContainer::getUuid() const
{
  return QUuid("{ac8d51d8-9167-5628-a060-95a8863a76b1}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyDataContainer::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::MemoryManagementFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyDataContainer::getHumanLabel() const
{
  return "Copy Data Container";
}

// -----------------------------------------------------------------------------
CopyDataContainer::Pointer CopyDataContainer::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<CopyDataContainer> CopyDataContainer::New()
{
  struct make_shared_enabler : public CopyDataContainer
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString CopyDataContainer::getNameOfClass() const
{
  return QString("CopyDataContainer");
}

// -----------------------------------------------------------------------------
QString CopyDataContainer::ClassName()
{
  return QString("CopyDataContainer");
}

// -----------------------------------------------------------------------------
void CopyDataContainer::setSelectedDataContainerName(const DataArrayPath& value)
{
  m_SelectedDataContainerName = value;
}

// -----------------------------------------------------------------------------
DataArrayPath CopyDataContainer::getSelectedDataContainerName() const
{
  return m_SelectedDataContainerName;
}

// -----------------------------------------------------------------------------
void CopyDataContainer::setNewDataContainerName(const DataArrayPath& value)
{
  m_NewDataContainerName = value;
}

// -----------------------------------------------------------------------------
DataArrayPath CopyDataContainer::getNewDataContainerName() const
{
  return m_NewDataContainerName;
}
