


#ifndef _CopyAttributeMatrix_H_
#define _CopyAttributeMatrix_H_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/AbstractFilter.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The CopyAttributeMatrix class. See [Filter documentation](@ref copyattributematrix) for details.
 */
class SIMPLib_EXPORT CopyAttributeMatrix : public AbstractFilter
{
    Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */
  public:
    SIMPL_SHARED_POINTERS(CopyAttributeMatrix)
    SIMPL_STATIC_NEW_MACRO(CopyAttributeMatrix)
    SIMPL_TYPE_MACRO_SUPER(CopyAttributeMatrix, AbstractFilter)

    virtual ~CopyAttributeMatrix();

    SIMPL_FILTER_PARAMETER(DataArrayPath, SelectedAttributeMatrixPath)
    Q_PROPERTY(DataArrayPath SelectedAttributeMatrixPath READ getSelectedAttributeMatrixPath WRITE setSelectedAttributeMatrixPath)

    SIMPL_FILTER_PARAMETER(QString, NewAttributeMatrix)
    Q_PROPERTY(QString NewAttributeMatrix READ getNewAttributeMatrix WRITE setNewAttributeMatrix)

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
    CopyAttributeMatrix();

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    void dataCheck();

  private:

    CopyAttributeMatrix(const CopyAttributeMatrix&); // Copy Constructor Not Implemented
    void operator=(const CopyAttributeMatrix&); // Operator '=' Not Implemented
};

#endif /* _CopyAttributeMatrix_H_ */
