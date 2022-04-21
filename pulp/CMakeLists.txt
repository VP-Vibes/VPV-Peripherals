#
# Copyright (c) 2019 -2021 MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)

###############################################################################
# Pulp
###############################################################################
FILE(GLOB PulpinoHeaders ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/experimental/*.h)
if(PULP_EXPERIMENTAL)
  add_library(${PROJECT_NAME}_pulpino
    experimental/external_uart_device.cpp
  )
  target_include_directories(${PROJECT_NAME}_pulpino PUBLIC ${CMAKE_CURRENT_LIST_DIR})
  target_link_libraries(${PROJECT_NAME}_pulpino PUBLIC scc)
  target_link_libraries(${PROJECT_NAME}_pulpino PRIVATE Boost::filesystem Boost::iostreams)
  set_target_properties(${PROJECT_NAME}_pulpino PROPERTIES
  	VERSION ${PROJECT_VERSION}
  	FRAMEWORK FALSE
  	PUBLIC_HEADER "${PulpHeaders}" # specify the public headers
  )
else()
  add_library(${PROJECT_NAME}_pulpino INTERFACE)
  target_include_directories(${PROJECT_NAME}_pulpino INTERFACE ${vpvper_SOURCE_DIR})
  target_link_libraries(${PROJECT_NAME}_pulpino INTERFACE scc)
  set_target_properties(${PROJECT_NAME}_pulpino PROPERTIES
    PUBLIC_HEADER "${PulpinoHeaders}" # specify the public headers
  )
endif()

set_target_properties(${PROJECT_NAME}_pulpino PROPERTIES
  VERSION ${PROJECT_VERSION}
  FRAMEWORK FALSE
  PUBLIC_HEADER "${PulpinoHeaders}" # specify the public headers
)
