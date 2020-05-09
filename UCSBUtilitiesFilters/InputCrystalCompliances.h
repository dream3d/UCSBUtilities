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
#include "SIMPLib/DataArrays/DataArray.hpp"

#include "UCSBUtilities/FilterParameters/Symmetric6x6FilterParameter.h"
#include "UCSBUtilities/UCSBUtilitiesDLLExport.h"

/**
 * @brief The InputCrystalCompliances class. See [Filter documentation](@ref inputcrystalcompliances) for details.
 */
class UCSBUtilities_EXPORT InputCrystalCompliances : public AbstractFilter
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(InputCrystalCompliances SUPERCLASS AbstractFilter)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(InputCrystalCompliances)
  PYB11_FILTER_NEW_MACRO(InputCrystalCompliances)
  PYB11_PROPERTY(FloatVec21_t Compliances READ getCompliances WRITE setCompliances)
  PYB11_PROPERTY(DataArrayPath CrystalCompliancesArrayPath READ getCrystalCompliancesArrayPath WRITE setCrystalCompliancesArrayPath)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = InputCrystalCompliances;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<InputCrystalCompliances> New();

  /**
   * @brief Returns the name of the class for InputCrystalCompliances
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for InputCrystalCompliances
   */
  static QString ClassName();

  ~InputCrystalCompliances() override;

  /**
   * @brief Setter property for Compliances
   */
  void setCompliances(const FloatVec21_t& value);
  /**
   * @brief Getter property for Compliances
   * @return Value of Compliances
   */
  FloatVec21_t getCompliances() const;

  Q_PROPERTY(FloatVec21_t Compliances READ getCompliances WRITE setCompliances)

  /**
   * @brief Setter property for CrystalCompliancesArrayPath
   */
  void setCrystalCompliancesArrayPath(const DataArrayPath& value);
  /**
   * @brief Getter property for CrystalCompliancesArrayPath
   * @return Value of CrystalCompliancesArrayPath
   */
  DataArrayPath getCrystalCompliancesArrayPath() const;

  Q_PROPERTY(DataArrayPath CrystalCompliancesArrayPath READ getCrystalCompliancesArrayPath WRITE setCrystalCompliancesArrayPath)

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
  InputCrystalCompliances();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck() override;

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  std::weak_ptr<DataArray<float>> m_CrystalCompliancesPtr;
  float* m_CrystalCompliances = nullptr;

  FloatVec21_t m_Compliances = {};
  DataArrayPath m_CrystalCompliancesArrayPath = {};

public:
  InputCrystalCompliances(const InputCrystalCompliances&) = delete;            // Copy Constructor Not Implemented
  InputCrystalCompliances(InputCrystalCompliances&&) = delete;                 // Move Constructor Not Implemented
  InputCrystalCompliances& operator=(const InputCrystalCompliances&) = delete; // Copy Assignment Not Implemented
  InputCrystalCompliances& operator=(InputCrystalCompliances&&) = delete;      // Move Assignment Not Implemented
};
