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

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedPathCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/DataContainers/DataContainer.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"
#include "UCSBUtilities/UCSBUtilitiesVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CopyAttributeMatrix::CopyAttributeMatrix()
: m_SelectedAttributeMatrixPath("", "", "")
, m_NewAttributeMatrix("")
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CopyAttributeMatrix::~CopyAttributeMatrix() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeMatrix::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  {
    AttributeMatrixSelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_AM_SELECTION_FP("Attribute Matrix to Copy", SelectedAttributeMatrixPath, FilterParameter::RequiredArray, CopyAttributeMatrix, req));
  }

  parameters.push_back(SIMPL_NEW_AM_WITH_LINKED_DC_FP("Copied Attribute Matrix", NewAttributeMatrix, SelectedAttributeMatrixPath, FilterParameter::CreatedArray, CopyAttributeMatrix));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeMatrix::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedAttributeMatrixPath(reader->readDataArrayPath("SelectedAttributeMatrixPath", getSelectedAttributeMatrixPath()));
  setNewAttributeMatrix(reader->readString("NewAttributeMatrix", getNewAttributeMatrix()));
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
  clearErrorCode();
  clearWarningCode();

  if(m_NewAttributeMatrix.isEmpty())
  {
    QString ss = QObject::tr("The new Attribute Matrix name must be set");
    setErrorCondition(-11004, ss);
  }

  AttributeMatrix::Pointer attrMat = getDataContainerArray()->getPrereqAttributeMatrixFromPath(this, getSelectedAttributeMatrixPath(), -301);
  if(getErrorCode() < 0)
  {
    return;
  }

  DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(getSelectedAttributeMatrixPath().getDataContainerName());

  AttributeMatrix::Pointer p = attrMat->deepCopy(getInPreflight());
  p->setName(getNewAttributeMatrix());
  dc->addOrReplaceAttributeMatrix(p);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeMatrix::execute()
{
  clearErrorCode();
  clearWarningCode();
  dataCheck(); // calling the dataCheck will rename the array, so nothing is required here
  if(getErrorCode() < 0)
  {
    return;
  }
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer CopyAttributeMatrix::newFilterInstance(bool copyFilterParameters) const
{
  CopyAttributeMatrix::Pointer filter = CopyAttributeMatrix::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyAttributeMatrix::getCompiledLibraryName() const
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyAttributeMatrix::getBrandingString() const
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyAttributeMatrix::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyAttributeMatrix::getGroupName() const
{
  return SIMPL::FilterGroups::CoreFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid CopyAttributeMatrix::getUuid() const
{
  return QUuid("{9bc962eb-f363-5caf-9f82-23a26be8ae2f}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyAttributeMatrix::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::MemoryManagementFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyAttributeMatrix::getHumanLabel() const
{
  return "Copy Attribute Matrix";
}

// -----------------------------------------------------------------------------
CopyAttributeMatrix::Pointer CopyAttributeMatrix::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<CopyAttributeMatrix> CopyAttributeMatrix::New()
{
  struct make_shared_enabler : public CopyAttributeMatrix
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString CopyAttributeMatrix::getNameOfClass() const
{
  return QString("CopyAttributeMatrix");
}

// -----------------------------------------------------------------------------
QString CopyAttributeMatrix::ClassName()
{
  return QString("CopyAttributeMatrix");
}

// -----------------------------------------------------------------------------
void CopyAttributeMatrix::setSelectedAttributeMatrixPath(const DataArrayPath& value)
{
  m_SelectedAttributeMatrixPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath CopyAttributeMatrix::getSelectedAttributeMatrixPath() const
{
  return m_SelectedAttributeMatrixPath;
}

// -----------------------------------------------------------------------------
void CopyAttributeMatrix::setNewAttributeMatrix(const QString& value)
{
  m_NewAttributeMatrix = value;
}

// -----------------------------------------------------------------------------
QString CopyAttributeMatrix::getNewAttributeMatrix() const
{
  return m_NewAttributeMatrix;
}
