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

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataContainerSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

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
  FilterParameterVector parameters;

  {
    DataContainerSelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_DC_SELECTION_FP("Data Container to Copy", SelectedDataContainerName, FilterParameter::RequiredArray, CopyDataContainer, req));
  }

  parameters.push_back(SIMPL_NEW_STRING_FP("Copied Data Container", NewDataContainerName, FilterParameter::CreatedArray, CopyDataContainer));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyDataContainer::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedDataContainerName( reader->readString("SelectedDataContainerName", getSelectedDataContainerName()) );
  setNewDataContainerName( reader->readString( "NewDataContainerName", getNewDataContainerName() ) );
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
  setErrorCondition(0);
  setWarningCondition(0);

  if(getNewDataContainerName().isEmpty())
  {
    setErrorCondition(-11001);
    QString ss = QObject::tr("The new Data Container name must be set");
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  DataContainer::Pointer dc = getDataContainerArray()->getPrereqDataContainer(this, getSelectedDataContainerName());
  if(getErrorCondition() < 0) { return; }

  DataContainer::Pointer dcNew = dc->deepCopy(getInPreflight());
  dcNew->setName(getNewDataContainerName());
  getDataContainerArray()->addDataContainer(dcNew);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyDataContainer::preflight()
{
  setInPreflight(true);
  Q_EMIT preflightAboutToExecute();
  Q_EMIT updateFilterParameters(this);
  dataCheck();
  Q_EMIT preflightExecuted();
  setInPreflight(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyDataContainer::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  dataCheck(); // calling the dataCheck will copy the array, so nothing is required here
  if(getErrorCondition() < 0) { return; }

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
const QString CopyDataContainer::getCompiledLibraryName() const
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyDataContainer::getBrandingString() const
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyDataContainer::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyDataContainer::getGroupName() const
{ return SIMPL::FilterGroups::CoreFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid CopyDataContainer::getUuid()
{
  return QUuid("{ac8d51d8-9167-5628-a060-95a8863a76b1}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyDataContainer::getSubGroupName() const
{ return SIMPL::FilterSubGroups::MemoryManagementFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyDataContainer::getHumanLabel() const
{ return "Copy Data Container"; }
