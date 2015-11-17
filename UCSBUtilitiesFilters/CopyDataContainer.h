


#ifndef _CopyDataContainer_H_
#define _CopyDataContainer_H_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/AbstractFilter.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The CopyDataContainer class. See [Filter documentation](@ref copydatacontainer) for details.
 */
class SIMPLib_EXPORT CopyDataContainer : public AbstractFilter
{
    Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */
  public:
    SIMPL_SHARED_POINTERS(CopyDataContainer)
    SIMPL_STATIC_NEW_MACRO(CopyDataContainer)
    SIMPL_TYPE_MACRO_SUPER(CopyDataContainer, AbstractFilter)

    virtual ~CopyDataContainer();

    SIMPL_FILTER_PARAMETER(QString, SelectedDataContainerName)
    Q_PROPERTY(QString SelectedDataContainerName READ getSelectedDataContainerName WRITE setSelectedDataContainerName)

    SIMPL_FILTER_PARAMETER(QString, NewDataContainerName)
    Q_PROPERTY(QString NewDataContainerName READ getNewDataContainerName WRITE setNewDataContainerName)

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
    CopyDataContainer();

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    void dataCheck();

  private:

    CopyDataContainer(const CopyDataContainer&); // Copy Constructor Not Implemented
    void operator=(const CopyDataContainer&); // Operator '=' Not Implemented
};

#endif /* _CopyDataContainer_H_ */
