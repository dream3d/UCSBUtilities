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
FloatVec4FilterParameter::Pointer FloatVec4FilterParameter::New(const QString& humanLabel, const QString& propertyName, const FloatVec4_t& defaultValue, Category category, int groupIndex)
{

  FloatVec4FilterParameter::Pointer ptr = FloatVec4FilterParameter::New();
  ptr->setHumanLabel(humanLabel);
  ptr->setPropertyName(propertyName);
  QVariant v;
  v.setValue(defaultValue);
  ptr->setDefaultValue(v);
  ptr->setCategory(category);
  ptr->setGroupIndex(groupIndex);

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
