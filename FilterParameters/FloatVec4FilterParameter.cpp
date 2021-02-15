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

#include "FloatVec4FilterParameter.h"
Q_DECLARE_METATYPE(FloatVec4Type)

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FloatVec4FilterParameter::FloatVec4FilterParameter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FloatVec4FilterParameter::~FloatVec4FilterParameter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FloatVec4FilterParameter::Pointer FloatVec4FilterParameter::Create(const QString& humanLabel, const QString& propertyName, const FloatVec4Type& defaultValue, Category category,
                                                                   const SetterCallbackType& setterCallback, const GetterCallbackType& getterCallback, int groupIndex)
{
  FloatVec4FilterParameter::Pointer ptr = FloatVec4FilterParameter::New();
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
QString FloatVec4FilterParameter::getWidgetType() const
{
  return QString("FloatVec4Widget");
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FloatVec4FilterParameter::readJson(const QJsonObject& json)
{
  QJsonValue jsonValue = json[getPropertyName()];
  if(jsonValue.isUndefined())
  {
    jsonValue = json[getLegacyPropertyName()];
  }
  if(!jsonValue.isUndefined() && m_SetterCallback)
  {
    QJsonObject json = jsonValue.toObject();
    FloatVec4Type floatVec4;
    if(json["a"].isDouble() && json["b"].isDouble() && json["c"].isDouble() && json["d"].isDouble())
    {
      floatVec4[0] = static_cast<float>(json["a"].toDouble());
      floatVec4[1] = static_cast<float>(json["b"].toDouble());
      floatVec4[2] = static_cast<float>(json["c"].toDouble());
      floatVec4[3] = static_cast<float>(json["d"].toDouble());
    }
    m_SetterCallback(floatVec4);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FloatVec4FilterParameter::writeJson(QJsonObject& obj)
{
  if(m_GetterCallback)
  {
    FloatVec4Type floatVec4 = m_GetterCallback();
    QJsonObject json;
    json["a"] = static_cast<double>(floatVec4[0]);
    json["b"] = static_cast<double>(floatVec4[1]);
    json["c"] = static_cast<double>(floatVec4[2]);
    json["d"] = static_cast<double>(floatVec4[3]);
    obj[getPropertyName()] = json;
  }
}

// -----------------------------------------------------------------------------
FloatVec4FilterParameter::Pointer FloatVec4FilterParameter::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
FloatVec4FilterParameter::Pointer FloatVec4FilterParameter::New()
{
  Pointer sharedPtr(new(FloatVec4FilterParameter));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
QString FloatVec4FilterParameter::getNameOfClass() const
{
  return QString("FloatVec4FilterParameter");
}

// -----------------------------------------------------------------------------
QString FloatVec4FilterParameter::ClassName()
{
  return QString("FloatVec4FilterParameter");
}

// -----------------------------------------------------------------------------
void FloatVec4FilterParameter::setSetterCallback(const FloatVec4FilterParameter::SetterCallbackType& value)
{
  m_SetterCallback = value;
}

// -----------------------------------------------------------------------------
FloatVec4FilterParameter::SetterCallbackType FloatVec4FilterParameter::getSetterCallback() const
{
  return m_SetterCallback;
}

// -----------------------------------------------------------------------------
void FloatVec4FilterParameter::setGetterCallback(const FloatVec4FilterParameter::GetterCallbackType& value)
{
  m_GetterCallback = value;
}

// -----------------------------------------------------------------------------
FloatVec4FilterParameter::GetterCallbackType FloatVec4FilterParameter::getGetterCallback() const
{
  return m_GetterCallback;
}
