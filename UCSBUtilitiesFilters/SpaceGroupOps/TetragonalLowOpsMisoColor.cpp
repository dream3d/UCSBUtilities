


#include "TetragonalLowOpsMisoColor.h"

// Include this FIRST because there is a needed define for some compiles
// to expose some of the constants needed below
#include "SIMPLib/Math/SIMPLibMath.h"
#include "SIMPLib/Utilities/ColorTable.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TetragonalLowOpsMisoColor::TetragonalLowOpsMisoColor() :
  TetragonalLowOps()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TetragonalLowOpsMisoColor::~TetragonalLowOpsMisoColor()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DREAM3D::Rgb TetragonalLowOpsMisoColor::generateMisorientationColor(const QuatF& q, const QuatF& refFrame)
{
  Q_ASSERT(false);

  return RgbColor::dRgb(0, 0, 0, 0);
}

