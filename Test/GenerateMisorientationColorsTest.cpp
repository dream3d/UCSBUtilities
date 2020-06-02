/* ============================================================================
 * Copyright (c) 2020 BlueQuartz Software, LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <random>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QString>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/DataContainers/DataContainer.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/FilterParameters/JsonFilterParametersReader.h"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/FilterPipeline.h"
#include "SIMPLib/Filtering/QMetaObjectUtilities.h"
#include "UnitTestSupport.hpp"

#include "UCSBUtilitiesFilters/util/UCSBHelpers.h"
#include "UCSBUtilitiesTestFileLocations.h"

class GenerateMisorientationColorsTest
{

public:
  GenerateMisorientationColorsTest() = default;
  ~GenerateMisorientationColorsTest() = default;

  const QString k_InputFile = QString("Small_IN100_Slice_1_Miso.png");
  const QString k_PipelineFileName = QString("MisorientationColors.json");
  const DataArrayPath k_MisorientationColorsPath = {"EBSD Scan", "Scan Data", "MisorientationColor"};

  const DataArrayPath k_ExemplarPath = {"Exemplar Data", "CellData", "Exemplar Image"};

  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
#if REMOVE_TEST_FILES

#endif
  }

  // -----------------------------------------------------------------------------
  int32_t TestMisorientationColors()
  {
    QString pipelineFile = UnitTest::PluginSourceDir + "/" + "ExamplePipelines/" + UnitTest::PluginName + "/" + k_PipelineFileName;

    // Sanity Check the filepath to make sure it exists, Report an error and bail if it does not
    QFileInfo fi(pipelineFile);
    if(!fi.exists())
    {
      std::cout << "The input file '" << pipelineFile.toStdString() << "' does not exist" << std::endl;
      return EXIT_FAILURE;
    }

    // Use the static method to read the Pipeline file and return a Filter Pipeline
    FilterPipeline::Pointer pipeline;
    JsonFilterParametersReader::Pointer jsonReader = JsonFilterParametersReader::New();
    pipeline = jsonReader->readPipelineFromFile(pipelineFile);

    if(nullptr == pipeline.get())
    {
      std::cout << "An error occurred trying to read the pipeline file. Exiting now." << std::endl;
      return EXIT_FAILURE;
    }

    // std::cout << "Pipeline Count: " << pipeline->size() << std::endl;

    FilterPipeline::FilterContainerType filters = pipeline->getFilterContainer();
    AbstractFilter::Pointer imageReader = filters[6];
    QVariant var = UnitTest::PluginTestDir + "/TestFiles/Small_IN100_Slice_1_Miso.png";
    bool didSet = imageReader->setProperty("FileName", var);
    DREAM3D_REQUIRE(didSet == true)

    //    Observer obs; // Create an Observer to report errors/progress from the executing pipeline
    //    pipeline->addMessageReceiver(&obs);
    // Preflight the pipeline
    int32_t err = pipeline->preflightPipeline();
    if(err < 0)
    {
      std::cout << "Errors preflighting the pipeline. Exiting Now." << std::endl;
    }
    DREAM3D_REQUIRE(err >= 0)

    // Now actually execute the pipeline
    DataContainerArray::Pointer dca = pipeline->execute();
    err = pipeline->getErrorCode();
    if(err < 0)
    {
      std::cout << "Error Condition of Pipeline: " << err << std::endl;
    }
    DREAM3D_REQUIRE(err >= 0)

    IDataArray::Pointer iMisoColors = dca->getPrereqIDataArrayFromPath(nullptr, k_MisorientationColorsPath);
    DREAM3D_REQUIRE_VALID_POINTER(iMisoColors);

    IDataArray::Pointer iExemplar = dca->getPrereqIDataArrayFromPath(nullptr, k_ExemplarPath);
    DREAM3D_REQUIRE_VALID_POINTER(iMisoColors);

    UInt8ArrayType::Pointer misoColors = std::dynamic_pointer_cast<UInt8ArrayType>(iMisoColors);
    DREAM3D_REQUIRE_VALID_POINTER(misoColors);

    UInt8ArrayType::Pointer exemplarColors = std::dynamic_pointer_cast<UInt8ArrayType>(iExemplar);
    DREAM3D_REQUIRE_VALID_POINTER(exemplarColors);

    // Compare all the elements
    size_t numElements = misoColors->size();
    for(size_t i = 0; i < numElements; i++)
    {
      //      if((*misoColors)[i] != (*exemplarColors)[i])
      //      {
      //        std::cout << "Mismatch of values at i=" << i << std::endl;
      //        std::cout << "Output Color Channel:" << static_cast<int32_t>((*misoColors)[i]) << std::endl;
      //        std::cout << "Exemplar Color Channel:" << static_cast<int32_t>((*exemplarColors)[i]) << std::endl;
      //      }
      DREAM3D_REQUIRED((*misoColors)[i], ==, (*exemplarColors)[i])
    }

    return 0;
  }

  // -----------------------------------------------------------------------------
  void operator()()
  {
    int err = EXIT_SUCCESS;
    DREAM3D_REGISTER_TEST(TestMisorientationColors())
    DREAM3D_REGISTER_TEST(RemoveTestFiles())
  }

public:
  GenerateMisorientationColorsTest(const GenerateMisorientationColorsTest&) = delete;            // Copy Constructor Not Implemented
  GenerateMisorientationColorsTest(GenerateMisorientationColorsTest&&) = delete;                 // Move Constructor Not Implemented
  GenerateMisorientationColorsTest& operator=(const GenerateMisorientationColorsTest&) = delete; // Copy Assignment Not Implemented
  GenerateMisorientationColorsTest& operator=(GenerateMisorientationColorsTest&&) = delete;      // Move Assignment Not Implemented
};
