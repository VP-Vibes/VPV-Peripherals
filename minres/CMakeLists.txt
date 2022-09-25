#
# Copyright (c) 2019 -2021 MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)

add_library(${PROJECT_NAME}_minres
	gpio.cpp
	presc.cpp
	tim.cpp
	qspi.cpp
	uart_terminal.cpp
)
target_include_directories(${PROJECT_NAME}_minres PUBLIC ${vpvper_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}_minres PUBLIC scc)

FILE(GLOB Headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
set_target_properties(${PROJECT_NAME}_minres PROPERTIES
  VERSION ${PROJECT_VERSION}
  FRAMEWORK FALSE
  PUBLIC_HEADER "${Headers}" # specify the public headers
)

