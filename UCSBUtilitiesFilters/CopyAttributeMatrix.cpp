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

#include "CopyAttributeMatrix.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersWriter.h"
#include "SIMPLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"
#include "UCSBUtilities/UCSBUtilitiesVersion.h"

// Include the MOC generated file for this class
#include "moc_CopyAttributeMatrix.cpp"



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CopyAttributeMatrix::CopyAttributeMatrix() :
  AbstractFilter(),
  m_SelectedAttributeMatrixPath("", "", ""),
  m_NewAttributeMatrix("")
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CopyAttributeMatrix::~CopyAttributeMatrix()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeMatrix::setupFilterParameters()
{
  FilterParameterVector parameters;

  {
    AttributeMatrixSelectionFilterParameter::RequirementType req;
    parameters.push_back(AttributeMatrixSelectionFilterParameter::New("Attribute Matrix to Copy", "SelectedAttributeMatrixPath", getSelectedAttributeMatrixPath(), FilterParameter::RequiredArray, req, SIMPL_BIND_SETTER(CopyAttributeMatrix, this, SelectedAttributeMatrixPath), SIMPL_BIND_GETTER(CopyAttributeMatrix, this, SelectedAttributeMatrixPath)));
  }

  parameters.push_back(StringFilterParameter::New("Copied Attribute Matrix", "NewAttributeMatrix", getNewAttributeMatrix(), FilterParameter::CreatedArray, SIMPL_BIND_SETTER(CopyAttributeMatrix, this, NewAttributeMatrix), SIMPL_BIND_GETTER(CopyAttributeMatrix, this, NewAttributeMatrix)));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeMatrix::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedAttributeMatrixPath( reader->readDataArrayPath("SelectedAttributeMatrixPath", getSelectedAttributeMatrixPath()) );
  setNewAttributeMatrix( reader->readString( "NewAttributeMatrix", getNewAttributeMatrix() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeMatrix::initialize()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeMatrix::dataCheck()
{
  setErrorCondition(0);

  if (m_NewAttributeMatrix.isEmpty() == true)
  {
    setErrorCondition(-11004);
    QString ss = QObject::tr("The new Attribute Matrix name must be set");
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  AttributeMatrix::Pointer attrMat = getDataContainerArray()->getPrereqAttributeMatrixFromPath<AbstractFilter>(this, getSelectedAttributeMatrixPath(), -301);
  if(getErrorCondition() < 0 ) { return; }

  DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(getSelectedAttributeMatrixPath().getDataContainerName());

  AttributeMatrix::Pointer p = attrMat->deepCopy();
  p->setName(getNewAttributeMatrix());
  dc->addAttributeMatrix(getNewAttributeMatrix(), p );
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeMatrix::preflight()
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
void CopyAttributeMatrix::execute()
{
  setErrorCondition(0);
  dataCheck(); // calling the dataCheck will rename the array, so nothing is required here
  if(getErrorCondition() < 0) { return; }

  notifyStatusMessage(getHumanLabel(), "Complete");
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer CopyAttributeMatrix::newFilterInstance(bool copyFilterParameters)
{
  CopyAttributeMatrix::Pointer filter = CopyAttributeMatrix::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyAttributeMatrix::getCompiledLibraryName()
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyAttributeMatrix::getBrandingString()
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyAttributeMatrix::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyAttributeMatrix::getGroupName()
{ return SIMPL::FilterGroups::CoreFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyAttributeMatrix::getSubGroupName()
{ return SIMPL::FilterSubGroups::MemoryManagementFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyAttributeMatrix::getHumanLabel()
{ return "Copy Attribute Matrix"; }
