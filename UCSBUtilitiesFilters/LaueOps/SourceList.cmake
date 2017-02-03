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

set(OrientationLib_SpaceGroupOps_HDRS
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/SpaceGroupOps.h
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/CubicOps.h
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/CubicLowOps.h
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/HexagonalOps.h
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/HexagonalLowOps.h
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/OrthoRhombicOps.h
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/TrigonalOps.h
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/TrigonalLowOps.h
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/TetragonalOps.h
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/TetragonalLowOps.h
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/TriclinicOps.h
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/MonoclinicOps.h
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/SO3Sampler.h
)
set(OrientationLib_SpaceGroupOps_SRCS
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/SpaceGroupOps.cpp
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/CubicOps.cpp
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/CubicLowOps.cpp
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/HexagonalOps.cpp
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/HexagonalLowOps.cpp
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/OrthoRhombicOps.cpp
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/TrigonalOps.cpp
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/TrigonalLowOps.cpp
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/TetragonalOps.cpp
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/TetragonalLowOps.cpp
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/TriclinicOps.cpp
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/MonoclinicOps.cpp
  ${OrientationLib_SOURCE_DIR}/SpaceGroupOps/SO3Sampler.cpp
)
cmp_IDE_SOURCE_PROPERTIES( "SpaceGroupOps" "${OrientationLib_SpaceGroupOps_HDRS}" "${OrientationLib_SpaceGroupOps_SRCS}" "0")
if( ${PROJECT_INSTALL_HEADERS} EQUAL 1 )
    INSTALL (FILES ${OrientationLib_SpaceGroupOps_HDRS}
            DESTINATION include/OrientationLib/SpaceGroupOps
            COMPONENT Headers   )
endif()
