/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                               *
 * Copyright (c) 2015 The Regents of the University of California, Author: William Lenthe        *
 *                                                                                               *
 * This program is free software: you can redistribute it and/or modify it under the terms of    *
 * the GNU Lesser General Public License as published by the Free Software Foundation, either    *
 * version 2 of the License, or (at your option) any later version.                              *
 *                                                                                               *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     *
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     *
 * See the GNU Lesser General Public License for more details.                                   *
 *                                                                                               *
 * You should have received a copy of the GNU Lesser General Public License                      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.                         *
 *                                                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "FloatVec4Widget.h"

#include <QtCore/QMetaProperty>


#include "DREAM3DWidgetsLib/DREAM3DWidgetsLibConstants.h"

#include "DREAM3DWidgetsLib/FilterParameterWidgets/FilterParameterWidgetsDialogs.h"

#include "UCSBUtilities/FilterParameters/FloatVec4FilterParameter.h"


// Include the MOC generated file for this class
#include "moc_FloatVec4Widget.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FloatVec4Widget::FloatVec4Widget(FilterParameter* parameter, AbstractFilter* filter, QWidget* parent) :
  FilterParameterWidget(parameter, filter, parent)
{
  m_FilterParameter = dynamic_cast<FloatVec4FilterParameter*>(parameter);
  Q_ASSERT_X(m_FilterParameter != NULL, "NULL Pointer", "FloatVec4Widget can ONLY be used with a FloatVec4FilterParameter object");

  setupUi(this);
  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FloatVec4Widget::~FloatVec4Widget()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FloatVec4Widget::setupGui()
{
  // Catch when the filter is about to execute the preflight
  connect(getFilter(), SIGNAL(preflightAboutToExecute()),
          this, SLOT(beforePreflight()));

  // Catch when the filter is finished running the preflight
  connect(getFilter(), SIGNAL(preflightExecuted()),
          this, SLOT(afterPreflight()));

  // Catch when the filter wants its values updated
  connect(getFilter(), SIGNAL(updateFilterParameters(AbstractFilter*)),
          this, SLOT(filterNeedsInputParameters(AbstractFilter*)));


  connect(aData, SIGNAL(textChanged(const QString&)),
          this, SLOT(widgetChanged(const QString&) ) );
  connect(bData, SIGNAL(textChanged(const QString&)),
          this, SLOT(widgetChanged(const QString&) ) );
  connect(cData, SIGNAL(textChanged(const QString&)),
          this, SLOT(widgetChanged(const QString&) ) );
  connect(dData, SIGNAL(textChanged(const QString&)),
          this, SLOT(widgetChanged(const QString&) ) );

  QDoubleValidator* aVal = new QDoubleValidator(aData);
  aData->setValidator(aVal);
  QDoubleValidator* bVal = new QDoubleValidator(bData);
  bData->setValidator(bVal);
  QDoubleValidator* cVal = new QDoubleValidator(cData);
  cData->setValidator(cVal);
  QDoubleValidator* dVal = new QDoubleValidator(dData);
  dData->setValidator(dVal);
  if (getFilterParameter() != NULL)
  {
    label->setText(getFilterParameter()->getHumanLabel() );

    FloatVec4_t data = getFilter()->property(PROPERTY_NAME_AS_CHAR).value<FloatVec4_t>();
    aData->setText(QString::number(data.a) );
    bData->setText(QString::number(data.b) );
    cData->setText(QString::number(data.c) );
    dData->setText(QString::number(data.d) );
  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FloatVec4Widget::widgetChanged(const QString& text)
{
  emit parametersChanged();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FloatVec4Widget::filterNeedsInputParameters(AbstractFilter* filter)
{
  bool ok = false;
  FloatVec4_t data;
  data.a = aData->text().toDouble(&ok);
  data.b = bData->text().toDouble(&ok);
  data.c = cData->text().toDouble(&ok);
  data.d = dData->text().toDouble(&ok);

  QVariant v;
  v.setValue(data);
  ok = filter->setProperty(PROPERTY_NAME_AS_CHAR, v);
  if(false == ok)
  {
    FilterParameterWidgetsDialogs::ShowCouldNotSetFilterParameter(getFilter(), getFilterParameter());
  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FloatVec4Widget::beforePreflight()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FloatVec4Widget::afterPreflight()
{

}
