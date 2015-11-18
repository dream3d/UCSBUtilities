


#ifndef _InputCrystalCompliances_H_
#define _InputCrystalCompliances_H_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/AbstractFilter.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "UCSBUtilities/FilterParameters/Symmetric6x6FilterParameter.h"

/**
 * @brief The InputCrystalCompliances class. See [Filter documentation](@ref inputcrystalcompliances) for details.
 */
class InputCrystalCompliances : public AbstractFilter
{
    Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */
  public:
    SIMPL_SHARED_POINTERS(InputCrystalCompliances)
    SIMPL_STATIC_NEW_MACRO(InputCrystalCompliances)
    SIMPL_TYPE_MACRO_SUPER(InputCrystalCompliances, AbstractFilter)

    virtual ~InputCrystalCompliances();

    SIMPL_FILTER_PARAMETER(FloatVec21_t, Compliances)
    Q_PROPERTY(FloatVec21_t Compliances READ getCompliances WRITE setCompliances)

    SIMPL_FILTER_PARAMETER(DataArrayPath, CrystalCompliancesArrayPath)
    Q_PROPERTY(DataArrayPath CrystalCompliancesArrayPath READ getCrystalCompliancesArrayPath WRITE setCrystalCompliancesArrayPath)

    /**
     * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getCompiledLibraryName();

    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters);

    /**
     * @brief getGroupName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getGroupName();

    /**
     * @brief getSubGroupName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getSubGroupName();

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    virtual const QString getHumanLabel();

    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters();

    /**
     * @brief writeFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual int writeFilterParameters(AbstractFilterParametersWriter* writer, int index);

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

    /**
     * @brief execute Reimplemented from @see AbstractFilter class
     */
    virtual void execute();

    /**
    * @brief preflight Reimplemented from @see AbstractFilter class
    */
    virtual void preflight();

  signals:
    /**
     * @brief updateFilterParameters Emitted when the Filter requests all the latest Filter parameters
     * be pushed from a user-facing control (such as a widget)
     * @param filter Filter instance pointer
     */
    void updateFilterParameters(AbstractFilter* filter);

    /**
     * @brief parametersChanged Emitted when any Filter parameter is changed internally
     */
    void parametersChanged();

    /**
     * @brief preflightAboutToExecute Emitted just before calling dataCheck()
     */
    void preflightAboutToExecute();

    /**
     * @brief preflightExecuted Emitted just after calling dataCheck()
     */
    void preflightExecuted();

  protected:
    InputCrystalCompliances();

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    void dataCheck();
  private:
    DEFINE_DATAARRAY_VARIABLE(float, CrystalCompliances)

    InputCrystalCompliances(const InputCrystalCompliances&); // Copy Constructor Not Implemented
    void operator=(const InputCrystalCompliances&); // Operator '=' Not Implemented
};

#endif /* InputCrystalCompliances_H_ */