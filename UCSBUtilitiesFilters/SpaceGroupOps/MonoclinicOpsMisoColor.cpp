

#include "MonoclinicOpsMisoColor.h"


// Include this FIRST because there is a needed define for some compiles
// to expose some of the constants needed below
#include "SIMPLib/Math/SIMPLibMath.h"
#include "SIMPLib/Utilities/ColorTable.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MonoclinicOpsMisoColor::MonoclinicOpsMisoColor() :
  MonoclinicOps()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MonoclinicOpsMisoColor::~MonoclinicOpsMisoColor()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DREAM3D::Rgb MonoclinicOpsMisoColor::generateMisorientationColor(const QuatF& q, const QuatF& refFrame)
{
  Q_ASSERT(false);
  return RgbColor::dRgb(0, 0, 0, 0);
}

