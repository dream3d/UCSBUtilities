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

#include "TetragonalLowOpsMisoColor.h"

#include <stdexcept>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TetragonalLowOpsMisoColor::TetragonalLowOpsMisoColor() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TetragonalLowOpsMisoColor::~TetragonalLowOpsMisoColor() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SIMPL::Rgb TetragonalLowOpsMisoColor::generateMisorientationColor(const QuatD& q, const QuatD& refFrame) const
{
  throw std::runtime_error("TetragonalLowOpsMisoColor::generateMisorientationColor is not implemented");

  return RgbColor::dRgb(0, 0, 0, 0);
}

// -----------------------------------------------------------------------------
TetragonalLowOpsMisoColor::Pointer TetragonalLowOpsMisoColor::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::string TetragonalLowOpsMisoColor::getNameOfClass() const
{
  return std::string("TetragonalLowOpsMisoColor");
}

// -----------------------------------------------------------------------------
QString TetragonalLowOpsMisoColor::ClassName()
{
  return QString("TetragonalLowOpsMisoColor");
}

// -----------------------------------------------------------------------------
TetragonalLowOpsMisoColor::Pointer TetragonalLowOpsMisoColor::New()
{
  Pointer sharedPtr(new(TetragonalLowOpsMisoColor));
  return sharedPtr;
}
