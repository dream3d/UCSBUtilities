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

#include <QtCore/QVector>

#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/DataArrays/NeighborList.hpp"
#include "SIMPLib/DataArrays/StatsDataArray.h"
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/DataArrays/StructArray.hpp"
#include "SIMPLib/SIMPLib.h"

class IDataArray;
using IDataArrayShPtrType = std::shared_ptr<IDataArray>;

#include "EbsdLib/Utilities/ModifiedLambertProjectionArray.h"

class UCSBHelpers
{
public:
  UCSBHelpers();
  virtual ~UCSBHelpers();

  template <typename T>
  static IDataArray::Pointer ReorderCopy(typename DataArray<T>::Pointer array, std::vector<size_t> newOrderMap)
  {
    if(newOrderMap.size() != static_cast<std::vector<size_t>::size_type>(array->getNumberOfTuples()))
    {
      return IDataArray::NullPointer();
    }
    IDataArrayShPtrType daCopy = array->createNewArray(array->getNumberOfTuples(), array->getComponentDimensions(), array->getName(), array->isAllocated());
    if(array->isAllocated() == true)
    {
      daCopy->initializeWithZeros();
      size_t chunkSize = array->getNumberOfComponents() * sizeof(T);
      for(size_t i = 0; i < array->getNumberOfTuples(); i++)
      {
        T* src = array->getPointer(i * array->getNumberOfComponents());
        void* dest = daCopy->getVoidPointer(newOrderMap[i] * array->getNumberOfComponents());
        ::memcpy(dest, src, chunkSize);
      }
    }
    return daCopy;
  }

  /**
   * @brief reorderCopy
   * @param newOrderMap
   * @return
   */
  template <typename T>
  static IDataArrayShPtrType ReorderCopy(typename NeighborList<T>::Pointer array, std::vector<size_t> newOrderMap)
  {
    size_t newOrderMapSize = static_cast<size_t>(newOrderMap.size());
    if(newOrderMapSize != array->getNumberOfTuples())
    {
      return IDataArray::NullPointer();
    }

    typename NeighborList<T>::Pointer daCopyPtr = NeighborList<T>::CreateArray(array->getNumberOfTuples(), "Copy of NeighborList", true);
    daCopyPtr->initializeWithZeros();
    size_t numTuples = array->getNumberOfTuples();
    for(size_t i = 0; i < numTuples; i++)
    {
      typename NeighborList<T>::SharedVectorType sharedNeiLst; //(new std::vector<T>);
      sharedNeiLst = array->getList(i);
      daCopyPtr->setList(newOrderMap[i], sharedNeiLst);
    }

    return daCopyPtr;
  }

  /**
   * @brief reorderCopy
   * @param newOrderMap
   * @return
   */
  static IDataArrayShPtrType ReorderCopy(StatsDataArray::Pointer array, std::vector<size_t> newOrderMap);

  /**
   * @brief ReorderCopy
   * @param array
   * @param newOrderMap
   * @return
   */
  static IDataArrayShPtrType ReorderCopy(StringDataArray::Pointer array, std::vector<size_t> newOrderMap);

  /**
   * @brief reorderCopy
   * @param newOrderMap
   * @return
   */
  template <typename T>
  static IDataArrayShPtrType ReorderCopy(typename StructArray<T>::Pointer array, std::vector<size_t> newOrderMap)
  {
    if(newOrderMap.size() != static_cast<std::vector<size_t>::size_type>(array->getNumberOfTuples()))
    {
      return IDataArray::NullPointer();
    }
    IDataArrayShPtrType daCopy = createNewArray(array->getNumberOfTuples(), array->getComponentDimensions(), array->getName(), array->isAllocated());
    if(array->isAllocated() == true)
    {
      daCopy->initializeWithZeros();
      size_t chunkSize = array->getNumberOfComponents() * sizeof(T);
      for(size_t i = 0; i < array->getNumberOfTuples(); i++)
      {
        T* src = getPointer(i * array->getNumberOfComponents());
        void* dest = daCopy->getVoidPointer(newOrderMap[i] * array->getNumberOfComponents());
        ::memcpy(dest, src, chunkSize);
      }
    }
    return daCopy;
  }

#if 0
    /**
     * @brief reorderCopy
     * @param newOrderMap
     * @return
     */
    static IDataArrayShPtrType ReorderCopy(ModifiedLambertProjectionArray::Pointer array, std::vector<size_t> newOrderMap);
#endif
  /**
   * @brief computes linear regression
   * @param fit slope
   * @param fit intercept
   * @param x values to fit
   * @param y values to fit
   * @return true if no errors encounterd in regression, false otherwise
   */
  template <typename T>
  static bool linearRegression(double& slope, double& intercept, typename std::vector<T>& x, typename std::vector<T>& y)
  {
    // make sure x and y are same length
    size_t count = x.size();
    if(count != y.size())
    {
      slope = 0;
      intercept = 0;
      return false;
    }

    // compute sums
    double sumX = 0, sumY = 0, sumX2 = 0, sumXY = 0;
    for(size_t i = 0; i < count; i++)
    {
      sumX += static_cast<double>(x[i]);
      sumY += static_cast<double>(y[i]);
      sumX2 += static_cast<double>(x[i] * x[i]);
      sumXY += static_cast<double>(x[i] * y[i]);
    }

    // fit line
    slope = (static_cast<double>(count) * sumXY - sumX * sumY) / (static_cast<double>(count) * sumX2 - sumX * sumX);
    intercept = (sumY - slope * sumX) / static_cast<double>(count);
    return true;
  }

  static void SubtractBackground(std::vector<int64_t>& xshifts, std::vector<int64_t>& yshifts, int64_t dims[3]);

protected:
private:
};
