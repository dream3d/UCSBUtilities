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

#include <memory>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Filtering/AbstractFilter.h"

#include "UCSBUtilities/UCSBUtilitiesDLLExport.h"

/**
 * @brief The CopyAttributeMatrix class. See [Filter documentation](@ref copyattributematrix) for details.
 */
class UCSBUtilities_EXPORT CopyAttributeMatrix : public AbstractFilter
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(CopyAttributeMatrix SUPERCLASS AbstractFilter)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(CopyAttributeMatrix)
  PYB11_FILTER_NEW_MACRO(CopyAttributeMatrix)
  PYB11_PROPERTY(DataArrayPath SelectedAttributeMatrixPath READ getSelectedAttributeMatrixPath WRITE setSelectedAttributeMatrixPath)
  PYB11_PROPERTY(QString NewAttributeMatrix READ getNewAttributeMatrix WRITE setNewAttributeMatrix)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = CopyAttributeMatrix;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<CopyAttributeMatrix> New();

  /**
   * @brief Returns the name of the class for CopyAttributeMatrix
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for CopyAttributeMatrix
   */
  static QString ClassName();

  ~CopyAttributeMatrix() override;

  /**
   * @brief Setter property for SelectedAttributeMatrixPath
   */
  void setSelectedAttributeMatrixPath(const DataArrayPath& value);
  /**
   * @brief Getter property for SelectedAttributeMatrixPath
   * @return Value of SelectedAttributeMatrixPath
   */
  DataArrayPath getSelectedAttributeMatrixPath() const;

  Q_PROPERTY(DataArrayPath SelectedAttributeMatrixPath READ getSelectedAttributeMatrixPath WRITE setSelectedAttributeMatrixPath)

  /**
   * @brief Setter property for NewAttributeMatrix
   */
  void setNewAttributeMatrix(const QString& value);
  /**
   * @brief Getter property for NewAttributeMatrix
   * @return Value of NewAttributeMatrix
   */
  QString getNewAttributeMatrix() const;

  Q_PROPERTY(QString NewAttributeMatrix READ getNewAttributeMatrix WRITE setNewAttributeMatrix)

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  QString getCompiledLibraryName() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
   */
  QString getBrandingString() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  QString getFilterVersion() const override;

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  QString getGroupName() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  QUuid getUuid() const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  QString getHumanLabel() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  /**
   * @brief execute Reimplemented from @see AbstractFilter class
   */
  void execute() override;

protected:
  CopyAttributeMatrix();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck() override;

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

public:
  CopyAttributeMatrix(const CopyAttributeMatrix&) = delete;            // Copy Constructor Not Implemented
  CopyAttributeMatrix(CopyAttributeMatrix&&) = delete;                 // Move Constructor Not Implemented
  CopyAttributeMatrix& operator=(const CopyAttributeMatrix&) = delete; // Copy Assignment Not Implemented
  CopyAttributeMatrix& operator=(CopyAttributeMatrix&&) = delete;      // Move Assignment Not Implemented

private:
  DataArrayPath m_SelectedAttributeMatrixPath = {};
  QString m_NewAttributeMatrix = {};
};
