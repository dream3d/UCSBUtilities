

#ifndef _FloatVec4Widget_H_
#define _FloatVec4Widget_H_


#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtWidgets/QWidget>

#include "QtSupportLib/FaderWidget.h"


#include "SIMPLib/Common/AbstractFilter.h"

#include "DREAM3DWidgetsLib/DREAM3DWidgetsLib.h"
#include "DREAM3DWidgetsLib/FilterParameterWidgets/FilterParameterWidget.h"

#include "UCSBUtilities/FilterParameters/FloatVec4FilterParameter.h"


#include "ui_FloatVec4Widget.h"

#include "UCSBUtilities/FilterParameters/FloatVec4FilterParameter.h"

/**
* @brief
* @author
* @version
*/
class DREAM3DWidgetsLib_EXPORT FloatVec4Widget : public FilterParameterWidget, private Ui::FloatVec4Widget
{
    Q_OBJECT

  public:
    /**
    * @brief Constructor
    * @param parameter The FilterParameter object that this widget represents
    * @param filter The instance of the filter that this parameter is a part of
    * @param parent The parent QWidget for this Widget
    */
    FloatVec4Widget(FilterParameter* parameter, AbstractFilter* filter = NULL, QWidget* parent = NULL);

    virtual ~FloatVec4Widget();

    /**
    * @brief This method does additional GUI widget connections
    */
    void setupGui();

  public slots:
    void widgetChanged(const QString& msg);
    void filterNeedsInputParameters(AbstractFilter* filter); // When the filter is ready for us to update its input parameter(s) that we are responsible for
    void beforePreflight(); // Called just before the "dataCheck()" is called
    void afterPreflight(); // Called just after the dataCheck() is called.


  signals:
    void errorSettingFilterParameter(const QString& msg);
    void parametersChanged();

  private:

    bool m_DidCausePreflight;

    FloatVec4FilterParameter* m_FilterParameter;


    FloatVec4Widget(const FloatVec4Widget&); // Copy Constructor Not Implemented
    void operator=(const FloatVec4Widget&); // Operator '=' Not Implemented

};

#endif /* _FloatVec4Widget_H_ */


