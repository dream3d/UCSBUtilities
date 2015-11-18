

#include "HexagonalLowOpsMisoColor.h"


// Include this FIRST because there is a needed define for some compiles
// to expose some of the constants needed below
#include "SIMPLib/Math/SIMPLibMath.h"
#include "SIMPLib/Utilities/ColorTable.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HexagonalLowOpsMisoColor::HexagonalLowOpsMisoColor() :
  HexagonalLowOps()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HexagonalLowOpsMisoColor::~HexagonalLowOpsMisoColor()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DREAM3D::Rgb HexagonalLowOpsMisoColor::generateMisorientationColor(const QuatF& q, const QuatF& refFrame)
{
  DREAM3D::Rgb rgb = RgbColor::dRgb(0, 0, 0, 0);

  Q_ASSERT(false);

  return rgb;
}

