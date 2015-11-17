


#ifndef _CubicLowOpsMisoColor_H_
#define _CubicLowOpsMisoColor_H_


#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Math/QuaternionMath.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/SpaceGroupOps/CubicLowOps.h"



class CubicLowOpsMisoColor : public CubicLowOps
{
  public:
    SIMPL_SHARED_POINTERS(CubicLowOpsMisoColor)
    SIMPL_TYPE_MACRO_SUPER(CubicLowOpsMisoColor, CubicLowOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(CubicLowOpsMisoColor)

    CubicLowOpsMisoColor();
    virtual ~CubicLowOpsMisoColor();


    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet DREAM3D::Rgb
     */
    virtual DREAM3D::Rgb generateMisorientationColor(const QuatF& q, const QuatF& refFrame);


  protected:
    float _calcMisoQuat(const QuatF quatsym[24], int numsym,
                        QuatF& q1, QuatF& q2,
                        float& n1, float& n2, float& n3);
  private:
    CubicLowOpsMisoColor(const CubicLowOpsMisoColor&); // Copy Constructor Not Implemented
    void operator=(const CubicLowOpsMisoColor&); // Operator '=' Not Implemented
};

#endif /* CubicLowOpsMisoColor_H_ */

