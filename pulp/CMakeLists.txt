#
# Copyright (c) 2019 -2021 Chair of Electronic Design Automation, Technical University of Munich
#
# SPDX-License-Identifier: Apache-2.0
#

cmake_minimum_required(VERSION 3.12)

project(vpvper VERSION 1.0.0)

###############################################################################
# WORK-IN-PROGRESS: has to be adapted to VPV packaging. see cmake/ for helper scripts 
set(SystemCLanguage_DIR CACHE STRING "${SYSTEMC_PREFIX}/lib/cmake/SystemCLanguage")
find_package(SystemCLanguage CONFIG)

if (NOT TARGET SystemC::systemc)
  find_path(SYSTEMC_INCLUDE_DIR
    NAMES systemc
    PATHS ${SYSTEMC_PREFIX} ${SYSTEMC_HOME}
    PATH_SUFFIXES include
  )
  find_library(SYSTEMC_LIBRARY
    NAMES systemc
    PATHS ${SYSTEMC_PREFIX} ${SYSTEMC_HOME}
    PATH_SUFFIXES lib-linux64 lib
  )
  if(NOT SYSTEMC_LIBRARY)
    message(FATAL_ERROR
      "SystemC not found. Please set SYSTEMC_PREFIX to its install location"
    )
  endif()
  set(SYSTEMC_FOUND TRUE)
  set(SYSTEMC_INCLUDE_DIRS ${SYSTEMC_INCLUDE_DIR})
  set(SYSTEMC_LIBRARIES ${SYSTEMC_LIBRARY})
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(SystemC
    FOUND_VAR SYSTEMC_FOUND
    REQUIRED_VARS SYSTEMC_INCLUDE_DIRS SYSTEMC_LIBRARIES
  )

  if (NOT TARGET SystemC::systemc)
    add_library(SystemC::systemc UNKNOWN IMPORTED)
    set_target_properties(SystemC::systemc PROPERTIES
      IMPORTED_LOCATION "${SYSTEMC_LIBRARIES}"
      INTERFACE_INCLUDE_DIRECTORIES "${SYSTEMC_INCLUDE_DIRS}"
    )
  endif()
endif()
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD} CACHE STRING
   "C++ standard to build all targets. Supported values: std >= 14")
set (CMAKE_CXX_STANDARD_REQUIRED ${SystemC_CXX_STANDARD_REQUIRED} CACHE BOOL
   "The with CMAKE_CXX_STANDARD selected C++ standard is a requirement and has to be equal to SystemC's C++ standard.")
if(${CMAKE_CXX_STANDARD} LESS 14)
  message (FATAL_ERROR 
  "Minimum C++ standard: c++14 (configured: c++${CMAKE_CXX_STANDARD})")
endif()                                                             
                                                             # WORK-IN-PROGRESS
###############################################################################


###############################################################################
# Pulp
###############################################################################
find_package(Boost 1.70 REQUIRED 
  COMPONENTS filesystem iostreams
)
add_library(${PROJECT_NAME}_pulp 
  experimental/external_uart_device.cpp)
target_include_directories(${PROJECT_NAME}_pulp INTERFACE 
  experimental
)
target_link_libraries(${PROJECT_NAME}_pulp PUBLIC 
  scc
  SystemC::systemc
  ${Boost_LIBRARIES}
)
set_target_properties(${PROJECT_NAME}_pulp PROPERTIES
  VERSION ${PROJECT_VERSION}
  FRAMEWORK FALSE
)
