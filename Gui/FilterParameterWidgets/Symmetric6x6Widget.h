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

#pragma once

#include <QtWidgets/QWidget>

#include "SVWidgetsLib/QtSupport/QtSFaderWidget.h"

#include "SIMPLib/Filtering/AbstractFilter.h"

#include "SVWidgetsLib/FilterParameterWidgets/FilterParameterWidget.h"
#include "SVWidgetsLib/SVWidgetsLib.h"

#include "ui_Symmetric6x6Widget.h"

#include "UCSBUtilities/FilterParameters/Symmetric6x6FilterParameter.h"

/**
 * @brief
 * @author
 * @version
 */
class Symmetric6x6Widget : public FilterParameterWidget, private Ui::Symmetric6x6Widget
{
  Q_OBJECT

public:
  /**
   * @brief Constructor
   * @param parameter The FilterParameter object that this widget represents
   * @param filter The instance of the filter that this parameter is a part of
   * @param parent The parent QWidget for this Widget
   */
  Symmetric6x6Widget(FilterParameter* parameter, AbstractFilter* filter = nullptr, QWidget* parent = nullptr);

  virtual ~Symmetric6x6Widget();

  /**
   * @brief This method does additional GUI widget connections
   */
  void setupGui();

public slots:
  void widgetChanged(const QString& msg);
  void filterNeedsInputParameters(AbstractFilter* filter); // When the filter is ready for us to update its input parameter(s) that we are responsible for
  void beforePreflight();                                  // Called just before the "dataCheck()" is called
  void afterPreflight();                                   // Called just after the dataCheck() is called.

  void changeSym(int newState); // slot to connect combobox to symmetry state

  // slots for special symmetry relationships
  void vbar66(const QString& dummy);
  void tetragonalv16Changed(const QString& dummy);
  void tetragonalv26Changed(const QString& dummy);

  void trigonalv14Changed(const QString& dummy);
  void trigonalv24Changed(const QString& dummy);
  void trigonalv56Changed(const QString& dummy);

  void trigonalv15Changed(const QString& dummy);
  void trigonalv25Changed(const QString& dummy);
  void trigonalv46Changed(const QString& dummy);

private:
  Symmetric6x6FilterParameter* m_FilterParameter;

  // functions for changing symmetry type
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

  // helper functions for complex inter dependancies
  // void vbar(QLineEdit* obj);

  Symmetric6x6Widget(const Symmetric6x6Widget&) = delete; // Copy Constructor Not Implemented
  void operator=(const Symmetric6x6Widget&) = delete;     // Move assignment Not Implemented
};
