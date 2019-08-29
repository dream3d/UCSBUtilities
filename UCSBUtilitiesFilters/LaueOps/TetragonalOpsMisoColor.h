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

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/LaueOps/TetragonalOps.h"



class TetragonalOpsMisoColor : public TetragonalOps
{
  public:
    SIMPL_SHARED_POINTERS(TetragonalOpsMisoColor)
     SIMPL_TYPE_MACRO_SUPER_OVERRIDE(TetragonalOpsMisoColor, TetragonalOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(TetragonalOpsMisoColor)

    TetragonalOpsMisoColor();
    ~TetragonalOpsMisoColor() override;

    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet SIMPL::Rgb
     */
    SIMPL::Rgb generateMisorientationColor(const QuatType& q, const QuatType& refFrame) const override;

  public:
    TetragonalOpsMisoColor(const TetragonalOpsMisoColor&) = delete; // Copy Constructor Not Implemented
    TetragonalOpsMisoColor(TetragonalOpsMisoColor&&) = delete;      // Move Constructor Not Implemented
    TetragonalOpsMisoColor& operator=(const TetragonalOpsMisoColor&) = delete; // Copy Assignment Not Implemented
    TetragonalOpsMisoColor& operator=(TetragonalOpsMisoColor&&) = delete;      // Move Assignment Not Implemented
};


