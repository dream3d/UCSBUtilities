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

#include "OrientationLib/Core/Orientation.hpp"
#include "OrientationLib/LaueOps/TrigonalLowOps.h"
#include "OrientationLib/OrientationLib.h"

class TrigonalLowOpsMisoColor : public TrigonalLowOps
{
  public:
    SIMPL_SHARED_POINTERS(TrigonalLowOpsMisoColor)
     SIMPL_TYPE_MACRO_SUPER_OVERRIDE(TrigonalLowOpsMisoColor, TrigonalLowOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(TrigonalLowOpsMisoColor)

    TrigonalLowOpsMisoColor();
    ~TrigonalLowOpsMisoColor() override;

    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet SIMPL::Rgb
     */
    SIMPL::Rgb generateMisorientationColor(const QuatType& q, const QuatType& refFrame) const override;

  public:
    TrigonalLowOpsMisoColor(const TrigonalLowOpsMisoColor&) = delete; // Copy Constructor Not Implemented
    TrigonalLowOpsMisoColor(TrigonalLowOpsMisoColor&&) = delete;      // Move Constructor Not Implemented
    TrigonalLowOpsMisoColor& operator=(const TrigonalLowOpsMisoColor&) = delete; // Copy Assignment Not Implemented
    TrigonalLowOpsMisoColor& operator=(TrigonalLowOpsMisoColor&&) = delete;      // Move Assignment Not Implemented
};


