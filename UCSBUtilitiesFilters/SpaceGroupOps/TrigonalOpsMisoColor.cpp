

#include "TrigonalOpsMisoColor.h"

// Include this FIRST because there is a needed define for some compiles
// to expose some of the constants needed below
#include "SIMPLib/Math/SIMPLibMath.h"
#include "SIMPLib/Utilities/ColorTable.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TrigonalOpsMisoColor::TrigonalOpsMisoColor() :
  TrigonalOps()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TrigonalOpsMisoColor::~TrigonalOpsMisoColor()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DREAM3D::Rgb TrigonalOpsMisoColor::generateMisorientationColor(const QuatF& q, const QuatF& refFrame)
{
  DREAM3D::Rgb rgb = RgbColor::dRgb(0, 0, 0, 0);

  BOOST_ASSERT(false);

  return rgb;
}

