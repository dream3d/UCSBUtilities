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

#include "CopyAttributeArray.h"

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedPathCreationFilterParameter.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"
#include "UCSBUtilities/UCSBUtilitiesVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CopyAttributeArray::CopyAttributeArray()
: m_SelectedArrayPath("", "", "")
, m_NewArrayName("")
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CopyAttributeArray::~CopyAttributeArray() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeArray::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  {
    DataArraySelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to Copy", SelectedArrayPath, FilterParameter::RequiredArray, CopyAttributeArray, req));
  }

  parameters.push_back(SIMPL_NEW_DA_WITH_LINKED_AM_FP("Copied Attribute Array", NewArrayName, SelectedArrayPath, SelectedArrayPath, FilterParameter::CreatedArray, CopyAttributeArray));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeArray::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedArrayPath(reader->readDataArrayPath("SelectedArrayPath", getSelectedArrayPath()));
  setNewArrayName(reader->readString("NewArrayName", getNewArrayName()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeArray::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeArray::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  if(m_NewArrayName.isEmpty())
  {
    QString ss = QObject::tr("The new Attribute Array name must be set");
    setErrorCondition(-11009, ss);
    return;
  }

  QString daName = getSelectedArrayPath().getDataArrayName();

  IDataArray::Pointer dataArray = getDataContainerArray()->getPrereqIDataArrayFromPath(this, getSelectedArrayPath());
  if(getErrorCode() < 0)
  {
    return;
  }

  DataArrayPath path(getSelectedArrayPath().getDataContainerName(), getSelectedArrayPath().getAttributeMatrixName(), "");
  AttributeMatrix::Pointer attrMat = getDataContainerArray()->getAttributeMatrix(path);

  IDataArray::Pointer pNew = dataArray->deepCopy();
  pNew->setName(m_NewArrayName); // Set the name of the array
  int32_t err = attrMat->insertOrAssign(pNew);

  if(0 != err)
  {
    QString ss = QObject::tr("Attempt to copy Attribute Array '%1' to '%2' failed").arg(daName).arg(m_NewArrayName);
    setErrorCondition(err, ss);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeArray::execute()
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
AbstractFilter::Pointer CopyAttributeArray::newFilterInstance(bool copyFilterParameters) const
{
  CopyAttributeArray::Pointer filter = CopyAttributeArray::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyAttributeArray::getCompiledLibraryName() const
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyAttributeArray::getBrandingString() const
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyAttributeArray::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyAttributeArray::getGroupName() const
{
  return SIMPL::FilterGroups::CoreFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid CopyAttributeArray::getUuid() const
{
  return QUuid("{06d9ebfd-4c69-566a-8f4c-30e133a523e7}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyAttributeArray::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::MemoryManagementFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CopyAttributeArray::getHumanLabel() const
{
  return "Copy Attribute Array";
}

// -----------------------------------------------------------------------------
CopyAttributeArray::Pointer CopyAttributeArray::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<CopyAttributeArray> CopyAttributeArray::New()
{
  struct make_shared_enabler : public CopyAttributeArray
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString CopyAttributeArray::getNameOfClass() const
{
  return QString("CopyAttributeArray");
}

// -----------------------------------------------------------------------------
QString CopyAttributeArray::ClassName()
{
  return QString("CopyAttributeArray");
}

// -----------------------------------------------------------------------------
void CopyAttributeArray::setSelectedArrayPath(const DataArrayPath& value)
{
  m_SelectedArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath CopyAttributeArray::getSelectedArrayPath() const
{
  return m_SelectedArrayPath;
}

// -----------------------------------------------------------------------------
void CopyAttributeArray::setNewArrayName(const QString& value)
{
  m_NewArrayName = value;
}

// -----------------------------------------------------------------------------
QString CopyAttributeArray::getNewArrayName() const
{
  return m_NewArrayName;
}
