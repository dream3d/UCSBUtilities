

#include "HexagonalOpsMisoColor.h"

// Include this FIRST because there is a needed define for some compiles
// to expose some of the constants needed below
#include "SIMPLib/Math/SIMPLibMath.h"
#include "SIMPLib/Utilities/ColorUtilities.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HexagonalOpsMisoColor::HexagonalOpsMisoColor() :
  HexagonalOps()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HexagonalOpsMisoColor::~HexagonalOpsMisoColor()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DREAM3D::Rgb HexagonalOpsMisoColor::generateMisorientationColor(const QuatF& q, const QuatF& refFrame)
{
  float n1, n2, n3, w;
  float xo, xo1, xo2, xo3, x, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11;
  float yo, yo1, yo2, yo3, y, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10, y11;
  float zo, zo1, zo2, zo3, z, z1, z2, z3, z4, z5, z6, z7, z8, z9, z10, z11;
  float k, h, s, v;

  QuatF q1, q2;
  QuaternionMathF::Copy(q, q1);
  QuaternionMathF::Copy(refFrame, q2);

  //get misorientation
  w = getMisoQuat(q1, q2, n1, n2, n3);

  //eq c5.1
  k = tan(w / 2.0f);
  xo = n1;
  yo = n2;
  zo = n3;

  FOrientArrayType rod(xo, yo, zo, k);
  rod = getMDFFZRod(rod);
  xo = rod[0];
  yo = rod[1];
  zo = rod[2];
  k = rod[3];

  //eq c5.2

  if(k <= M_PI / 12.0f)
  {
    k = sqrtf(xo * xo + yo * yo);
    if(k > 0)
    {
      k = xo / k;
    }
    else
    {
      k = xo;
    }
  }
  else
  {
    k = sqrtf(xo * xo + yo * yo);
    if(k > 0)
    {
      k = (SIMPLib::Constants::k_Sqrt3 * xo + yo) / (2.0f * k);
    }
    else
    {
      k = (SIMPLib::Constants::k_Sqrt3 * xo + yo) / 2.0f;
    }
  }
  xo1 = xo * k;
  yo1 = yo * k;
  zo1 = zo / (2.0f - SIMPLib::Constants::k_Sqrt3);

  //eq c5.3
  k = 3.0f * atan2(yo1, xo1);
  xo2 = sqrtf(xo1 * xo1 + yo1 * yo1) * cos(k);
  yo2 = sqrtf(xo1 * xo1 + yo1 * yo1) * sin(k);
  zo2 = zo1;

  //eq c5.4
  k = std::max(xo2, yo2);
  if(fabs(k) > 0)
  {
    k = sqrtf(xo2 * xo2 + yo2 * yo2) / k;
  }
  else
  {
    k = sqrtf(xo2 * xo2 + yo2 * yo2);
  }
  xo3 = xo2 * k;
  yo3 = yo2 * k;
  zo3 = zo2;

  //substitute c5.4 results into c1.1
  x = xo3;
  y = yo3;
  z = zo3;

  //eq c1.2
  k = std::max(x, y);
  k = std::max(k, z);
  k = (k * SIMPLib::Constants::k_Sqrt3) / (x + y + z);
  x1 = x * k;
  y1 = y * k;
  z1 = z * k;

  //eq c1.3
  //3 rotation matricies (in paper) can be multiplied into one (here) for simplicity / speed
  //g1*g2*g3 = {{sqrt(2/3), -1/sqrt(6), -1/sqrt(6)},{0, 1/sqrt(2), -1/sqrt(2)},{1/sqrt(3), 1/sqrt(3), 1/sqrt(3)}}
  x2 = x1 * (SIMPLib::Constants::k_Sqrt2 / SIMPLib::Constants::k_Sqrt3) - (y1 + z1) / (SIMPLib::Constants::k_Sqrt2 * SIMPLib::Constants::k_Sqrt3);
  y2 = (y1 - z1) / SIMPLib::Constants::k_Sqrt2;
  z2 = (x1 + y1 + z1) / SIMPLib::Constants::k_Sqrt3;

  //eq c1.4
  k = fmodf(atan2f(y2, x2) + SIMPLib::Constants::k_2Pi, SIMPLib::Constants::k_2Pi);
  x3 = cos(k) * sqrt(x2 * x2 + y2 * y2) * sin(SIMPLib::Constants::k_Pi / 6.0f + fmodf(k, SIMPLib::Constants::k_2Pi / 3.0f)) / SIMPLib::Constants::k_HalfSqrt2;
  y3 = sin(k) * sqrt(x2 * x2 + y2 * y2) * sin(SIMPLib::Constants::k_Pi / 6.0f + fmodf(k, SIMPLib::Constants::k_2Pi / 3.0f)) / SIMPLib::Constants::k_HalfSqrt2;
  z3 = z2 - 1.0f;

  //eq c1.5
  k = sqrt(x3 * x3 + y3 * y3 + z3 * z3);
  if(k > 0)
  {
    k = (sqrt(x3 * x3 + y3 * y3) - z3) / k;
  }
  else
  {
    k = (sqrt(x3 * x3 + y3 * y3) - z3);
  }
  x4 = x3 * k;
  y4 = y3 * k;
  z4 = z3 * k;

  //eq c1.6, 7, and 8 (from matlab code not paper)
  k = fmod(atan2(y4, x4) + 2 * M_PI, 2 * M_PI);

  int type;
  if(k >= 0.0f && k < M_2PI / 3.0f)
  {
    type = 1;
    x5 = (x4 + y4 * SIMPLib::Constants::k_Sqrt3) / 2.0f;
    y5 = (-x4 * SIMPLib::Constants::k_Sqrt3 + y4) / 2.0f;
  }
  else if(k >= M_2PI / 3.0f && k < 4.0f * M_PI / 3.0f)
  {
    type = 2;
    x5 = x4;
    y5 = y4;
  }
  else//k>=4*pi/3 && <2*pi
  {
    type = 3;
    x5 = (x4 - y4 * SIMPLib::Constants::k_Sqrt3) / 2.0f;
    y5 = (x4 * SIMPLib::Constants::k_Sqrt3 + y4) / 2.0f;
  }
  z5 = z4;

  k = 1.5f * atan2(y5, x5);
  x6 = sqrt(x5 * x5 + y5 * y5) * cos(k);
  y6 = sqrt(x5 * x5 + y5 * y5) * sin(k);
  z6 = z5;

  k = 2.0f * atan2(x6, -z6);
  x7 = sqrt(x6 * x6 + z6 * z6) * sin(k);
  y7 = y6;
  z7 = -sqrt(x6 * x6 + z6 * z6) * cos(k);

  k = (2.0f / 3.0f) * atan2(y7, x7);
  x8 = sqrt(x7 * x7 + y7 * y7) * cos(k);
  y8 = sqrt(x7 * x7 + y7 * y7) * sin(k);
  z8 = z7;

  if(type == 1)
  {
    x9 = (x8 - y8 * SIMPLib::Constants::k_Sqrt3) / 2.0f;
    y9 = (x8 * SIMPLib::Constants::k_Sqrt3 + y8) / 2.0f;
  }
  else if(type == 2)
  {
    x9 = x8;
    y9 = y8;
  }
  else//type==3;
  {
    x9 = (x8 + y8 * SIMPLib::Constants::k_Sqrt3) / 2.0f;
    y9 = (-x8 * SIMPLib::Constants::k_Sqrt3 + y8) / 2.0f;
  }
  z9 = z8;

  //c1.9
  x10 = (x9 - y9 * SIMPLib::Constants::k_Sqrt3) / 2.0f;
  y10 = (x9 * SIMPLib::Constants::k_Sqrt3 + y9) / 2.0f;
  z10 = z9;

  //cartesian to traditional hsv
  x11 = sqrt(x10 * x10 + y10 * y10 + z10 * z10); //r
  y11 = acos(z10 / x11) / M_PI; //theta
  z11 = fmod(fmod(atan2(y10, x10) + M_2PI, M_2PI) + 4.0f * M_PI / 3.0f, M_2PI) / (M_2PI); //rho

  if(x11 == 0)
  {
    y11 = 0;
    z11 = 0;
  }

  h = z11;
  if(y11 >= 0.5f)
  {
    s = (1.0f - x11);
    v = 2.0f * x11 * (1.0f - y11) + (1.0f - x11) / 2.0f;
    if(v > 0)
    {
      s = s / (2.0f * v);
    }
    s = 1.0f - s;
  }
  else
  {
    s = (4.0f * x11 * y11) / (1.0f + x11);
    v = 0.5f + x11 / 2;
  }

  DREAM3D::Rgb rgb = ColorUtilities::convertHSVtoRgb(h, s, v);

  //now standard 0-255 rgb, needs inversion
  return RgbColor::dRgb(255 - RgbColor::dRed(rgb), 255 - RgbColor::dGreen(rgb), 255 - RgbColor::dBlue(rgb), 0);
}
