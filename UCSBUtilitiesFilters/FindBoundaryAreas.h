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

#include <QtCore/QString>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/DataArrays/IDataArray.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/DataArrays/DataArray.hpp"

#include "UCSBUtilities/UCSBUtilitiesDLLExport.h"

/**
 * @class FindBoundaryAreas FindBoundaryAreas.h /FilterCategoryFilters/FindBoundaryAreas.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class UCSBUtilities_EXPORT FindBoundaryAreas : public AbstractFilter
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(FindBoundaryAreas SUPERCLASS AbstractFilter)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(FindBoundaryAreas)
  PYB11_FILTER_NEW_MACRO(FindBoundaryAreas)
  PYB11_PROPERTY(DataArrayPath SurfaceMeshTriangleAreasArrayPath READ getSurfaceMeshTriangleAreasArrayPath WRITE setSurfaceMeshTriangleAreasArrayPath)
  PYB11_PROPERTY(DataArrayPath SurfaceMeshFeatureFaceIdsArrayPath READ getSurfaceMeshFeatureFaceIdsArrayPath WRITE setSurfaceMeshFeatureFaceIdsArrayPath)
  PYB11_PROPERTY(DataArrayPath SurfaceMeshBoundaryAreasArrayPath READ getSurfaceMeshBoundaryAreasArrayPath WRITE setSurfaceMeshBoundaryAreasArrayPath)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = FindBoundaryAreas;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<FindBoundaryAreas> New();

  /**
   * @brief Returns the name of the class for FindBoundaryAreas
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for FindBoundaryAreas
   */
  static QString ClassName();

  ~FindBoundaryAreas() override;

  // required arrays
  /**
   * @brief Setter property for SurfaceMeshTriangleAreasArrayPath
   */
  void setSurfaceMeshTriangleAreasArrayPath(const DataArrayPath& value);
  /**
   * @brief Getter property for SurfaceMeshTriangleAreasArrayPath
   * @return Value of SurfaceMeshTriangleAreasArrayPath
   */
  DataArrayPath getSurfaceMeshTriangleAreasArrayPath() const;

  Q_PROPERTY(DataArrayPath SurfaceMeshTriangleAreasArrayPath READ getSurfaceMeshTriangleAreasArrayPath WRITE setSurfaceMeshTriangleAreasArrayPath)

  /**
   * @brief Setter property for SurfaceMeshFeatureFaceIdsArrayPath
   */
  void setSurfaceMeshFeatureFaceIdsArrayPath(const DataArrayPath& value);
  /**
   * @brief Getter property for SurfaceMeshFeatureFaceIdsArrayPath
   * @return Value of SurfaceMeshFeatureFaceIdsArrayPath
   */
  DataArrayPath getSurfaceMeshFeatureFaceIdsArrayPath() const;

  Q_PROPERTY(DataArrayPath SurfaceMeshFeatureFaceIdsArrayPath READ getSurfaceMeshFeatureFaceIdsArrayPath WRITE setSurfaceMeshFeatureFaceIdsArrayPath)

  // created array path
  /**
   * @brief Setter property for SurfaceMeshBoundaryAreasArrayPath
   */
  void setSurfaceMeshBoundaryAreasArrayPath(const DataArrayPath& value);
  /**
   * @brief Getter property for SurfaceMeshBoundaryAreasArrayPath
   * @return Value of SurfaceMeshBoundaryAreasArrayPath
   */
  DataArrayPath getSurfaceMeshBoundaryAreasArrayPath() const;

  Q_PROPERTY(DataArrayPath SurfaceMeshBoundaryAreasArrayPath READ getSurfaceMeshBoundaryAreasArrayPath WRITE setSurfaceMeshBoundaryAreasArrayPath)

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
  FindBoundaryAreas();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck() override;

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  std::weak_ptr<DataArray<double>> m_SurfaceMeshTriangleAreasPtr;
  double* m_SurfaceMeshTriangleAreas = nullptr;
  std::weak_ptr<DataArray<int32_t>> m_SurfaceMeshFeatureFaceIdsPtr;
  int32_t* m_SurfaceMeshFeatureFaceIds = nullptr;
  std::weak_ptr<DataArray<double>> m_SurfaceMeshBoundaryAreasPtr;
  double* m_SurfaceMeshBoundaryAreas = nullptr;

  DataArrayPath m_SurfaceMeshTriangleAreasArrayPath = {};
  DataArrayPath m_SurfaceMeshFeatureFaceIdsArrayPath = {};
  DataArrayPath m_SurfaceMeshBoundaryAreasArrayPath = {};

public:
  FindBoundaryAreas(const FindBoundaryAreas&) = delete;            // Copy Constructor Not Implemented
  FindBoundaryAreas(FindBoundaryAreas&&) = delete;                 // Move Constructor Not Implemented
  FindBoundaryAreas& operator=(const FindBoundaryAreas&) = delete; // Copy Assignment Not Implemented
  FindBoundaryAreas& operator=(FindBoundaryAreas&&) = delete;      // Move Assignment Not Implemented
};
