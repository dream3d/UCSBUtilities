


#ifndef _TetragonalLowOpsMisoColor_H_
#define _TetragonalLowOpsMisoColor_H_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Math/QuaternionMath.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/SpaceGroupOps/TetragonalLowOps.h"


class TetragonalLowOpsMisoColor : public TetragonalLowOps
{
  public:
    SIMPL_SHARED_POINTERS(TetragonalLowOpsMisoColor)
    SIMPL_TYPE_MACRO_SUPER(TetragonalLowOpsMisoColor, TetragonalLowOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(TetragonalLowOpsMisoColor)

    TetragonalLowOpsMisoColor();
    virtual ~TetragonalLowOpsMisoColor();


    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet DREAM3D::Rgb
     */
    virtual DREAM3D::Rgb generateMisorientationColor(const QuatF& q, const QuatF& refFrame);



  protected:


  private:
    TetragonalLowOpsMisoColor(const TetragonalLowOpsMisoColor&); // Copy Constructor Not Implemented
    void operator=(const TetragonalLowOpsMisoColor&); // Operator '=' Not Implemented
};

#endif /* TetragonalLowOpsMisoColor_H_ */

