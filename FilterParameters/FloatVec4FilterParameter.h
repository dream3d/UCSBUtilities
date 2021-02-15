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

#include "SIMPLib/Common/SIMPLArray.hpp"
#include "SIMPLib/FilterParameters/FilterParameter.h"

#include "UCSBUtilities/UCSBUtilitiesDLLExport.h"

/**
 * @brief SIMPL_NEW_FLOAT_FP This macro is a short-form way of instantiating an instance of
 * FloatFilterParameter. There are 4 required parameters and 1 optional parameter
 * that are always passed to this macro in the following order: HumanLabel, PropertyName, Category,
 * FilterName (class name), GroupIndex (optional).
 *
 * Therefore, the macro should be written like this (this is a concrete example):
 * SIMPL_NEW_FLOAT_FP("HumanLabel", PropertyName, Category, FilterName, GroupIndex)
 *
 * Example 1 (instantiated within a filter called [GenericExample](@ref genericexample), without optional GroupIndex parameter):
 * SIMPL_NEW_FLOAT_FP("Float 2", Float2, FilterParameter::Category::Parameter, GenericExample);
 */
#define SIMPL_NEW_FLOAT_VEC4_FP(...)                                                                                                                                                                   \
  SIMPL_EXPAND(_FP_GET_OVERRIDE(__VA_ARGS__, SIMPL_NEW_FP_9, SIMPL_NEW_FP_8, SIMPL_NEW_FP_7, SIMPL_NEW_FP_6, SIMPL_NEW_FP_5, SIMPL_NEW_FP_4)(FloatFilterParameter, __VA_ARGS__))

class UCSBUtilities_EXPORT FloatVec4FilterParameter : public FilterParameter
{
public:
  using Self = FloatVec4FilterParameter;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static Pointer New();

  /**
   * @brief Returns the name of the class for FloatVec4FilterParameter
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for FloatVec4FilterParameter
   */
  static QString ClassName();

  using SetterCallbackType = std::function<void(FloatVec4Type)>;
  using GetterCallbackType = std::function<FloatVec4Type(void)>;

  /**
   * @brief New
   * @param humanLabel
   * @param propertyName
   * @param defaultValue
   * @param category
   * @param setterCallback
   * @param getterCallback
   * @param groupIndex
   * @return
   */
  static Pointer Create(const QString& humanLabel, const QString& propertyName, const FloatVec4Type& defaultValue, Category category, const SetterCallbackType& setterCallback,
                        const GetterCallbackType& getterCallback, int groupIndex = -1);

  virtual ~FloatVec4FilterParameter();

  /**
   * @brief getWidgetType Returns the type of widget that displays and controls
   * this FilterParameter subclass
   * @return
   */
  QString getWidgetType() const override;

  /**
   * @brief readJson Reads this filter parameter's corresponding property out of a QJsonObject.
   * @param json The QJsonObject that the filter parameter reads from.
   */
  void readJson(const QJsonObject& json) override;

  /**
   * @brief writeJson Writes this filter parameter's corresponding property to a QJsonObject.
   * @param json The QJsonObject that the filter parameter writes to.
   */
  void writeJson(QJsonObject& json) override;

  /**
   * @param SetterCallback The method in the AbstractFilter subclass that <i>sets</i> the value of the property
   * that this FilterParameter subclass represents.
   * from the filter parameter.
   */
  /**
   * @brief Setter property for SetterCallback
   */
  void setSetterCallback(const FloatVec4FilterParameter::SetterCallbackType& value);
  /**
   * @brief Getter property for SetterCallback
   * @return Value of SetterCallback
   */
  FloatVec4FilterParameter::SetterCallbackType getSetterCallback() const;

  /**
   * @param GetterCallback The method in the AbstractFilter subclass that <i>gets</i> the value of the property
   * that this FilterParameter subclass represents.
   * @return The GetterCallback
   */
  /**
   * @brief Setter property for GetterCallback
   */
  void setGetterCallback(const FloatVec4FilterParameter::GetterCallbackType& value);
  /**
   * @brief Getter property for GetterCallback
   * @return Value of GetterCallback
   */
  FloatVec4FilterParameter::GetterCallbackType getGetterCallback() const;

protected:
  FloatVec4FilterParameter();

public:
  FloatVec4FilterParameter(const FloatVec4FilterParameter&) = delete;            // Copy Constructor Not Implemented
  FloatVec4FilterParameter(FloatVec4FilterParameter&&) = delete;                 // Move Constructor Not Implemented
  FloatVec4FilterParameter& operator=(const FloatVec4FilterParameter&) = delete; // Copy Assignment Not Implemented
  FloatVec4FilterParameter& operator=(FloatVec4FilterParameter&&) = delete;      // Move Assignment Not Implemented

private:
  FloatVec4FilterParameter::SetterCallbackType m_SetterCallback = {};
  FloatVec4FilterParameter::GetterCallbackType m_GetterCallback = {};
};
