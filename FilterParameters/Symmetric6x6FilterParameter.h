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

#pragma once

#include <memory>

#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include "SIMPLib/FilterParameters/FilterParameter.h"

typedef struct
{
  float v11;
  float v12;
  float v13;
  float v14;
  float v15;
  float v16;
  float v22;
  float v23;
  float v24;
  float v25;
  float v26;
  float v33;
  float v34;
  float v35;
  float v36;
  float v44;
  float v45;
  float v46;
  float v55;
  float v56;
  float v66;

  void writeJson(QJsonArray& json)
  {
    json.push_back(v11);
    json.push_back(v12);
    json.push_back(v13);
    json.push_back(v14);
    json.push_back(v15);
    json.push_back(v16);
    json.push_back(v22);
    json.push_back(v23);
    json.push_back(v24);
    json.push_back(v25);
    json.push_back(v26);
    json.push_back(v33);
    json.push_back(v34);
    json.push_back(v35);
    json.push_back(v36);
    json.push_back(v44);
    json.push_back(v45);
    json.push_back(v46);
    json.push_back(v55);
    json.push_back(v56);
    json.push_back(v66);
  }

  bool readJson(QJsonArray& json)
  {
    if(json.size() == 21)
    {
      v11 = static_cast<float>(json[0].toDouble());
      v12 = static_cast<float>(json[1].toDouble());
      v13 = static_cast<float>(json[2].toDouble());
      v14 = static_cast<float>(json[3].toDouble());
      v15 = static_cast<float>(json[4].toDouble());
      v16 = static_cast<float>(json[5].toDouble());
      v22 = static_cast<float>(json[6].toDouble());
      v23 = static_cast<float>(json[7].toDouble());
      v24 = static_cast<float>(json[8].toDouble());
      v25 = static_cast<float>(json[9].toDouble());
      v26 = static_cast<float>(json[10].toDouble());
      v33 = static_cast<float>(json[11].toDouble());
      v34 = static_cast<float>(json[12].toDouble());
      v35 = static_cast<float>(json[13].toDouble());
      v36 = static_cast<float>(json[14].toDouble());
      v44 = static_cast<float>(json[15].toDouble());
      v45 = static_cast<float>(json[16].toDouble());
      v46 = static_cast<float>(json[17].toDouble());
      v55 = static_cast<float>(json[18].toDouble());
      v56 = static_cast<float>(json[19].toDouble());
      v66 = static_cast<float>(json[20].toDouble());
      return true;
    }
    return false;
  }
} FloatVec21_t;

Q_DECLARE_METATYPE(FloatVec21_t)

class Symmetric6x6FilterParameter : public FilterParameter
{
public:
  using Self = Symmetric6x6FilterParameter;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static Pointer New();

  /**
   * @brief Returns the name of the class for Symmetric6x6FilterParameter
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for Symmetric6x6FilterParameter
   */
  static QString ClassName();

  typedef std::function<void(FloatVec21_t)> SetterCallbackType;
  typedef std::function<FloatVec21_t(void)> GetterCallbackType;

  static Pointer New(const QString& humanLabel, const QString& propertyName, const FloatVec21_t& defaultValue, Category category, const SetterCallbackType& setterCallback,
                     const GetterCallbackType& getterCallback, int groupIndex = -1);

  virtual ~Symmetric6x6FilterParameter();

  /**
   * @brief getWidgetType Returns the type of widget that displays and controls
   * this FilterParameter subclass
   * @return
   */
  QString getWidgetType() const override;

  /**
   * @brief readJson
   * @return
   */
  void readJson(const QJsonObject& json);

  /**
   * @brief writeJson
   * @return
   */
  void writeJson(QJsonObject& json) override;

  /**
   * @brief Setter property for SetterCallback
   */
  void setSetterCallback(const Symmetric6x6FilterParameter::SetterCallbackType& value);
  /**
   * @brief Getter property for SetterCallback
   * @return Value of SetterCallback
   */
  Symmetric6x6FilterParameter::SetterCallbackType getSetterCallback() const;

  /**
   * @brief Setter property for GetterCallback
   */
  void setGetterCallback(const Symmetric6x6FilterParameter::GetterCallbackType& value);
  /**
   * @brief Getter property for GetterCallback
   * @return Value of GetterCallback
   */
  Symmetric6x6FilterParameter::GetterCallbackType getGetterCallback() const;

protected:
  Symmetric6x6FilterParameter();

private:
  Symmetric6x6FilterParameter::SetterCallbackType m_SetterCallback = {};
  Symmetric6x6FilterParameter::GetterCallbackType m_GetterCallback = {};

  Symmetric6x6FilterParameter(const Symmetric6x6FilterParameter&) = delete; // Copy Constructor Not Implemented
  void operator=(const Symmetric6x6FilterParameter&) = delete;              // Move assignment Not Implemented
};
