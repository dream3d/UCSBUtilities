


#ifndef _HEXAGONALOpsMisoColor_H_
#define _HEXAGONALOpsMisoColor_H_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Math/QuaternionMath.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/SpaceGroupOps/HexagonalOps.h"


class HexagonalOpsMisoColor : public HexagonalOps
{
  public:
    SIMPL_SHARED_POINTERS(HexagonalOpsMisoColor)
    SIMPL_TYPE_MACRO_SUPER(HexagonalOpsMisoColor, HexagonalOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(HexagonalOpsMisoColor)

    HexagonalOpsMisoColor();
    virtual ~HexagonalOpsMisoColor();


    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet DREAM3D::Rgb
     */
    virtual DREAM3D::Rgb generateMisorientationColor(const QuatF& q, const QuatF& refFrame);

  protected:

  private:
    HexagonalOpsMisoColor(const HexagonalOpsMisoColor&); // Copy Constructor Not Implemented
    void operator=(const HexagonalOpsMisoColor&); // Operator '=' Not Implemented
};

#endif /* HEXAGONALOpsMisoColor_H_ */

