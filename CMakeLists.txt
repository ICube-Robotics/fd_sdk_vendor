# Copyright 2023 ICUBE Laboratory, University of Strasbourg
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Author: Thibault Poignonec (thibault.poignonec@gmail.com)

cmake_minimum_required(VERSION 3.11)
project(fd_sdk_vendor)
set(FD_SDK_VERSION "3.16.0")

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17) # Default to C++17
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
include(FetchContent)

# Downdload SDK binaries
#------------------------
fetchcontent_declare(
  fd_sdk_binaries
  URL      https://www.forcedimension.com/downloads/sdk/sdk-${FD_SDK_VERSION}-linux-x86_64-gcc.tar.gz
)
fetchcontent_makeavailable(fd_sdk_binaries)

# Set DHD and DRD library paths
#-------------------------------
string(REPLACE "." ";" VERSION_SDK_LIST ${FD_SDK_VERSION})
list(GET VERSION_SDK_LIST 0 VERSION_SDK_MAJOR)

set(
  dhd_LIBS
  ${fd_sdk_binaries_SOURCE_DIR}/lib/release/lin-x86_64-gcc/libdhd.so.${FD_SDK_VERSION}
)
set(
  drd_LIBS
  ${fd_sdk_binaries_SOURCE_DIR}/lib/release/lin-x86_64-gcc/libdrd.so.${FD_SDK_VERSION}
)

# Setup vendor library
#----------------------
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${fd_sdk_binaries_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(
  ${PROJECT_NAME} INTERFACE
  ${CMAKE_INSTALL_PREFIX}/lib/libdhd.so.${VERSION_SDK_MAJOR}
  ${CMAKE_INSTALL_PREFIX}/lib/libdrd.so.${VERSION_SDK_MAJOR}
  usb-1.0
)

# Testing
#----------
if(BUILD_TESTING)
  find_package(ament_cmake_test REQUIRED)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)

  ament_lint_auto_find_test_dependencies()

  #[==[
  add_library(fd_sdk_libraries INTERFACE)
  target_include_directories(fd_sdk_libraries INTERFACE "${CMAKE_INSTALL_PREFIX}/include")
  target_link_directories(fd_sdk_libraries INTERFACE "${CMAKE_INSTALL_PREFIX}/lib")
  get_target_property(LINKED_LIBS ${PROJECT_NAME} LINK_LIBRARIES)
  target_link_libraries(
    fd_sdk_libraries
    INTERFACE
    ${LINKED_LIBS}
  )
  add_dependencies(fd_sdk_libraries ${PROJECT_NAME})
  #]==]
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(
    test_dhd
    test/test_dhd.cpp
    test/test_drd.cpp
  )
  target_link_libraries(test_dhd ${PROJECT_NAME})
  target_include_directories(
    test_dhd
    PRIVATE
    "${CMAKE_INSTALL_PREFIX}/include"
  )

endif()

# Install
#----------

install(
  DIRECTORY ${fd_sdk_binaries_SOURCE_DIR}/include
  DESTINATION .
)
install(
  DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
)

install(
  FILES ${dhd_LIBS}
  DESTINATION lib
  RENAME libdhd.so.${VERSION_SDK_MAJOR}
)
install(
  FILES ${drd_LIBS}
  DESTINATION lib
  RENAME libdrd.so.${VERSION_SDK_MAJOR}
)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)

ament_package()
