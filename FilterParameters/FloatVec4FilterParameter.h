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

#include <QtCore/QJsonObject>

#include "SIMPLib/FilterParameters/FilterParameter.h"

 struct FloatVec4_t
 {
  float a; float b; float c; float d;

  void writeJson(QJsonObject &json)
  {
    json["a"] = a;
    json["b"] = b;
    json["c"] = c;
    json["d"] = d;
  }

  bool readJson(QJsonObject &json)
  {
    if (json["a"].isDouble() && json["b"].isDouble() && json["c"].isDouble() && json["d"].isDouble())
    {
      a = static_cast<float>(json["a"].toDouble());
      b = static_cast<float>(json["b"].toDouble());
      c = static_cast<float>(json["c"].toDouble());
      d = static_cast<float>(json["d"].toDouble());
      return true;
    }
    return false;
  }
} ;

Q_DECLARE_METATYPE(FloatVec4_t)

class FloatVec4FilterParameter : public FilterParameter
{
public:
  SIMPL_SHARED_POINTERS(FloatVec4FilterParameter)
    SIMPL_STATIC_NEW_MACRO(FloatVec4FilterParameter)
    SIMPL_TYPE_MACRO(FloatVec4FilterParameter)

    static Pointer New(const QString& humanLabel, const QString& propertyName,
    const FloatVec4_t& defaultValue, Category category,
     int groupIndex = -1);

    virtual ~FloatVec4FilterParameter();

  /**
   * @brief getWidgetType Returns the type of widget that displays and controls
   * this FilterParameter subclass
   * @return
   */
    QString getWidgetType() const override;

  protected:
    FloatVec4FilterParameter();

  private:
    FloatVec4FilterParameter(const FloatVec4FilterParameter&) = delete; // Copy Constructor Not Implemented
    void operator=(const FloatVec4FilterParameter&) = delete;           // Move assignment Not Implemented
};

