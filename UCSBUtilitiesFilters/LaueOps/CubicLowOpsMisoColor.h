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

#ifndef _cubiclowopsmisocolor_h_
#define _cubiclowopsmisocolor_h_


#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Math/QuaternionMath.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/LaueOps/CubicLowOps.h"



class CubicLowOpsMisoColor : public CubicLowOps
{
  public:
    SIMPL_SHARED_POINTERS(CubicLowOpsMisoColor)
    SIMPL_TYPE_MACRO_SUPER(CubicLowOpsMisoColor, CubicLowOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(CubicLowOpsMisoColor)

    CubicLowOpsMisoColor();
    virtual ~CubicLowOpsMisoColor();


    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet SIMPL::Rgb
     */
    virtual SIMPL::Rgb generateMisorientationColor(const QuatF& q, const QuatF& refFrame);


  protected:
    float _calcMisoQuat(const QuatF quatsym[24], int numsym,
                        QuatF& q1, QuatF& q2,
                        float& n1, float& n2, float& n3);
  private:
    CubicLowOpsMisoColor(const CubicLowOpsMisoColor&); // Copy Constructor Not Implemented
    void operator=(const CubicLowOpsMisoColor&); // Operator '=' Not Implemented
};

#endif /* CubicLowOpsMisoColor_H_ */

