


#ifndef _ORTHORHOMBICOpsMisoColor_H_
#define _ORTHORHOMBICOpsMisoColor_H_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Math/QuaternionMath.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/SpaceGroupOps/OrthoRhombicOps.h"



class OrthoRhombicOpsMisoColor : public OrthoRhombicOps
{
  public:
    SIMPL_SHARED_POINTERS(OrthoRhombicOpsMisoColor)
    SIMPL_TYPE_MACRO_SUPER(OrthoRhombicOpsMisoColor, OrthoRhombicOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(OrthoRhombicOpsMisoColor)

    OrthoRhombicOpsMisoColor();
    virtual ~OrthoRhombicOpsMisoColor();


    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet DREAM3D::Rgb
     */
    virtual DREAM3D::Rgb generateMisorientationColor(const QuatF& q, const QuatF& refFrame);


  protected:


  private:
    OrthoRhombicOpsMisoColor(const OrthoRhombicOpsMisoColor&); // Copy Constructor Not Implemented
    void operator=(const OrthoRhombicOpsMisoColor&); // Operator '=' Not Implemented
};

#endif /* ORTHORHOMBICOpsMisoColor_H_ */

