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

#include "EbsdLib/LaueOps/CubicOps.h"
#include "EbsdLib/LaueOps/HexagonalOps.h"
#include "EbsdLib/LaueOps/LaueOps.h"
#include "EbsdLib/LaueOps/OrthoRhombicOps.h"
#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/DataArrays/IDataArray.h"
#include "SIMPLib/DataContainers/DataContainer.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/DataArrays/DataArray.hpp"

#include "UCSBUtilities/UCSBUtilitiesDLLExport.h"

class UCSBUtilities_EXPORT FindModulusMismatch : public AbstractFilter
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(FindModulusMismatch SUPERCLASS AbstractFilter)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(FindModulusMismatch)
  PYB11_FILTER_NEW_MACRO(FindModulusMismatch)
  PYB11_PROPERTY(DataArrayPath ModuliArrayPath READ getModuliArrayPath WRITE setModuliArrayPath)
  PYB11_PROPERTY(DataArrayPath SurfaceMeshFaceLabelsArrayPath READ getSurfaceMeshFaceLabelsArrayPath WRITE setSurfaceMeshFaceLabelsArrayPath)
  PYB11_PROPERTY(QString SurfaceMeshDeltaModulusArrayName READ getSurfaceMeshDeltaModulusArrayName WRITE setSurfaceMeshDeltaModulusArrayName)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = FindModulusMismatch;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<FindModulusMismatch> New();

  /**
   * @brief Returns the name of the class for FindModulusMismatch
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for FindModulusMismatch
   */
  static QString ClassName();

  ~FindModulusMismatch() override;

  /**
   * @brief Setter property for ModuliArrayPath
   */
  void setModuliArrayPath(const DataArrayPath& value);
  /**
   * @brief Getter property for ModuliArrayPath
   * @return Value of ModuliArrayPath
   */
  DataArrayPath getModuliArrayPath() const;

  Q_PROPERTY(DataArrayPath ModuliArrayPath READ getModuliArrayPath WRITE setModuliArrayPath)

  /**
   * @brief Setter property for SurfaceMeshFaceLabelsArrayPath
   */
  void setSurfaceMeshFaceLabelsArrayPath(const DataArrayPath& value);
  /**
   * @brief Getter property for SurfaceMeshFaceLabelsArrayPath
   * @return Value of SurfaceMeshFaceLabelsArrayPath
   */
  DataArrayPath getSurfaceMeshFaceLabelsArrayPath() const;

  Q_PROPERTY(DataArrayPath SurfaceMeshFaceLabelsArrayPath READ getSurfaceMeshFaceLabelsArrayPath WRITE setSurfaceMeshFaceLabelsArrayPath)

  /**
   * @brief Setter property for SurfaceMeshDeltaModulusArrayName
   */
  void setSurfaceMeshDeltaModulusArrayName(const QString& value);
  /**
   * @brief Getter property for SurfaceMeshDeltaModulusArrayName
   * @return Value of SurfaceMeshDeltaModulusArrayName
   */
  QString getSurfaceMeshDeltaModulusArrayName() const;

  Q_PROPERTY(QString SurfaceMeshDeltaModulusArrayName READ getSurfaceMeshDeltaModulusArrayName WRITE setSurfaceMeshDeltaModulusArrayName)

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
  FindModulusMismatch();

  /**
   * @brief dataCheck dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck() override;

private:
  std::weak_ptr<DataArray<float>> m_ModuliPtr;
  float* m_Moduli = nullptr;
  std::weak_ptr<DataArray<int32_t>> m_SurfaceMeshFaceLabelsPtr;
  int32_t* m_SurfaceMeshFaceLabels = nullptr;
  std::weak_ptr<DataArray<float>> m_SurfaceMeshDeltaModulusPtr;
  float* m_SurfaceMeshDeltaModulus = nullptr;

  DataArrayPath m_ModuliArrayPath = {};
  DataArrayPath m_SurfaceMeshFaceLabelsArrayPath = {};
  QString m_SurfaceMeshDeltaModulusArrayName = {};

  CubicOps::Pointer m_CubicOps;
  HexagonalOps::Pointer m_HexOps;
  OrthoRhombicOps::Pointer m_OrthoOps;

  /**
   * @brief dataCheckVoxel dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheckVoxel();

  /**
   * @brief dataCheckSurfaceMesh dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheckSurfaceMesh();

public:
  FindModulusMismatch(const FindModulusMismatch&) = delete;            // Copy Constructor Not Implemented
  FindModulusMismatch(FindModulusMismatch&&) = delete;                 // Move Constructor Not Implemented
  FindModulusMismatch& operator=(const FindModulusMismatch&) = delete; // Copy Assignment Not Implemented
  FindModulusMismatch& operator=(FindModulusMismatch&&) = delete;      // Move Assignment Not Implemented
};
