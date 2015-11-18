


#ifndef _TriclinicOpsMisoColor_H_
#define _TriclinicOpsMisoColor_H_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Math/QuaternionMath.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/SpaceGroupOps/TriclinicOps.h"



class TriclinicOpsMisoColor : public TriclinicOps
{
  public:
    SIMPL_SHARED_POINTERS(TriclinicOpsMisoColor)
    SIMPL_TYPE_MACRO_SUPER(TriclinicOpsMisoColor, TriclinicOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(TriclinicOpsMisoColor)

    TriclinicOpsMisoColor();
    virtual ~TriclinicOpsMisoColor();

    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet DREAM3D::Rgb
     */
    virtual DREAM3D::Rgb generateMisorientationColor(const QuatF& q, const QuatF& refFrame);



  protected:

  private:
    TriclinicOpsMisoColor(const TriclinicOpsMisoColor&); // Copy Constructor Not Implemented
    void operator=(const TriclinicOpsMisoColor&); // Operator '=' Not Implemented
};

#endif /* TriclinicOpsMisoColor_H_ */
