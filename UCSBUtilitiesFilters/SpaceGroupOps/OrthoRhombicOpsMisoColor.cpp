


#include "OrthoRhombicOpsMisoColor.h"

// Include this FIRST because there is a needed define for some compiles
// to expose some of the constants needed below
#include "SIMPLib/Math/SIMPLibMath.h"
#include "SIMPLib/Utilities/ColorTable.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
OrthoRhombicOpsMisoColor::OrthoRhombicOpsMisoColor() :
  OrthoRhombicOps()
{
  // TODO Auto-generated constructor stub
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
OrthoRhombicOpsMisoColor::~OrthoRhombicOpsMisoColor()
{
  // TODO Auto-generated destructor stub
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DREAM3D::Rgb OrthoRhombicOpsMisoColor::generateMisorientationColor(const QuatF& q, const QuatF& refFrame)
{
  BOOST_ASSERT(false);

  float n1, n2, n3, w;
  float x, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11;
  float y, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10, y11;
  float z, z1, z2, z3, z4, z5, z6, z7, z8, z9, z10, z11;
  float k, h, s, v, c, r, g, b;

  QuatF q1, q2;
  QuaternionMathF::Copy(q, q1);
  QuaternionMathF::Copy(refFrame, q2);

  //get disorientation
  w = getMisoQuat(q1, q2, n1, n2, n3);
  n1 = fabs(n1);
  n2 = fabs(n2);
  n3 = fabs(n3);

  //eq c1.1
  k = tan(w / 2.0f);
  x = n1;
  y = n2;
  z = n3;

  FOrientArrayType rod(x, y, z, k);
  rod = getMDFFZRod(rod);
  x = rod[0];
  y = rod[1];
  z = rod[2];
  k = rod[3];

  //eq c1.2
  k = std::max(x, y);
  k = std::max(k, z);
  k = (k * sqrt(3.0f)) / (x + y + z);
  x1 = x * k;
  y1 = y * k;
  z1 = z * k;

  //eq c1.3
  //3 rotation matricies (in paper) can be multiplied into one (here) for simplicity / speed
  //g1*g2*g3 = {{sqrt(2/3), 0, 1/sqrt(3)},{-1/sqrt(6), 1/sqrt(2), 1/sqrt(3)},{-1/sqrt(6), 1/sqrt(2), 1/sqrt(3)}}
  x2 = x1 * sqrt(2.0f / 3.0f) - (y1 + z1) / sqrt(6.0f);
  y2 = (y1 - z1) / sqrt(2.0f);
  z2 = (x1 + y1 + z1) / sqrt(3.0f);

  //eq c1.4
  k = fmodf(atan2f(y2, x2) + 2.0f * SIMPLib::Constants::k_Pi, 2.0f * SIMPLib::Constants::k_Pi);
  x3 = cos(k) * sqrt((x2 * x2 + y2 * y2) / 2.0) * sin(SIMPLib::Constants::k_Pi / 6.0 + fmodf(k, 2.0f * SIMPLib::Constants::k_Pi / 3.0f)) / 0.5f;
  y3 = sin(k) * sqrt((x2 * x2 + y2 * y2) / 2.0) * sin(SIMPLib::Constants::k_Pi / 6.0 + fmodf(k, 2.0f * SIMPLib::Constants::k_Pi / 3.0f)) / 0.5f;
  z3 = z2 - 1.0f;

  //eq c1.5
  k = (sqrt(x3 * x3 + y3 * y3) - z3) / sqrt(x3 * x3 + y3 * y3 + z3 * z3);
  x4 = x3 * k;
  y4 = y3 * k;
  z4 = z3 * k;

  //eq c1.6, 7, and 8 (from matlab code not paper)
  k = fmod(atan2(y4, x4) + 2 * M_PI, 2 * M_PI);

  int type;
  if(k >= 0.0f && k < 2.0f * M_PI / 3.0f)
  {
    type = 1;
    x5 = (x4 + y4 * sqrt(3.0f)) / 2.0f;
    y5 = (-x4 * sqrt(3.0f) + y4) / 2.0f;
  }
  else if(k >= 2.0f * M_PI / 3.0f && k < 4.0f * M_PI / 3.0f)
  {
    type = 2;
    x5 = x4;
    y5 = y4;
  }
  else//k>=4*pi/3 && <2*pi
  {
    type = 3;
    x5 = (x4 - y4 * sqrt(3.0f)) / 2.0f;
    y5 = (x4 * sqrt(3.0f) + y4) / 2.0f;
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
    x9 = (x8 - y8 * sqrt(3.0f)) / 2.0f;
    y9 = (x8 * sqrt(3.0f) + y8) / 2.0f;
  }
  else if(type == 2)
  {
    x9 = x8;
    y9 = y8;
  }
  else//type==3;
  {
    x9 = (x8 + y8 * sqrt(3.0f)) / 2.0f;
    y9 = (-x8 * sqrt(3.0f) + y8) / 2.0f;
  }
  z9 = z8;

  //c1.9
  x10 = (x9 - y9 * sqrt(3.0f)) / 2.0f;
  y10 = (x9 * sqrt(3.0f) + y9) / 2.0f;
  z10 = z9;

  //cartesian to traditional hsv
  x11 = sqrt(x10 * x10 + y10 * y10 + z10 * z10); //r
  y11 = acos(z10 / x11) / M_PI; //theta
  z11 = fmod(fmod(atan2(y10, x10) + 2.0f * M_PI, 2.0f * M_PI) + 4.0f * M_PI / 3.0f, 2.0f * M_PI) / (2.0f * M_PI); //rho

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

  //hsv to rgb (from wikipedia hsv/hsl page)
  c = v * s;
  k = c * (1 - fabs(fmod(h * 6, 2) - 1)); //x in wiki article
  h = h * 6;
  r = 0;
  g = 0;
  b = 0;

  if(h >= 0)
  {
    if(h < 1)
    {
      r = c;
      g = k;
    }
    else if(h < 2)
    {
      r = k;
      g = c;
    }
    else if(h < 3)
    {
      g = c;
      b = k;
    }
    else if(h < 4)
    {
      g = k;
      b = c;
    }
    else if (h < 5)
    {
      r = k;
      b = c;
    }
    else if(h < 6)
    {
      r = c;
      b = k;
    }
  }

  //adjust lumosity and invert
  r = 1 - (r + (v - c));
  g = 1 - (g + (v - c));
  b = 1 - (b + (v - c));

  DREAM3D::Rgb rgb = RgbColor::dRgb(r * 255, g * 255, b * 255, 0);

  return rgb;
}

