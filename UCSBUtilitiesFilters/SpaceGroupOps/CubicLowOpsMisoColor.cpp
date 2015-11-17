

#include "CubicLowOpsMisoColor.h"

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
DREAM3D::Rgb CubicLowOpsMisoColor::generateMisorientationColor(const QuatF& q, const QuatF& refFrame)
{
  BOOST_ASSERT(false);

  float n1, n2, n3, w;
  float x, x1, x2, x3, x4;
  float y, y1, y2, y3, y4;
  float z, z1, z2, z3, z4;
  float k, h, s, v, c, r, g, b;

  QuatF q1, q2;
  QuaternionMathF::Copy(q, q1);
  QuaternionMathF::Copy(refFrame, q2);

  w = getMisoQuat(q1, q2, n1, n2, n3);

  //eq c7.1
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

  //eq c7.2
  k = atan2(y, x);
  if(k < M_PI_4)
  {
    x1 = z;
    y1 = x;
    z1 = y;
  }
  else
  {
    x1 = y;
    y1 = z;
    z1 = x;
  }

  //eq c7.3
  //3 rotation matricies (in paper) can be multiplied into one (here) for simplicity / speed
  //g1*g2*g3 = {{sqrt(2/3), 0, 1/sqrt(3)},{-1/sqrt(6), 1/sqrt(2), 1/sqrt(3)},{-1/sqrt(6), 1/sqrt(2), 1/sqrt(3)}}
  x2 = x1 * sqrt(2.0f / 3.0f) - (y1 + z1) / sqrt(6.0f);
  y2 = (y1 - z1) / sqrt(2.0f);
  z2 = (x1 + y1 + z1) / sqrt(3.0f);

  //eq c7.4
  k = (sqrt(3.0f) * y2 + x2) / (2.0f * pow(x2 * x2 + y2 * y2, 1.5f));
  x3 = x2 * (x2 + sqrt(3.0f) * y2) * (x2 - sqrt(3.0f) * y2) * k;
  y3 = y2 * (y2 + sqrt(3.0f) * x2) * (sqrt(3.0f) * x2 - y2) * k;
  z3 = z2 * sqrt(3.0f);

  //eq c7.5 these hsv are from 0 to 1 in cartesian coordinates
  x4 = -x3;
  y4 = -y3;
  z4 = z3;

  //convert to traditional hsv (0-1)
  h = fmod(atan2f(y4, x4) + 2.0f * M_PI, 2.0f * M_PI) / (2.0f * M_PI);
  s = sqrt(x4 * x4 + y4 * y4);
  v = z4;
  if(v > 0)
  {
    s = s / v;
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
  r = (r + (v - c));
  g = (g + (v - c));
  b = (b + (v - c));

  DREAM3D::Rgb rgb = RgbColor::dRgb(r * 255, g * 255, b * 255, 0);

  return rgb;
}



