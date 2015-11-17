


#ifndef _TetragonalOpsMisoColor_H_
#define _TetragonalOpsMisoColor_H_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Math/QuaternionMath.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/SpaceGroupOps/TetragonalOps.h"



class TetragonalOpsMisoColor : public TetragonalOps
{
  public:
    SIMPL_SHARED_POINTERS(TetragonalOpsMisoColor)
    SIMPL_TYPE_MACRO_SUPER(TetragonalOpsMisoColor, TetragonalOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(TetragonalOpsMisoColor)

    TetragonalOpsMisoColor();
    virtual ~TetragonalOpsMisoColor();


    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet DREAM3D::Rgb
     */
    virtual DREAM3D::Rgb generateMisorientationColor(const QuatF& q, const QuatF& refFrame);

  protected:


  private:
    TetragonalOpsMisoColor(const TetragonalOpsMisoColor&); // Copy Constructor Not Implemented
    void operator=(const TetragonalOpsMisoColor&); // Operator '=' Not Implemented
};

#endif /* TetragonalOpsMisoColor_H_ */

