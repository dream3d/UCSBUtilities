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

#include "UCSBHelpers.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
UCSBHelpers::UCSBHelpers() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
UCSBHelpers::~UCSBHelpers() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataArray::Pointer UCSBHelpers::ReorderCopy(StatsDataArray::Pointer array, std::vector<size_t> newOrderMap)
{
  if(static_cast<size_t>(newOrderMap.size()) != array->getNumberOfTuples())
  {
    return IDataArray::Pointer();
  }
  StatsDataArray::Pointer daCopyPtr = StatsDataArray::New();
  daCopyPtr->resizeTuples(array->getNumberOfTuples());
  daCopyPtr->initializeWithZeros();
  StatsDataArray& daCopy = *daCopyPtr;
  for(size_t i = 0; i < array->getNumberOfTuples(); i++)
  {
    daCopy[newOrderMap[i]] = array->getStatsData(i);
  }

  return daCopyPtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataArray::Pointer UCSBHelpers::ReorderCopy(StringDataArray::Pointer array, std::vector<size_t> newOrderMap)
{
  if(static_cast<size_t>(newOrderMap.size()) != array->getNumberOfTuples())
  {
    return IDataArray::NullPointer();
  }
  StringDataArray::Pointer daCopy = StringDataArray::CreateArray(array->getNumberOfTuples(), array->getName(), true);
  daCopy->initializeWithZeros();
  for(QVector<QString>::size_type i = 0; i < array->getNumberOfTuples(); ++i)
  {
    daCopy->setValue(newOrderMap[i], array->getValue(i));
  }
  return daCopy;
}

#if 0
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataArray::Pointer UCSBHelpers::ReorderCopy(ModifiedLambertProjectionArray::Pointer array, std::vector<size_t> newOrderMap)
{
  if( static_cast<size_t>(newOrderMap.size()) != array->getNumberOfTuples())
  {
    return IDataArray::NullPointer();
  }
  ModifiedLambertProjectionArray::Pointer daCopyPtr = ModifiedLambertProjectionArray::New();
  daCopyPtr->resizeTuples(array->getNumberOfTuples());
  daCopyPtr->initializeWithZeros();
  ModifiedLambertProjectionArray& daCopy = *daCopyPtr;
  for(size_t i = 0; i < array->getNumberOfTuples(); i++)
  {
    daCopy[newOrderMap[i]] = array->getModifiedLambertProjection(i);
  }

  return daCopyPtr;
}
#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UCSBHelpers::SubtractBackground(std::vector<int64_t>& xshifts, std::vector<int64_t>& yshifts, int64_t dims[3])
{
  int64_t slice = 0;

  /**fit x and y shifts to lines
   *
   * y = mx + b
   *
   * m = (n*sum(x_i * y_i) - sum(x_i) * sum(y_i)) / (n*sum(x_i^2)-sum(x_i)^2
   *
   * b = (sum(y_i)-m*sum(x_i))/n
   *
   */

  // same for both
  double sumX = 0.0;   // sum(x_i)
  double sumX_2 = 0.0; // sum(x_i^2)

  // x shift line
  double x_sumY = 0.0;  // sum(y_i)
  double x_sumXY = 0.0; // sum(x_i * y_i)

  // y shift line
  double y_sumY = 0.0;  // sum(y_i)
  double y_sumXY = 0.0; // sum(x_i * y_i)

  for(int64_t iter = 0; iter < dims[2]; iter++)
  {
    slice = static_cast<int64_t>((dims[2] - 1) - iter);
    sumX = static_cast<double>(sumX + iter);
    sumX_2 = static_cast<double>(sumX_2 + iter * iter);
    x_sumY = static_cast<double>(x_sumY + xshifts[iter]);
    x_sumXY = static_cast<double>(x_sumXY + iter * xshifts[iter]);
    y_sumY = static_cast<double>(y_sumY + yshifts[iter]);
    y_sumXY = static_cast<double>(y_sumXY + iter * yshifts[iter]);
  }

  double mx = static_cast<double>((dims[2] * x_sumXY - x_sumXY) / (dims[2] * sumX_2 - sumX));
  double my = static_cast<double>((dims[2] * y_sumXY - y_sumXY) / (dims[2] * sumX_2 - sumX));

  // adjust shifts so that fit line has 0 slope (~ends of the sample are fixed)
  for(int64_t iter = 1; iter < dims[2]; iter++)
  {
    slice = (dims[2] - 1) - iter;
    xshifts[iter] = static_cast<int64_t>(xshifts[iter] - iter * mx);
    yshifts[iter] = static_cast<int64_t>(yshifts[iter] - iter * my);
  }
}
