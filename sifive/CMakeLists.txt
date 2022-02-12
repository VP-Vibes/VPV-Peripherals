#
# Copyright (c) 2019 -2021 MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)

###############################################################################
# SiFive
###############################################################################
add_library(${PROJECT_NAME}_sifive
	aon.cpp
	clint.cpp
	plic.cpp
	prci.cpp
	pwm.cpp
	spi.cpp
	uart.cpp
 	uart_terminal.cpp
	gpio.cpp
)
target_include_directories(${PROJECT_NAME}_sifive PUBLIC ${vpvper_SOURCE_DIR})
if(SEASOCKS_LIB)
	target_compile_definitions(${PROJECT_NAME}_sifive PUBLIC HAS_WEB_SOCKETS)
	target_link_libraries(${PROJECT_NAME}_sifive PUBLIC ${PROJECT_NAME}_generic)
endif()
target_link_libraries(${PROJECT_NAME}_sifive PUBLIC ${PROJECT_NAME}_generic scc)

FILE(GLOB SiFiveHeaders ${CMAKE_CURRENT_SOURCE_DIR}/sifive/*.h ${CMAKE_CURRENT_SOURCE_DIR}/sifive/*/*.h)
set_target_properties(${PROJECT_NAME}_sifive PROPERTIES
  VERSION ${PROJECT_VERSION}
  FRAMEWORK FALSE
  PUBLIC_HEADER "${SiFiveHeaders}" # specify the public headers
)
