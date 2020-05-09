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

#include <vector>
#include <QtCore/QString>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/DataArrays/IDataArray.h"
#include "SIMPLib/DataContainers/DataContainer.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/DataArrays/DataArray.hpp"

#include "UCSBUtilities/UCSBUtilitiesDLLExport.h"

/**
 * @class FindDirectionalModuli FindDirectionalModuli.h DREAM3DLib/GenericFilters/FindDirectionalModuli.h
 * @brief
 * @author
 * @date Nov 19, 2011
 * @version 1.0
 */
class UCSBUtilities_EXPORT FindDirectionalModuli : public AbstractFilter
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(FindDirectionalModuli SUPERCLASS AbstractFilter)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(FindDirectionalModuli)
  PYB11_FILTER_NEW_MACRO(FindDirectionalModuli)
  PYB11_PROPERTY(DataArrayPath FeaturePhasesArrayPath READ getFeaturePhasesArrayPath WRITE setFeaturePhasesArrayPath)
  PYB11_PROPERTY(DataArrayPath CrystalStructuresArrayPath READ getCrystalStructuresArrayPath WRITE setCrystalStructuresArrayPath)
  PYB11_PROPERTY(DataArrayPath CrystalCompliancesArrayPath READ getCrystalCompliancesArrayPath WRITE setCrystalCompliancesArrayPath)
  PYB11_PROPERTY(DataArrayPath AvgQuatsArrayPath READ getAvgQuatsArrayPath WRITE setAvgQuatsArrayPath)
  PYB11_PROPERTY(QString DirectionalModuliArrayName READ getDirectionalModuliArrayName WRITE setDirectionalModuliArrayName)
  PYB11_PROPERTY(FloatVec3Type LoadingDirection READ getLoadingDirection WRITE setLoadingDirection)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = FindDirectionalModuli;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<FindDirectionalModuli> New();

  /**
   * @brief Returns the name of the class for FindDirectionalModuli
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for FindDirectionalModuli
   */
  static QString ClassName();

  ~FindDirectionalModuli() override;

  /**
   * @brief Setter property for FeaturePhasesArrayPath
   */
  void setFeaturePhasesArrayPath(const DataArrayPath& value);
  /**
   * @brief Getter property for FeaturePhasesArrayPath
   * @return Value of FeaturePhasesArrayPath
   */
  DataArrayPath getFeaturePhasesArrayPath() const;

  Q_PROPERTY(DataArrayPath FeaturePhasesArrayPath READ getFeaturePhasesArrayPath WRITE setFeaturePhasesArrayPath)

  /**
   * @brief Setter property for CrystalStructuresArrayPath
   */
  void setCrystalStructuresArrayPath(const DataArrayPath& value);
  /**
   * @brief Getter property for CrystalStructuresArrayPath
   * @return Value of CrystalStructuresArrayPath
   */
  DataArrayPath getCrystalStructuresArrayPath() const;

  Q_PROPERTY(DataArrayPath CrystalStructuresArrayPath READ getCrystalStructuresArrayPath WRITE setCrystalStructuresArrayPath)

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
   * @brief Setter property for AvgQuatsArrayPath
   */
  void setAvgQuatsArrayPath(const DataArrayPath& value);
  /**
   * @brief Getter property for AvgQuatsArrayPath
   * @return Value of AvgQuatsArrayPath
   */
  DataArrayPath getAvgQuatsArrayPath() const;

  Q_PROPERTY(DataArrayPath AvgQuatsArrayPath READ getAvgQuatsArrayPath WRITE setAvgQuatsArrayPath)

  /**
   * @brief Setter property for DirectionalModuliArrayName
   */
  void setDirectionalModuliArrayName(const QString& value);
  /**
   * @brief Getter property for DirectionalModuliArrayName
   * @return Value of DirectionalModuliArrayName
   */
  QString getDirectionalModuliArrayName() const;

  Q_PROPERTY(QString DirectionalModuliArrayName READ getDirectionalModuliArrayName WRITE setDirectionalModuliArrayName)

  /**
   * @brief Setter property for LoadingDirection
   */
  void setLoadingDirection(const FloatVec3Type& value);
  /**
   * @brief Getter property for LoadingDirection
   * @return Value of LoadingDirection
   */
  FloatVec3Type getLoadingDirection() const;

  Q_PROPERTY(FloatVec3Type LoadingDirection READ getLoadingDirection WRITE setLoadingDirection)

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
  FindDirectionalModuli();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck() override;

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  std::weak_ptr<DataArray<float>> m_DirectionalModuliPtr;
  float* m_DirectionalModuli = nullptr;
  std::weak_ptr<DataArray<int32_t>> m_FeaturePhasesPtr;
  int32_t* m_FeaturePhases = nullptr;
  std::weak_ptr<DataArray<float>> m_AvgQuatsPtr;
  float* m_AvgQuats = nullptr;
  std::weak_ptr<DataArray<unsigned int>> m_CrystalStructuresPtr;
  unsigned int* m_CrystalStructures = nullptr;
  std::weak_ptr<DataArray<float>> m_CrystalCompliancesPtr;
  float* m_CrystalCompliances = nullptr;

  DataArrayPath m_FeaturePhasesArrayPath = {};
  DataArrayPath m_CrystalStructuresArrayPath = {};
  DataArrayPath m_CrystalCompliancesArrayPath = {};
  DataArrayPath m_AvgQuatsArrayPath = {};
  QString m_DirectionalModuliArrayName = {};
  FloatVec3Type m_LoadingDirection = {};

public:
  FindDirectionalModuli(const FindDirectionalModuli&) = delete;            // Copy Constructor Not Implemented
  FindDirectionalModuli(FindDirectionalModuli&&) = delete;                 // Move Constructor Not Implemented
  FindDirectionalModuli& operator=(const FindDirectionalModuli&) = delete; // Copy Assignment Not Implemented
  FindDirectionalModuli& operator=(FindDirectionalModuli&&) = delete;      // Move Assignment Not Implemented
};
