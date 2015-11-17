


#include <QtCore/QCoreApplication>
#include <QtCore/QFile>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Common/FilterPipeline.h"
#include "SIMPLib/Common/FilterManager.h"
#include "SIMPLib/Common/FilterFactory.hpp"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"
#include "SIMPLib/Utilities/UnitTestSupport.hpp"
#include "SIMPLib/Utilities/QMetaObjectUtilities.h"

#include "UCSBUtilitiesFilters/util/UCSBHelpers.h"

#include "UCSBUtilitiesTestFileLocations.h"

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
template<typename T>
void __TestReorderCopy()
{
  size_t numTuples = 10;
  QVector<size_t> cDims(1, 5);
  QString name("Source Array");

  //make sure that an incorrectly sized order returns a null pointer
  typename DataArray<T>::Pointer src = DataArray<T>::CreateArray(numTuples, cDims, name, false);
  QVector<size_t> wrongSize(numTuples + 1);
  typename DataArray<T>::Pointer copy = boost::dynamic_pointer_cast<DataArray<T> >(UCSBHelpers::ReorderCopy<T>(src,wrongSize));
  DREAM3D_REQUIRE_EQUAL(copy.get(), 0);

  // fill reorder vector with same index (using this would be the same result as deep copy)
  QVector<size_t> newOrder(numTuples);
  for(size_t i = 0; i < numTuples; i++)
  {
    newOrder[i] = i;
  }

  //shuffle order
  std::random_shuffle(newOrder.begin(), newOrder.end());

  // First lets try it without allocating any memory
  copy = boost::dynamic_pointer_cast<DataArray<T> >(UCSBHelpers::ReorderCopy<T>(src, newOrder));

  //if newOrder is inporperly size a null pointer is returned
  DREAM3D_REQUIRE_NE(copy.get(), 0);

  //check sizes
  DREAM3D_REQUIRED(copy->getNumberOfTuples(), ==, src->getNumberOfTuples() );
  DREAM3D_REQUIRED(copy->isAllocated(), ==, src->isAllocated() );

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
      src->setComponent(i, j, static_cast<T>(i * cDims[0] + j) );
    }
  }
  copy = boost::dynamic_pointer_cast<DataArray<T> >(UCSBHelpers::ReorderCopy<T>(src, newOrder));
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
int TestFilterAvailability()
{
  // Now instantiate the UCSBUtilitiesFilter Filter from the FilterManager
  QString filtName = "UCSBUtilitiesFilter";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
  if (NULL == filterFactory.get())
  {
    std::stringstream ss;
    ss << "The UCSBUtilitiesFilterTest Requires the use of the " << filtName.toStdString() << " filter which is found in the UCSBUtilities Plugin";
    DREAM3D_TEST_THROW_EXCEPTION(ss.str())
  }
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int UCSBUtilitiesFilterTest()
{
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 /* Please write UCSBUtilitiesFilter test code here.
  *
  * Your IO test files are:
  * UnitTest::UCSBUtilitiesFilterTest::TestFile1
  * UnitTest::UCSBUtilitiesFilterTest::TestFile2
  *
  * SIMPLib provides some macros that will throw exceptions when a test fails
  * and thus report that during testing. These macros are located in the
  * SIMPLib/Utilities/UnitTestSupport.hpp file. Some examples are:
  *
  * SIMPLib_REQUIRE_EQUAL(foo, 0)
  * This means that if the variable foo is NOT equal to Zero then test will fail
  * and the current test will exit immediately. If there are more tests registered
  * with the SIMPLib_REGISTER_TEST() macro, the next test will execute. There are
  * lots of examples in the SIMPLib/Test folder to look at.
  */
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  int foo = 0;
  DREAM3D_REQUIRE_EQUAL(foo, 0)

  return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loadFilterPlugins()
{
  // Register all the filters including trying to load those from Plugins
  FilterManager* fm = FilterManager::Instance();
  SIMPLibPluginLoader::LoadPluginFilters(fm);

  // Send progress messages from PipelineBuilder to this object for display
  QMetaObjectUtilities::RegisterMetaTypes();
}


// -----------------------------------------------------------------------------
//  Use test framework
// -----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // Instantiate the QCoreApplication that we need to get the current path and load plugins.
  QCoreApplication app(argc, argv);
  QCoreApplication::setOrganizationName("");
  QCoreApplication::setOrganizationDomain("");
  QCoreApplication::setApplicationName("UCSBUtilitiesFilterTest");

  int err = EXIT_SUCCESS;
  DREAM3D_REGISTER_TEST( loadFilterPlugins() );
  DREAM3D_REGISTER_TEST( TestFilterAvailability() );

  DREAM3D_REGISTER_TEST( UCSBUtilitiesFilterTest() )
  DREAM3D_REGISTER_TEST( TestReorderCopy() )
  DREAM3D_REGISTER_TEST( RemoveTestFiles() )
  PRINT_TEST_SUMMARY();
  return err;
}
