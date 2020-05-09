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

#include "InputCrystalCompliances.h"

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArrayCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"
#include "UCSBUtilities/FilterParameters/Symmetric6x6FilterParameter.h"
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
InputCrystalCompliances::InputCrystalCompliances()
: m_CrystalCompliancesArrayPath("", "", "")
{
  m_Compliances.v11 = 1;
  m_Compliances.v12 = 1;
  m_Compliances.v13 = 1;
  m_Compliances.v14 = 0;
  m_Compliances.v15 = 0;
  m_Compliances.v16 = 0;

  m_Compliances.v22 = 1;
  m_Compliances.v23 = 1;
  m_Compliances.v24 = 0;
  m_Compliances.v25 = 0;
  m_Compliances.v26 = 0;

  m_Compliances.v33 = 1;
  m_Compliances.v34 = 0;
  m_Compliances.v35 = 0;
  m_Compliances.v36 = 0;

  m_Compliances.v44 = 1;
  m_Compliances.v45 = 0;
  m_Compliances.v46 = 0;

  m_Compliances.v55 = 1;
  m_Compliances.v56 = 0;

  m_Compliances.v66 = 1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
InputCrystalCompliances::~InputCrystalCompliances() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InputCrystalCompliances::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  parameters.push_back(Symmetric6x6FilterParameter::New("Compliance Values (10^-11 Pa^-1)", "Compliances", getCompliances(), FilterParameter::Parameter,
                                                        SIMPL_BIND_SETTER(InputCrystalCompliances, this, Compliances), SIMPL_BIND_GETTER(InputCrystalCompliances, this, Compliances)));
  parameters.push_back(SeparatorFilterParameter::New("Ensemble Data", FilterParameter::CreatedArray));
  {
    DataArrayCreationFilterParameter::RequirementType req = DataArrayCreationFilterParameter::CreateRequirement(AttributeMatrix::Category::Ensemble);
    parameters.push_back(SIMPL_NEW_DA_CREATION_FP("Crystal Compliances", CrystalCompliancesArrayPath, FilterParameter::CreatedArray, InputCrystalCompliances, req));
  }
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InputCrystalCompliances::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);

  QVector<float> tmp(21);
  reader->readArray("Compliances", tmp);
  m_Compliances.v11 = tmp[0];
  m_Compliances.v12 = tmp[1];
  m_Compliances.v13 = tmp[2];
  m_Compliances.v14 = tmp[3];
  m_Compliances.v15 = tmp[4];
  m_Compliances.v16 = tmp[5];
  m_Compliances.v22 = tmp[6];
  m_Compliances.v23 = tmp[7];
  m_Compliances.v24 = tmp[8];
  m_Compliances.v25 = tmp[9];
  m_Compliances.v26 = tmp[10];
  m_Compliances.v33 = tmp[11];
  m_Compliances.v34 = tmp[12];
  m_Compliances.v35 = tmp[13];
  m_Compliances.v36 = tmp[14];
  m_Compliances.v44 = tmp[15];
  m_Compliances.v45 = tmp[16];
  m_Compliances.v46 = tmp[17];
  m_Compliances.v55 = tmp[18];
  m_Compliances.v56 = tmp[19];
  m_Compliances.v66 = tmp[20];
  setCrystalCompliancesArrayPath(reader->readDataArrayPath("CrystalCompliancesArrayPath", getCrystalCompliancesArrayPath()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InputCrystalCompliances::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InputCrystalCompliances::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  // create compliances
  std::vector<size_t> cDims(2, 6); // 6 by 6 array
  m_CrystalCompliancesPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<float>>(this, getCrystalCompliancesArrayPath(), 0, cDims, "", DataArrayID31);
  if(nullptr != m_CrystalCompliancesPtr.lock())
  {
    m_CrystalCompliances = m_CrystalCompliancesPtr.lock()->getPointer(0);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InputCrystalCompliances::execute()
{
  clearErrorCode();
  clearWarningCode();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  // determine number of phases
  size_t numPhases = m_CrystalCompliancesPtr.lock()->getNumberOfTuples();

  // convert from 10^-2 GPa^-1 to Pa^-1
  m_Compliances.v11 /= 100000000000;
  m_Compliances.v12 /= 100000000000;
  m_Compliances.v13 /= 100000000000;
  m_Compliances.v14 /= 100000000000;
  m_Compliances.v15 /= 100000000000;
  m_Compliances.v16 /= 100000000000;

  m_Compliances.v22 /= 100000000000;
  m_Compliances.v23 /= 100000000000;
  m_Compliances.v24 /= 100000000000;
  m_Compliances.v25 /= 100000000000;
  m_Compliances.v26 /= 100000000000;

  m_Compliances.v33 /= 100000000000;
  m_Compliances.v34 /= 100000000000;
  m_Compliances.v35 /= 100000000000;
  m_Compliances.v36 /= 100000000000;

  m_Compliances.v44 /= 100000000000;
  m_Compliances.v45 /= 100000000000;
  m_Compliances.v46 /= 100000000000;

  m_Compliances.v55 /= 100000000000;
  m_Compliances.v56 /= 100000000000;

  m_Compliances.v66 /= 100000000000;

  // loop over each phase
  for(size_t i = 0; i < numPhases; i++)
  {
    // for now just give every phase the same values
    size_t index = 36 * i;
    m_CrystalCompliances[index + 0] = m_Compliances.v11;
    m_CrystalCompliances[index + 1] = m_Compliances.v12;
    m_CrystalCompliances[index + 2] = m_Compliances.v13;
    m_CrystalCompliances[index + 3] = m_Compliances.v14;
    m_CrystalCompliances[index + 4] = m_Compliances.v15;
    m_CrystalCompliances[index + 5] = m_Compliances.v16;

    m_CrystalCompliances[index + 6] = m_Compliances.v12;
    m_CrystalCompliances[index + 7] = m_Compliances.v22;
    m_CrystalCompliances[index + 8] = m_Compliances.v23;
    m_CrystalCompliances[index + 9] = m_Compliances.v24;
    m_CrystalCompliances[index + 10] = m_Compliances.v25;
    m_CrystalCompliances[index + 11] = m_Compliances.v26;

    m_CrystalCompliances[index + 12] = m_Compliances.v13;
    m_CrystalCompliances[index + 13] = m_Compliances.v23;
    m_CrystalCompliances[index + 14] = m_Compliances.v33;
    m_CrystalCompliances[index + 15] = m_Compliances.v34;
    m_CrystalCompliances[index + 16] = m_Compliances.v35;
    m_CrystalCompliances[index + 17] = m_Compliances.v36;

    m_CrystalCompliances[index + 18] = m_Compliances.v14;
    m_CrystalCompliances[index + 19] = m_Compliances.v24;
    m_CrystalCompliances[index + 20] = m_Compliances.v34;
    m_CrystalCompliances[index + 21] = m_Compliances.v44;
    m_CrystalCompliances[index + 22] = m_Compliances.v45;
    m_CrystalCompliances[index + 23] = m_Compliances.v46;

    m_CrystalCompliances[index + 24] = m_Compliances.v15;
    m_CrystalCompliances[index + 25] = m_Compliances.v25;
    m_CrystalCompliances[index + 26] = m_Compliances.v35;
    m_CrystalCompliances[index + 27] = m_Compliances.v45;
    m_CrystalCompliances[index + 28] = m_Compliances.v55;
    m_CrystalCompliances[index + 29] = m_Compliances.v56;

    m_CrystalCompliances[index + 30] = m_Compliances.v16;
    m_CrystalCompliances[index + 31] = m_Compliances.v26;
    m_CrystalCompliances[index + 32] = m_Compliances.v36;
    m_CrystalCompliances[index + 33] = m_Compliances.v46;
    m_CrystalCompliances[index + 34] = m_Compliances.v56;
    m_CrystalCompliances[index + 35] = m_Compliances.v66;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer InputCrystalCompliances::newFilterInstance(bool copyFilterParameters) const
{
  InputCrystalCompliances::Pointer filter = InputCrystalCompliances::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString InputCrystalCompliances::getCompiledLibraryName() const
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString InputCrystalCompliances::getBrandingString() const
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString InputCrystalCompliances::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString InputCrystalCompliances::getGroupName() const
{
  return SIMPL::FilterGroups::Generic;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid InputCrystalCompliances::getUuid() const
{
  return QUuid("{436eab43-0531-5e56-9309-5931109a85ca}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString InputCrystalCompliances::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::CrystallographyFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString InputCrystalCompliances::getHumanLabel() const
{
  return "Input Crystal Compliances";
}

// -----------------------------------------------------------------------------
InputCrystalCompliances::Pointer InputCrystalCompliances::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<InputCrystalCompliances> InputCrystalCompliances::New()
{
  struct make_shared_enabler : public InputCrystalCompliances
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString InputCrystalCompliances::getNameOfClass() const
{
  return QString("InputCrystalCompliances");
}

// -----------------------------------------------------------------------------
QString InputCrystalCompliances::ClassName()
{
  return QString("InputCrystalCompliances");
}

// -----------------------------------------------------------------------------
void InputCrystalCompliances::setCompliances(const FloatVec21_t& value)
{
  m_Compliances = value;
}

// -----------------------------------------------------------------------------
FloatVec21_t InputCrystalCompliances::getCompliances() const
{
  return m_Compliances;
}

// -----------------------------------------------------------------------------
void InputCrystalCompliances::setCrystalCompliancesArrayPath(const DataArrayPath& value)
{
  m_CrystalCompliancesArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath InputCrystalCompliances::getCrystalCompliancesArrayPath() const
{
  return m_CrystalCompliancesArrayPath;
}
