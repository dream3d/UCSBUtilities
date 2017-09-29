# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                                               #
# Copyright (c) 2015 The Regents of the University of California, Author: William Lenthe        #
#                                                                                               #
# This program is free software: you can redistribute it and/or modify it under the terms of    #
# the GNU Lesser General Public License as published by the Free Software Foundation, either    #
# version 2 of the License, or (at your option) any later version.                              #
#                                                                                               #
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     #
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     #
# See the GNU Lesser General Public License for more details.                                   #
#                                                                                               #
# You should have received a copy of the GNU Lesser General Public License                      #
# along with this program.  If not, see <http://www.gnu.org/licenses/>.                         #
#                                                                                               #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

set(PLUGIN_NAME "UCSBUtilities")
set(${PLUGIN_NAME}Test_SOURCE_DIR ${${PLUGIN_NAME}_SOURCE_DIR}/Test/Test)
set(${PLUGIN_NAME}Test_BINARY_DIR ${${PLUGIN_NAME}_BINARY_DIR}/Test)


#------------------------------------------------------------------------------
# Include this file from the SIMPL Project
include(${SIMPLProj_SOURCE_DIR}/Source/SIMPLib/Testing/SIMPLTestFunctions.cmake)

include_directories(${${PLUGIN_NAME}_SOURCE_DIR}/Test)
include_directories(${${PLUGIN_NAME}_BINARY_DIR})

set(TEST_TEMP_DIR ${${PLUGIN_NAME}Test_BINARY_DIR}/Test/Temp)
# Make sure the directory is created during CMake time
file(MAKE_DIRECTORY ${TEST_TEMP_DIR})


configure_file(${${PLUGIN_NAME}_SOURCE_DIR}/Test/TestFileLocations.h.in
               ${${PLUGIN_NAME}_BINARY_DIR}/Test/${PLUGIN_NAME}TestFileLocations.h @ONLY IMMEDIATE)

set(${PLUGIN_NAME}_Link_Libs Qt5::Core H5Support SIMPLib)

AddSIMPLUnitTest(TESTNAME ${PLUGIN_NAME}UnitTest
                   SOURCES ${${PLUGIN_NAME}_SOURCE_DIR}/Test/UCSBUtilitiesFilterTest.cpp
                   FOLDER "${PLUGIN_NAME}Plugin/Test"
                   LINK_LIBRARIES ${${PLUGIN_NAME}_Link_Libs})

target_include_directories(${PLUGIN_NAME}UnitTest
                          PUBLIC ${PLUGINS_SOURCE_DIR}
                          PUBLIC ${PLUGINS_BINARY_DIR}
                          PUBLIC ${PLUGINS_BINARY_DIR}/${PLUGIN_NAME}/Test
)

if(MSVC)
  set_source_files_properties(${${PLUGIN_NAME}Test_BINARY_DIR}/${PLUGIN_NAME}UnitTest.cpp PROPERTIES COMPILE_FLAGS /bigobj)
endif()
