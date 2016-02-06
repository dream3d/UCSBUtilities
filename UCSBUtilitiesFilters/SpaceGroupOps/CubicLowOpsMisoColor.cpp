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

#include "CubicLowOpsMisoColor.h"

#include <boost/assert.hpp>

// Include this FIRST because there is a needed define for some compiles
// to expose some of the constants needed below
#include "SIMPLib/Math/SIMPLibMath.h"
#include "SIMPLib/Utilities/ColorTable.h"




// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CubicLowOpsMisoColor::CubicLowOpsMisoColor() :
  CubicLowOps()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CubicLowOpsMisoColor::~CubicLowOpsMisoColor()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SIMPL::Rgb CubicLowOpsMisoColor::generateMisorientationColor(const QuatF& q, const QuatF& refFrame)
{
  BOOST_ASSERT(false);
#if 0
  float n1, n2, n3, w;
  float x, x1, x2, x3, x4;
  float y, y1, y2, y3, y4;
  float z, z1, z2, z3, z4;
  float k, h, s, v, c, r, g, b;

  //get misorientation as rodriguez vector in FZ (eq. c7.1)
  QuatF q1, q2;
  QuaternionMathF::Copy(q, q1);
  QuaternionMathF::Copy(refFrame, q2);
  w = getMisoQuat(q1, q2, n1, n2, n3);
  FOrientArrayType rod(n1, n2, n3, tan(w / 2.0f));
  rod = getMDFFZRod(rod);
  x = rod[0] * rod[3];
  y = rod[1] * rod[3];
  z = rod[2] * rod[3];

  //eq c7.2
  if(atan2(y, x) <= SIMPLib::Constants::k_PiOver4){
    x1 = z;
    y1 = x;
    z1 = y;
  } else {
    x1 = y;
    y1 = z;
    z1 = x;
  }

  //eq c7.3
  //3 rotation matricies (in paper) can be multiplied into one (here) for simplicity / speed
  //g1*g2*g3 = {{sqrt(2/3), 0, 1/sqrt(3)},{-1/sqrt(6), 1/sqrt(2), 1/sqrt(3)},{-1/sqrt(6), 1/sqrt(2), 1/sqrt(3)}}
  x2 = x1 * (SIMPLib::Constants::k_Sqrt2 / SIMPLib::Constants::k_Sqrt3) - (y1 + z1) / (SIMPLib::Constants::k_Sqrt2 * SIMPLib::Constants::k_Sqrt3);
  y2 = (y1 - z1) / SIMPLib::Constants::k_Sqrt2;
  z2 = (x1 + y1 + z1) / SIMPLib::Constants::k_Sqrt3;

  //eq c7.4
  k = (SIMPLib::Constants::k_Sqrt3 * y2 + x2) / (2.0f * pow(x2 * x2 + y2 * y2, 1.5f));
  x3 = x2 * (x2 + SIMPLib::Constants::k_Sqrt3 * y2) * (x2 - SIMPLib::Constants::k_Sqrt3 * y2) * k;
  y3 = y2 * (y2 + SIMPLib::Constants::k_Sqrt3 * x2) * (SIMPLib::Constants::k_Sqrt3 * x2 - y2) * k;
  z3 = z2 * SIMPLib::Constants::k_Sqrt3;

  //eq c7.5 these hsv are from 0 to 1 in cartesian coordinates
  x4 = -x3;
  y4 = -y3;
  z4 = z3;

  //convert to traditional hsv (0-1)
  h = atan2(y7, x7);
  if(h < 0.0f) {h += SIMPLib::Constants::k_2Pi;}
  h /= SIMPLib::Constants::k_2Pi;
  s = sqrt(x7 * x7 + y7 * y7);
  v = z7;
  if(v > 0.0f) {s = s / v;}

  //convert to rgb and invert
  SIMPL::Rgb rgb = ColorUtilities::convertHSVtoRgb(h, s, v);
#endif
  SIMPL::Rgb rgb = 0xFFFFFFFF;
  return RgbColor::dRgb(255 - RgbColor::dRed(rgb), 255 - RgbColor::dGreen(rgb), 255 - RgbColor::dBlue(rgb), 0);
}
