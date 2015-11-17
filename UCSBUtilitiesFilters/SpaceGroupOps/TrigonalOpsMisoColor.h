


#ifndef _TrigonalOpsMisoColor_H_
#define _TrigonalOpsMisoColor_H_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Math/QuaternionMath.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/SpaceGroupOps/TrigonalOps.h"



class TrigonalOpsMisoColor : public TrigonalOps
{
  public:
    SIMPL_SHARED_POINTERS(TrigonalOpsMisoColor)
    SIMPL_TYPE_MACRO_SUPER(TrigonalOpsMisoColor, TrigonalOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(TrigonalOpsMisoColor)

    TrigonalOpsMisoColor();
    virtual ~TrigonalOpsMisoColor();

    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet DREAM3D::Rgb
     */
    virtual DREAM3D::Rgb generateMisorientationColor(const QuatF& q, const QuatF& refFrame);


  protected:


  private:
    TrigonalOpsMisoColor(const TrigonalOpsMisoColor&); // Copy Constructor Not Implemented
    void operator=(const TrigonalOpsMisoColor&); // Operator '=' Not Implemented
};

#endif /* TrigonalOpsMisoColor_H_ */

