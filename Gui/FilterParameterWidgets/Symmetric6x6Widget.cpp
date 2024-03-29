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

#include "Symmetric6x6Widget.h"

#include "SIMPLib/Utilities/FilterCompatibility.hpp"

#include "SVWidgetsLib/Core/SVWidgetsLibConstants.h"

#include "SVWidgetsLib/FilterParameterWidgets/FilterParameterWidgetsDialogs.h"

#include "UCSBUtilities/FilterParameters/Symmetric6x6FilterParameter.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Symmetric6x6Widget::Symmetric6x6Widget(FilterParameter* parameter, AbstractFilter* filter, QWidget* parent)
: FilterParameterWidget(parameter, filter, parent)
{
  m_FilterParameter = SIMPL_FILTER_PARAMETER_COMPATIBILITY_CHECK(filter, parameter, Symmetric6x6Widget, Symmetric6x6FilterParameter);

  setupUi(this);
  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Symmetric6x6Widget::~Symmetric6x6Widget() = default;

void Symmetric6x6Widget::setCubicSym()
{
  // 11 == 22 == 33
  connect(v11, SIGNAL(textChanged(const QString&)), v22, SLOT(setText(const QString&)));
  connect(v22, SIGNAL(textChanged(const QString&)), v33, SLOT(setText(const QString&)));
  connect(v33, SIGNAL(textChanged(const QString&)), v11, SLOT(setText(const QString&)));
  v22->setText(v11->text());
  v33->setText(v11->text());

  // 12 == 13 == 23
  connect(v12, SIGNAL(textChanged(const QString&)), v13, SLOT(setText(const QString&)));
  connect(v13, SIGNAL(textChanged(const QString&)), v23, SLOT(setText(const QString&)));
  connect(v23, SIGNAL(textChanged(const QString&)), v12, SLOT(setText(const QString&)));
  v13->setText(v12->text());
  v23->setText(v12->text());

  // 44 == 55 == 66
  connect(v44, SIGNAL(textChanged(const QString&)), v55, SLOT(setText(const QString&)));
  connect(v55, SIGNAL(textChanged(const QString&)), v66, SLOT(setText(const QString&)));
  connect(v66, SIGNAL(textChanged(const QString&)), v44, SLOT(setText(const QString&)));
  v55->setText(v44->text());
  v66->setText(v44->text());

  // all others = 0
  v14->setText("0");
  v14->setEnabled(false);
  v15->setText("0");
  v15->setEnabled(false);
  v16->setText("0");
  v16->setEnabled(false);

  v24->setText("0");
  v24->setEnabled(false);
  v25->setText("0");
  v25->setEnabled(false);
  v26->setText("0");
  v26->setEnabled(false);

  v34->setText("0");
  v34->setEnabled(false);
  v35->setText("0");
  v35->setEnabled(false);
  v36->setText("0");
  v36->setEnabled(false);

  v45->setText("0");
  v45->setEnabled(false);
  v46->setText("0");
  v46->setEnabled(false);

  v56->setText("0");
  v56->setEnabled(false);
}

void Symmetric6x6Widget::setHexagonalSym()
{
  // s11 == s22
  connect(v11, SIGNAL(textChanged(const QString&)), v22, SLOT(setText(const QString&)));
  connect(v22, SIGNAL(textChanged(const QString&)), v11, SLOT(setText(const QString&)));
  v22->setText(v11->text());

  // s13 == s23
  connect(v13, SIGNAL(textChanged(const QString&)), v23, SLOT(setText(const QString&)));
  connect(v23, SIGNAL(textChanged(const QString&)), v13, SLOT(setText(const QString&)));
  v23->setText(v13->text());

  // s44 == s55
  connect(v44, SIGNAL(textChanged(const QString&)), v55, SLOT(setText(const QString&)));
  connect(v55, SIGNAL(textChanged(const QString&)), v44, SLOT(setText(const QString&)));
  v55->setText(v44->text());

  // s66 == 2(s11-s12)
  connect(v11, SIGNAL(textChanged(const QString&)), this, SLOT(vbar66(const QString&)));
  connect(v12, SIGNAL(textChanged(const QString&)), this, SLOT(vbar66(const QString&)));
  v66->setEnabled(false);
  vbar66("");

  // s12, s33 independant

  // all others = 0
  v14->setText("0");
  v14->setEnabled(false);
  v15->setText("0");
  v15->setEnabled(false);
  v16->setText("0");
  v16->setEnabled(false);

  v24->setText("0");
  v24->setEnabled(false);
  v25->setText("0");
  v25->setEnabled(false);
  v26->setText("0");
  v26->setEnabled(false);

  v34->setText("0");
  v34->setEnabled(false);
  v35->setText("0");
  v35->setEnabled(false);
  v36->setText("0");
  v36->setEnabled(false);

  v45->setText("0");
  v45->setEnabled(false);
  v46->setText("0");
  v46->setEnabled(false);

  v56->setText("0");
  v56->setEnabled(false);
}

void Symmetric6x6Widget::setTetHighSym()
{
  // s11 == s22
  connect(v11, SIGNAL(textChanged(const QString&)), v22, SLOT(setText(const QString&)));
  connect(v22, SIGNAL(textChanged(const QString&)), v11, SLOT(setText(const QString&)));
  v22->setText(v11->text());

  // s13 == s23
  connect(v13, SIGNAL(textChanged(const QString&)), v23, SLOT(setText(const QString&)));
  connect(v23, SIGNAL(textChanged(const QString&)), v13, SLOT(setText(const QString&)));
  v23->setText(v13->text());

  // s44 == s55
  connect(v44, SIGNAL(textChanged(const QString&)), v55, SLOT(setText(const QString&)));
  connect(v55, SIGNAL(textChanged(const QString&)), v44, SLOT(setText(const QString&)));
  v55->setText(v44->text());

  // s12, s33, s66 independant

  // all others = 0
  v14->setText("0");
  v14->setEnabled(false);
  v15->setText("0");
  v15->setEnabled(false);
  v16->setText("0");
  v16->setEnabled(false);

  v24->setText("0");
  v24->setEnabled(false);
  v25->setText("0");
  v25->setEnabled(false);
  v26->setText("0");
  v26->setEnabled(false);

  v34->setText("0");
  v34->setEnabled(false);
  v35->setText("0");
  v35->setEnabled(false);
  v36->setText("0");
  v36->setEnabled(false);

  v45->setText("0");
  v45->setEnabled(false);
  v46->setText("0");
  v46->setEnabled(false);

  v56->setText("0");
  v56->setEnabled(false);
}

void Symmetric6x6Widget::setTetLowSym()
{
  // s11 == s22
  connect(v11, SIGNAL(textChanged(const QString&)), v22, SLOT(setText(const QString&)));
  connect(v22, SIGNAL(textChanged(const QString&)), v11, SLOT(setText(const QString&)));
  v22->setText(v11->text());

  // s13 == s23
  connect(v13, SIGNAL(textChanged(const QString&)), v23, SLOT(setText(const QString&)));
  connect(v23, SIGNAL(textChanged(const QString&)), v13, SLOT(setText(const QString&)));
  v23->setText(v13->text());

  // s44 == s55
  connect(v44, SIGNAL(textChanged(const QString&)), v55, SLOT(setText(const QString&)));
  connect(v55, SIGNAL(textChanged(const QString&)), v44, SLOT(setText(const QString&)));
  v55->setText(v44->text());

  // s26 == -s16
  connect(v16, SIGNAL(textChanged(const QString&)), this, SLOT(tetragonalv16Changed(const QString&)));
  connect(v26, SIGNAL(textChanged(const QString&)), this, SLOT(tetragonalv26Changed(const QString&)));
  tetragonalv16Changed("");

  // s16, s12, s33, s66 independant

  // all others = 0
  v14->setText("0");
  v14->setEnabled(false);
  v15->setText("0");
  v15->setEnabled(false);

  v24->setText("0");
  v24->setEnabled(false);
  v25->setText("0");
  v25->setEnabled(false);

  v34->setText("0");
  v34->setEnabled(false);
  v35->setText("0");
  v35->setEnabled(false);
  v36->setText("0");
  v36->setEnabled(false);

  v45->setText("0");
  v45->setEnabled(false);
  v46->setText("0");
  v46->setEnabled(false);

  v56->setText("0");
  v56->setEnabled(false);
}

void Symmetric6x6Widget::setTrigHighSym()
{
  // s11 == s22
  connect(v11, SIGNAL(textChanged(const QString&)), v22, SLOT(setText(const QString&)));
  connect(v22, SIGNAL(textChanged(const QString&)), v11, SLOT(setText(const QString&)));
  v22->setText(v11->text());

  // s13 == s23
  connect(v13, SIGNAL(textChanged(const QString&)), v23, SLOT(setText(const QString&)));
  connect(v23, SIGNAL(textChanged(const QString&)), v13, SLOT(setText(const QString&)));
  v23->setText(v13->text());

  // s55 == s66
  connect(v44, SIGNAL(textChanged(const QString&)), v55, SLOT(setText(const QString&)));
  connect(v55, SIGNAL(textChanged(const QString&)), v44, SLOT(setText(const QString&)));
  v55->setText(v44->text());

  // s14 == -s24 && s56 == 2*s14
  connect(v14, SIGNAL(textChanged(const QString&)), this, SLOT(trigonalv14Changed(const QString&)));
  connect(v24, SIGNAL(textChanged(const QString&)), this, SLOT(trigonalv24Changed(const QString&)));
  connect(v56, SIGNAL(textChanged(const QString&)), this, SLOT(trigonalv56Changed(const QString&)));
  trigonalv14Changed("");

  // s66 == 2(s11 + s12)
  connect(v11, SIGNAL(textChanged(const QString&)), this, SLOT(vbar66(const QString&)));
  connect(v12, SIGNAL(textChanged(const QString&)), this, SLOT(vbar66(const QString&)));
  v66->setEnabled(false);
  vbar66("");

  // s12, s33 independant

  // all other = 0
  v15->setText("0");
  v15->setEnabled(false);
  v16->setText("0");
  v16->setEnabled(false);

  v25->setText("0");
  v25->setEnabled(false);
  v26->setText("0");
  v26->setEnabled(false);

  v34->setText("0");
  v34->setEnabled(false);
  v35->setText("0");
  v35->setEnabled(false);
  v36->setText("0");
  v36->setEnabled(false);

  v45->setText("0");
  v45->setEnabled(false);
  v46->setText("0");
  v46->setEnabled(false);
}

void Symmetric6x6Widget::setTrigLowSym()
{
  // s11 == s22
  connect(v11, SIGNAL(textChanged(const QString&)), v22, SLOT(setText(const QString&)));
  connect(v22, SIGNAL(textChanged(const QString&)), v11, SLOT(setText(const QString&)));
  v22->setText(v11->text());

  // s13 == s23
  connect(v13, SIGNAL(textChanged(const QString&)), v23, SLOT(setText(const QString&)));
  connect(v23, SIGNAL(textChanged(const QString&)), v13, SLOT(setText(const QString&)));
  v23->setText(v13->text());

  // s55 == s66
  connect(v44, SIGNAL(textChanged(const QString&)), v55, SLOT(setText(const QString&)));
  connect(v55, SIGNAL(textChanged(const QString&)), v44, SLOT(setText(const QString&)));
  v55->setText(v44->text());

  // s14 == -s24 && s56 == 2*s14
  connect(v14, SIGNAL(textChanged(const QString&)), this, SLOT(trigonalv14Changed(const QString&)));
  connect(v24, SIGNAL(textChanged(const QString&)), this, SLOT(trigonalv24Changed(const QString&)));
  connect(v56, SIGNAL(textChanged(const QString&)), this, SLOT(trigonalv56Changed(const QString&)));
  trigonalv14Changed("");

  // s25 == -s15 && s46 == 2*s25
  connect(v15, SIGNAL(textChanged(const QString&)), this, SLOT(trigonalv15Changed(const QString&)));
  connect(v25, SIGNAL(textChanged(const QString&)), this, SLOT(trigonalv25Changed(const QString&)));
  connect(v46, SIGNAL(textChanged(const QString&)), this, SLOT(trigonalv46Changed(const QString&)));
  trigonalv25Changed("");

  // s66 == 2(s11 + s12)
  connect(v11, SIGNAL(textChanged(const QString&)), this, SLOT(vbar66(const QString&)));
  connect(v12, SIGNAL(textChanged(const QString&)), this, SLOT(vbar66(const QString&)));
  v66->setEnabled(false);
  vbar66("");

  // s12, s33 independant

  // all other = 0
  v16->setText("0");
  v16->setEnabled(false);

  v26->setText("0");
  v26->setEnabled(false);

  v34->setText("0");
  v34->setEnabled(false);
  v35->setText("0");
  v35->setEnabled(false);
  v36->setText("0");
  v36->setEnabled(false);

  v45->setText("0");
  v45->setEnabled(false);
}

void Symmetric6x6Widget::setOrthorhomicSym()
{
  // s11, s22, s33 independant
  // s44, s55, s66 independant
  // s12, s13, s23 independant

  // all others = 0
  v14->setText("0");
  v14->setEnabled(false);
  v15->setText("0");
  v15->setEnabled(false);
  v16->setText("0");
  v16->setEnabled(false);

  v24->setText("0");
  v24->setEnabled(false);
  v25->setText("0");
  v25->setEnabled(false);
  v26->setText("0");
  v26->setEnabled(false);

  v34->setText("0");
  v34->setEnabled(false);
  v35->setText("0");
  v35->setEnabled(false);
  v36->setText("0");
  v36->setEnabled(false);

  v45->setText("0");
  v45->setEnabled(false);
  v46->setText("0");
  v46->setEnabled(false);

  v56->setText("0");
  v56->setEnabled(false);
}

void Symmetric6x6Widget::setMonoclinicSym()
{
  // s11, s22, s33 independant
  // s44, s55, s66 independant
  // s12, s13, s23 indpenedant
  // s15, s25, s35 indpenedant
  // s46 independant

  // all others = 0
  v14->setText("0");
  v14->setEnabled(false);
  v16->setText("0");
  v16->setEnabled(false);

  v24->setText("0");
  v24->setEnabled(false);
  v26->setText("0");
  v26->setEnabled(false);

  v34->setText("0");
  v34->setEnabled(false);
  v36->setText("0");
  v36->setEnabled(false);

  v45->setText("0");
  v45->setEnabled(false);

  v56->setText("0");
  v56->setEnabled(false);
}

void Symmetric6x6Widget::setTriclinicSym()
{
  // all independant
}

void Symmetric6x6Widget::clearSym()
{
  disconnect(v11, nullptr, nullptr, nullptr);
  disconnect(v12, nullptr, nullptr, nullptr);
  disconnect(v13, nullptr, nullptr, nullptr);
  disconnect(v14, nullptr, nullptr, nullptr);
  disconnect(v15, nullptr, nullptr, nullptr);
  disconnect(v16, nullptr, nullptr, nullptr);

  disconnect(v22, nullptr, nullptr, nullptr);
  disconnect(v23, nullptr, nullptr, nullptr);
  disconnect(v24, nullptr, nullptr, nullptr);
  disconnect(v25, nullptr, nullptr, nullptr);
  disconnect(v26, nullptr, nullptr, nullptr);

  disconnect(v33, nullptr, nullptr, nullptr);
  disconnect(v34, nullptr, nullptr, nullptr);
  disconnect(v35, nullptr, nullptr, nullptr);
  disconnect(v36, nullptr, nullptr, nullptr);

  disconnect(v44, nullptr, nullptr, nullptr);
  disconnect(v45, nullptr, nullptr, nullptr);
  disconnect(v46, nullptr, nullptr, nullptr);

  disconnect(v55, nullptr, nullptr, nullptr);
  disconnect(v56, nullptr, nullptr, nullptr);

  disconnect(v66, nullptr, nullptr, nullptr);

  v11->setEnabled(true);
  v12->setEnabled(true);
  v13->setEnabled(true);
  v14->setEnabled(true);
  v15->setEnabled(true);
  v16->setEnabled(true);

  v22->setEnabled(true);
  v23->setEnabled(true);
  v24->setEnabled(true);
  v25->setEnabled(true);
  v26->setEnabled(true);

  v33->setEnabled(true);
  v34->setEnabled(true);
  v35->setEnabled(true);
  v36->setEnabled(true);

  v44->setEnabled(true);
  v45->setEnabled(true);
  v46->setEnabled(true);

  v55->setEnabled(true);
  v56->setEnabled(true);

  v66->setEnabled(true);
}

void Symmetric6x6Widget::changeSym(int newState)
{
  clearSym();

  switch(newState)
  {
  case 0:
    setCubicSym();
    break;

  case 1:
    setHexagonalSym();
    break;

  case 2:
    setTetHighSym();
    break;

  case 3:
    setTetLowSym();
    break;

  case 4:
    setTrigHighSym();
    break;

  case 5:
    setTrigLowSym();
    break;

  case 6:
    setOrthorhomicSym();
    break;

  case 7:
    setMonoclinicSym();
    break;

  case 8:
    setTriclinicSym();
    break;
  }
}

// some hexagonal and trigonal low/high have s66 == 2 * (s11 - s12)
void Symmetric6x6Widget::vbar66(const QString& dummy)
{
  double s11 = v11->text().toDouble();
  double s12 = v12->text().toDouble();
  v66->setText(QString::number(2.0 * (s11 - s12)));
}

// tetragonal low has s26 == -s16
void Symmetric6x6Widget::tetragonalv16Changed(const QString& dummy)
{
  double v = v16->text().toDouble();
  v26->setText(QString::number(-v));
}
void Symmetric6x6Widget::tetragonalv26Changed(const QString& dummy)
{
  double v = v26->text().toDouble();
  v16->setText(QString::number(-v));
}

// trigonal low/high have s14 == -s24 && s56 == 2*s14
void Symmetric6x6Widget::trigonalv14Changed(const QString& dummy)
{
  double v = v14->text().toDouble();
  v24->setText(QString::number(-v));
  v56->setText(QString::number(2.0 * v));
}
void Symmetric6x6Widget::trigonalv24Changed(const QString& dummy)
{
  double v = v24->text().toDouble();
  v14->setText(QString::number(-v));
  v56->setText(QString::number(-2.0 * v));
}
void Symmetric6x6Widget::trigonalv56Changed(const QString& dummy)
{
  double v = v56->text().toDouble();
  v14->setText(QString::number(v / 2.0));
  v24->setText(QString::number(-v / 2.0));
}

// trigonal low has s15 == -s25 && s46 == 2*s25
void Symmetric6x6Widget::trigonalv15Changed(const QString& dummy)
{
  double v = v15->text().toDouble();
  v25->setText(QString::number(-v));
  v46->setText(QString::number(-2.0 * v));
}
void Symmetric6x6Widget::trigonalv25Changed(const QString& dummy)
{
  double v = v25->text().toDouble();
  v15->setText(QString::number(-v));
  v46->setText(QString::number(2.0 * v));
}
void Symmetric6x6Widget::trigonalv46Changed(const QString& dummy)
{
  double v = v46->text().toDouble();
  v15->setText(QString::number(-v / 2.0));
  v25->setText(QString::number(v / 2.0));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Symmetric6x6Widget::setupGui()
{
  // Catch when the filter is about to execute the preflight
  connect(getFilter(), &AbstractFilter::preflightAboutToExecute, this, &Symmetric6x6Widget::beforePreflight);

  // Catch when the filter is finished running the preflight
  connect(getFilter(), &AbstractFilter::preflightExecuted, this, &Symmetric6x6Widget::afterPreflight);

  // Catch when the filter wants its values updated
  connect(getFilter(), &AbstractFilter::updateFilterParameters, this, &Symmetric6x6Widget::filterNeedsInputParameters);

  connect(v11, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v12, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v13, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v14, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v15, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v16, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v22, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v23, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v24, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v25, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v26, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v33, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v34, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v35, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v36, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v44, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v45, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v46, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v55, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v56, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));
  connect(v66, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));

  QDoubleValidator* v11Val = new QDoubleValidator(v11);
  v11->setValidator(v11Val);
  QDoubleValidator* v12Val = new QDoubleValidator(v12);
  v12->setValidator(v12Val);
  QDoubleValidator* v13Val = new QDoubleValidator(v13);
  v13->setValidator(v13Val);
  QDoubleValidator* v14Val = new QDoubleValidator(v14);
  v14->setValidator(v14Val);
  QDoubleValidator* v15Val = new QDoubleValidator(v15);
  v15->setValidator(v15Val);
  QDoubleValidator* v16Val = new QDoubleValidator(v16);
  v16->setValidator(v16Val);

  QDoubleValidator* v22Val = new QDoubleValidator(v22);
  v22->setValidator(v22Val);
  QDoubleValidator* v23Val = new QDoubleValidator(v23);
  v23->setValidator(v23Val);
  QDoubleValidator* v24Val = new QDoubleValidator(v24);
  v24->setValidator(v24Val);
  QDoubleValidator* v25Val = new QDoubleValidator(v25);
  v25->setValidator(v25Val);
  QDoubleValidator* v26Val = new QDoubleValidator(v26);
  v26->setValidator(v26Val);

  QDoubleValidator* v33Val = new QDoubleValidator(v33);
  v33->setValidator(v33Val);
  QDoubleValidator* v34Val = new QDoubleValidator(v34);
  v34->setValidator(v34Val);
  QDoubleValidator* v35Val = new QDoubleValidator(v35);
  v35->setValidator(v35Val);
  QDoubleValidator* v36Val = new QDoubleValidator(v36);
  v36->setValidator(v36Val);

  QDoubleValidator* v44Val = new QDoubleValidator(v44);
  v44->setValidator(v44Val);
  QDoubleValidator* v45Val = new QDoubleValidator(v45);
  v45->setValidator(v45Val);
  QDoubleValidator* v46Val = new QDoubleValidator(v46);
  v46->setValidator(v46Val);

  QDoubleValidator* v55Val = new QDoubleValidator(v55);
  v55->setValidator(v55Val);
  QDoubleValidator* v56Val = new QDoubleValidator(v56);
  v56->setValidator(v56Val);

  QDoubleValidator* v66Val = new QDoubleValidator(v66);
  v66->setValidator(v66Val);

  if(getFilterParameter() != nullptr)
  {
    label->setText(getFilterParameter()->getHumanLabel());

    FloatVec21_t data = SafeFilterParameterGetter(m_FilterParameter, getFilter());
    v11->setText(QString::number(data.v11));
    v12->setText(QString::number(data.v12));
    v13->setText(QString::number(data.v13));
    v14->setText(QString::number(data.v14));
    v15->setText(QString::number(data.v15));
    v16->setText(QString::number(data.v16));

    v22->setText(QString::number(data.v22));
    v23->setText(QString::number(data.v23));
    v24->setText(QString::number(data.v24));
    v25->setText(QString::number(data.v25));
    v26->setText(QString::number(data.v26));

    v33->setText(QString::number(data.v33));
    v34->setText(QString::number(data.v34));
    v35->setText(QString::number(data.v35));
    v36->setText(QString::number(data.v36));

    v44->setText(QString::number(data.v44));
    v45->setText(QString::number(data.v45));
    v46->setText(QString::number(data.v46));

    v55->setText(QString::number(data.v55));
    v56->setText(QString::number(data.v56));

    v66->setText(QString::number(data.v66));
  }

  // connect combobox to symmetry state
  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSym(int)));
  clearSym();
  changeSym(comboBox->currentIndex());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Symmetric6x6Widget::widgetChanged(const QString& text)
{
  Q_EMIT parametersChanged();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Symmetric6x6Widget::filterNeedsInputParameters(AbstractFilter* filter)
{
  bool ok = false;
  FloatVec21_t data;
  data.v11 = v11->text().toDouble(&ok);
  data.v12 = v12->text().toDouble(&ok);
  data.v13 = v13->text().toDouble(&ok);
  data.v14 = v14->text().toDouble(&ok);
  data.v15 = v15->text().toDouble(&ok);
  data.v16 = v16->text().toDouble(&ok);

  data.v22 = v22->text().toDouble(&ok);
  data.v23 = v23->text().toDouble(&ok);
  data.v24 = v24->text().toDouble(&ok);
  data.v25 = v25->text().toDouble(&ok);
  data.v26 = v26->text().toDouble(&ok);

  data.v33 = v33->text().toDouble(&ok);
  data.v34 = v34->text().toDouble(&ok);
  data.v35 = v35->text().toDouble(&ok);
  data.v36 = v36->text().toDouble(&ok);

  data.v44 = v44->text().toDouble(&ok);
  data.v45 = v45->text().toDouble(&ok);
  data.v46 = v46->text().toDouble(&ok);

  data.v55 = v55->text().toDouble(&ok);
  data.v56 = v56->text().toDouble(&ok);

  data.v66 = v66->text().toDouble(&ok);

  SafeFilterParameterSetter(m_FilterParameter, data, getFilter());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Symmetric6x6Widget::beforePreflight()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Symmetric6x6Widget::afterPreflight()
{
}
