


#ifndef _CUBICOpsMisoColor_H_
#define _CUBICOpsMisoColor_H_


#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Math/QuaternionMath.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/SpaceGroupOps/CubicOps.h"


class CubicOpsMisoColor : public CubicOps
{
  public:
    SIMPL_SHARED_POINTERS(CubicOpsMisoColor)
    SIMPL_TYPE_MACRO_SUPER(CubicOpsMisoColor, CubicOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(CubicOpsMisoColor)

    CubicOpsMisoColor();
    virtual ~CubicOpsMisoColor();

    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet DREAM3D::Rgb
     */
    virtual DREAM3D::Rgb generateMisorientationColor(const QuatF& q, const QuatF& refFrame);


  private:
    CubicOpsMisoColor(const CubicOpsMisoColor&); // Copy Constructor Not Implemented
    void operator=(const CubicOpsMisoColor&); // Operator '=' Not Implemented

};

#endif /* CUBICOpsMisoColor_H_ */

