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

#include <QtGui/QDoubleValidator>

#include "SIMPLib/Utilities/FilterCompatibility.hpp"

#include "SVWidgetsLib/Core/SVWidgetsLibConstants.h"
#include "SVWidgetsLib/FilterParameterWidgets/FilterParameterWidgetsDialogs.h"

#include "UCSBUtilities/FilterParameters/FloatVec4FilterParameter.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FloatVec4Widget::FloatVec4Widget(FilterParameter* parameter, AbstractFilter* filter, QWidget* parent)
: FilterParameterWidget(parameter, filter, parent)
{
  m_FilterParameter = SIMPL_FILTER_PARAMETER_COMPATIBILITY_CHECK(filter, parameter, FloatVec4Widget, FloatVec4FilterParameter);

  setupUi(this);
  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FloatVec4Widget::~FloatVec4Widget() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FloatVec4Widget::setupGui()
{
  // Catch when the filter is about to execute the preflight
  connect(getFilter(), SIGNAL(preflightAboutToExecute()), this, SLOT(beforePreflight()));

  // Catch when the filter is finished running the preflight
  connect(getFilter(), SIGNAL(preflightExecuted()), this, SLOT(afterPreflight()));

  // Catch when the filter wants its values updated
  connect(getFilter(), SIGNAL(updateFilterParameters(AbstractFilter*)), this, SLOT(filterNeedsInputParameters(AbstractFilter*)));

  connect(aData, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(bData, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(cData, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(dData, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));

  QDoubleValidator* aVal = new QDoubleValidator(aData);
  aData->setValidator(aVal);
  QDoubleValidator* bVal = new QDoubleValidator(bData);
  bData->setValidator(bVal);
  QDoubleValidator* cVal = new QDoubleValidator(cData);
  cData->setValidator(cVal);
  QDoubleValidator* dVal = new QDoubleValidator(dData);
  dData->setValidator(dVal);
  if(getFilterParameter() != nullptr)
  {
    label->setText(getFilterParameter()->getHumanLabel());

    FloatVec4Type data = m_FilterParameter->getGetterCallback()();
    aData->setText(QString::number(data[0]));
    bData->setText(QString::number(data[1]));
    cData->setText(QString::number(data[2]));
    dData->setText(QString::number(data[3]));
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
  FloatVec4Type data;
  data[0] = aData->text().toDouble(&ok);
  data[1] = bData->text().toDouble(&ok);
  data[2] = cData->text().toDouble(&ok);
  data[3] = dData->text().toDouble(&ok);

  m_FilterParameter->getSetterCallback()(data);
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
