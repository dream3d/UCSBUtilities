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

#include "MonoclinicOpsMisoColor.h"

#include <stdexcept>

#include "EbsdLib/Core/EbsdMacros.h"
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MonoclinicOpsMisoColor::MonoclinicOpsMisoColor() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MonoclinicOpsMisoColor::~MonoclinicOpsMisoColor() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SIMPL::Rgb MonoclinicOpsMisoColor::generateMisorientationColor(const QuatD& q, const QuatD& refFrame) const
{
  throw EbsdLib::method_not_implemented("CubicLowOpsMisoColor::generateMisorientationColor is not implemented");
  return RgbColor::dRgb(0, 0, 0, 0);
}

// -----------------------------------------------------------------------------
MonoclinicOpsMisoColor::Pointer MonoclinicOpsMisoColor::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::string MonoclinicOpsMisoColor::getNameOfClass() const
{
  return std::string("MonoclinicOpsMisoColor");
}

// -----------------------------------------------------------------------------
QString MonoclinicOpsMisoColor::ClassName()
{
  return QString("MonoclinicOpsMisoColor");
}

// -----------------------------------------------------------------------------
MonoclinicOpsMisoColor::Pointer MonoclinicOpsMisoColor::New()
{
  Pointer sharedPtr(new(MonoclinicOpsMisoColor));
  return sharedPtr;
}
