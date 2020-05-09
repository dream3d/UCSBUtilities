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

#include <random>

#include <QtCore/QFile>

#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/FilterPipeline.h"
#include "SIMPLib/Filtering/QMetaObjectUtilities.h"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"
#include "SIMPLib/SIMPLib.h"
#include "UnitTestSupport.hpp"

#include "UCSBUtilitiesFilters/util/UCSBHelpers.h"

#include "UCSBUtilitiesTestFileLocations.h"

class UCSBUtilitiesFilterTest
{

public:
  UCSBUtilitiesFilterTest() = default;
  ~UCSBUtilitiesFilterTest() = default;

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
#if REMOVE_TEST_FILES
    QFile::remove(UnitTest::UCSBUtilitiesFilterTest::TestFile1);
    QFile::remove(UnitTest::UCSBUtilitiesFilterTest::TestFile2);
#endif
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  template <typename T>
  void __TestReorderCopy()
  {
    size_t numTuples = 10;
    std::vector<size_t> cDims(1, 5);
    QString name("Source Array");

    // make sure that an incorrectly sized order returns a null pointer
    typename DataArray<T>::Pointer src = DataArray<T>::CreateArray(numTuples, cDims, name, false);
    std::vector<size_t> wrongSize(numTuples + 1);
    typename DataArray<T>::Pointer copy = std::dynamic_pointer_cast<DataArray<T>>(UCSBHelpers::ReorderCopy<T>(src, wrongSize));
    DREAM3D_REQUIRE_EQUAL(copy.get(), 0)

    // fill reorder vector with same index (using this would be the same result as deep copy)
    std::vector<size_t> newOrder(numTuples);
    for(size_t i = 0; i < numTuples; i++)
    {
      newOrder[i] = i;
    }

    // shuffle order
    std::random_device rng;
    std::mt19937 urng(rng());
    std::shuffle(newOrder.begin(), newOrder.end(), urng);

    // First lets try it without allocating any memory
    copy = std::dynamic_pointer_cast<DataArray<T>>(UCSBHelpers::ReorderCopy<T>(src, newOrder));

    // if newOrder is inporperly size a null pointer is returned
    DREAM3D_REQUIRE_NE(copy.get(), 0)

    // check sizes
    DREAM3D_REQUIRED(copy->getNumberOfTuples(), ==, src->getNumberOfTuples());
    DREAM3D_REQUIRED(copy->isAllocated(), ==, src->isAllocated());

    // Create the array again, this time allocating the data and putting in some known data
    src = DataArray<T>::CreateArray(numTuples, cDims, name, true);
    for(size_t i = 0; i < src->getSize(); i++)
    {
      src->setValue(i, i);
    }

    for(size_t i = 0; i < numTuples; i++)
    {
      for(size_t j = 0; j < cDims[0]; j++)
      {
        src->setComponent(i, j, static_cast<T>(i * cDims[0] + j));
      }
    }
    copy = std::dynamic_pointer_cast<DataArray<T>>(UCSBHelpers::ReorderCopy<T>(src, newOrder));
    for(size_t i = 0; i < numTuples; i++)
    {
      for(size_t j = 0; j < cDims[0]; j++)
      {
        T cpy = copy->getComponent(newOrder[i], j);
        T val = src->getComponent(i, j);
        DREAM3D_REQUIRE_EQUAL(cpy, val)
      }
    }
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestReorderCopy()
  {
    __TestReorderCopy<int8_t>();
    __TestReorderCopy<uint8_t>();
    __TestReorderCopy<int16_t>();
    __TestReorderCopy<uint16_t>();
    __TestReorderCopy<int32_t>();
    __TestReorderCopy<uint32_t>();
    __TestReorderCopy<int64_t>();
    __TestReorderCopy<uint64_t>();
    __TestReorderCopy<float>();
    __TestReorderCopy<double>();
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    int err = EXIT_SUCCESS;
    DREAM3D_REGISTER_TEST(TestReorderCopy())
    DREAM3D_REGISTER_TEST(RemoveTestFiles())
  }

private:
  UCSBUtilitiesFilterTest(const UCSBUtilitiesFilterTest&); // Copy Constructor Not Implemented
  void operator=(const UCSBUtilitiesFilterTest&);          // Move assignment Not Implemented
};
