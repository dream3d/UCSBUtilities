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

#include "Symmetric6x6FilterParameter.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Symmetric6x6FilterParameter::Symmetric6x6FilterParameter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Symmetric6x6FilterParameter::~Symmetric6x6FilterParameter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Symmetric6x6FilterParameter::Pointer Symmetric6x6FilterParameter::New(const QString& humanLabel, const QString& propertyName, const FloatVec21_t& defaultValue, Category category,
                                                                      const SetterCallbackType& setterCallback, const GetterCallbackType& getterCallback, int groupIndex)
{

  Symmetric6x6FilterParameter::Pointer ptr = Symmetric6x6FilterParameter::New();
  ptr->setHumanLabel(humanLabel);
  ptr->setPropertyName(propertyName);
  QVariant v;
  v.setValue(defaultValue);
  ptr->setDefaultValue(v);
  ptr->setCategory(category);
  ptr->setGroupIndex(groupIndex);
  ptr->setSetterCallback(setterCallback);
  ptr->setGetterCallback(getterCallback);

  return ptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Symmetric6x6FilterParameter::getWidgetType() const
{
  return QString("Symmetric6x6Widget");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Symmetric6x6FilterParameter::readJson(const QJsonObject& json)
{
  FloatVec21_t compliances;
  QJsonArray compliancesArray = json["Compliances"].toArray();
  compliances.readJson(compliancesArray);
  m_SetterCallback(compliances);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Symmetric6x6FilterParameter::writeJson(QJsonObject& json)
{
  FloatVec21_t compliances = m_GetterCallback();
  QJsonArray compliancesArray;
  compliances.writeJson(compliancesArray);
  json["Compliances"] = compliancesArray;
}

// -----------------------------------------------------------------------------
Symmetric6x6FilterParameter::Pointer Symmetric6x6FilterParameter::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
Symmetric6x6FilterParameter::Pointer Symmetric6x6FilterParameter::New()
{
  Pointer sharedPtr(new(Symmetric6x6FilterParameter));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
QString Symmetric6x6FilterParameter::getNameOfClass() const
{
  return QString("Symmetric6x6FilterParameter");
}

// -----------------------------------------------------------------------------
QString Symmetric6x6FilterParameter::ClassName()
{
  return QString("Symmetric6x6FilterParameter");
}

// -----------------------------------------------------------------------------
void Symmetric6x6FilterParameter::setSetterCallback(const Symmetric6x6FilterParameter::SetterCallbackType& value)
{
  m_SetterCallback = value;
}

// -----------------------------------------------------------------------------
Symmetric6x6FilterParameter::SetterCallbackType Symmetric6x6FilterParameter::getSetterCallback() const
{
  return m_SetterCallback;
}

// -----------------------------------------------------------------------------
void Symmetric6x6FilterParameter::setGetterCallback(const Symmetric6x6FilterParameter::GetterCallbackType& value)
{
  m_GetterCallback = value;
}

// -----------------------------------------------------------------------------
Symmetric6x6FilterParameter::GetterCallbackType Symmetric6x6FilterParameter::getGetterCallback() const
{
  return m_GetterCallback;
}
