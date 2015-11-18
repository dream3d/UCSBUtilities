

#ifndef _Symmetric6x6Widget_H_
#define _Symmetric6x6Widget_H_


#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtWidgets/QWidget>

#include "QtSupportLib/FaderWidget.h"


#include "SIMPLib/Common/AbstractFilter.h"

#include "DREAM3DWidgetsLib/DREAM3DWidgetsLib.h"
#include "DREAM3DWidgetsLib/FilterParameterWidgets/FilterParameterWidget.h"


#include "ui_Symmetric6x6Widget.h"

#include "UCSBUtilities/FilterParameters/Symmetric6x6FilterParameter.h"

/**
* @brief
* @author
* @version
*/
class DREAM3DWidgetsLib_EXPORT Symmetric6x6Widget : public FilterParameterWidget, private Ui::Symmetric6x6Widget
{
    Q_OBJECT

  public:
    /**
    * @brief Constructor
    * @param parameter The FilterParameter object that this widget represents
    * @param filter The instance of the filter that this parameter is a part of
    * @param parent The parent QWidget for this Widget
    */
    Symmetric6x6Widget(FilterParameter* parameter, AbstractFilter* filter = NULL, QWidget* parent = NULL);

    virtual ~Symmetric6x6Widget();

    /**
    * @brief This method does additional GUI widget connections
    */
    void setupGui();

  public slots:
    void widgetChanged(const QString& msg);
    void filterNeedsInputParameters(AbstractFilter* filter); // When the filter is ready for us to update its input parameter(s) that we are responsible for
    void beforePreflight(); // Called just before the "dataCheck()" is called
    void afterPreflight(); // Called just after the dataCheck() is called.

    void changeSym(int newState); //slot to connect combobox to symmetry state

    //slots for special symmetry relationships
    void vbar66(const QString& dummy);
    void tetragonalv16Changed(const QString& dummy);
    void tetragonalv26Changed(const QString& dummy);

    void trigonalv14Changed(const QString& dummy);
    void trigonalv24Changed(const QString& dummy);
    void trigonalv56Changed(const QString& dummy);

    void trigonalv15Changed(const QString& dummy);
    void trigonalv25Changed(const QString& dummy);
    void trigonalv46Changed(const QString& dummy);

  signals:
    void errorSettingFilterParameter(const QString& msg);
    void parametersChanged();

  private:

    bool m_DidCausePreflight;

    Symmetric6x6FilterParameter* m_FilterParameter;

    //functions for changing symmetry type
    void clearSym();
    void setCubicSym();
    void setHexagonalSym();
    void setTetHighSym();
    void setTetLowSym();
    void setTrigHighSym();
    void setTrigLowSym();
    void setOrthorhomicSym();
    void setMonoclinicSym();
    void setTriclinicSym();

    //helper functions for complex inter dependancies
    // void vbar(QLineEdit* obj);

    Symmetric6x6Widget(const Symmetric6x6Widget&); // Copy Constructor Not Implemented
    void operator=(const Symmetric6x6Widget&); // Operator '=' Not Implemented

};

#endif /* _Symmetric6x6Widget_H_ */

