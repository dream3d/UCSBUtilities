


#ifndef _HexagonalLowOpsMisoColor_H_
#define _HexagonalLowOpsMisoColor_H_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Math/QuaternionMath.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/SpaceGroupOps/HexagonalLowOps.h"



class HexagonalLowOpsMisoColor : public HexagonalLowOps
{
  public:
    SIMPL_SHARED_POINTERS(HexagonalLowOpsMisoColor)
    SIMPL_TYPE_MACRO_SUPER(HexagonalLowOpsMisoColor, HexagonalLowOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(HexagonalLowOpsMisoColor)

    HexagonalLowOpsMisoColor();
    virtual ~HexagonalLowOpsMisoColor();


    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet DREAM3D::Rgb
     */
    virtual DREAM3D::Rgb generateMisorientationColor(const QuatF& q, const QuatF& refFrame);



  protected:


  private:
    HexagonalLowOpsMisoColor(const HexagonalLowOpsMisoColor&); // Copy Constructor Not Implemented
    void operator=(const HexagonalLowOpsMisoColor&); // Operator '=' Not Implemented
};

#endif /* HexagonalLowOpsMisoColor_H_ */

