


#ifndef _TrigonalLowOpsMisoColor_H_
#define _TrigonalLowOpsMisoColor_H_


#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Math/QuaternionMath.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/SpaceGroupOps/TrigonalLowOps.h"
#include "OrientationLib/OrientationMath/OrientationArray.hpp"


class TrigonalLowOpsMisoColor : public TrigonalLowOps
{
  public:
    SIMPL_SHARED_POINTERS(TrigonalLowOpsMisoColor)
    SIMPL_TYPE_MACRO_SUPER(TrigonalLowOpsMisoColor, TrigonalLowOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(TrigonalLowOpsMisoColor)

    TrigonalLowOpsMisoColor();
    virtual ~TrigonalLowOpsMisoColor();


    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet DREAM3D::Rgb
     */
    virtual DREAM3D::Rgb generateMisorientationColor(const QuatF& q, const QuatF& refFrame);



  protected:


  private:
    TrigonalLowOpsMisoColor(const TrigonalLowOpsMisoColor&); // Copy Constructor Not Implemented
    void operator=(const TrigonalLowOpsMisoColor&); // Operator '=' Not Implemented
};

#endif /* TrigonalLowOpsMisoColor_H_ */

