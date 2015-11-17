


#ifndef _MonoclinicOpsMisoColor_H_
#define _MonoclinicOpsMisoColor_H_


#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Math/QuaternionMath.hpp"

#include "OrientationLib/OrientationLib.h"
#include "OrientationLib/SpaceGroupOps/MonoclinicOps.h"


class MonoclinicOpsMisoColor : public MonoclinicOps
{
  public:
    SIMPL_SHARED_POINTERS(MonoclinicOpsMisoColor)
    SIMPL_TYPE_MACRO_SUPER(MonoclinicOpsMisoColor, MonoclinicOpsMisoColor)
    SIMPL_STATIC_NEW_MACRO(MonoclinicOpsMisoColor)

    MonoclinicOpsMisoColor();
    virtual ~MonoclinicOpsMisoColor();

    /**
     * @brief generateMisorientationColor Generates a color based on the method developed by C. Schuh and S. Patala.
     * @param q Quaternion representing the direction
     * @param refDir The sample reference direction
     * @return Returns the ARGB Quadruplet DREAM3D::Rgb
     */
    virtual DREAM3D::Rgb generateMisorientationColor(const QuatF& q, const QuatF& refFrame);


  protected:



  private:
    MonoclinicOpsMisoColor(const MonoclinicOpsMisoColor&); // Copy Constructor Not Implemented
    void operator=(const MonoclinicOpsMisoColor&); // Operator '=' Not Implemented
};

#endif /* MonoclinicOpsMisoColor_H_ */

