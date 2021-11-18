#
# Copyright (c) 2019 -2021 MINRES Technolgies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)

project(vpvper VERSION 1.1.0)

if(TARGET scc)
###############################################################################
# Generic
###############################################################################
if(TARGET CONAN_PKG::seasocks)
 	set(WEBSOCKET_SRC
 		generic/sc_comm_singleton.cpp
 	)
 	set(SEASOCKS_LIB CONAN_PKG::seasocks)
else()
	find_package(Seasocks QUIET)
	if(TARGET Seasocks::seasocks)
	 	set(WEBSOCKET_SRC
	 		generic/sc_comm_singleton.cpp
	 	)
	 	set(SEASOCKS_LIB Seasocks::seasocks)
	endif()	
endif()

add_library(${PROJECT_NAME}_generic generic/terminal.cpp ${WEBSOCKET_SRC})
target_include_directories(${PROJECT_NAME}_generic PUBLIC ${CMAKE_CURRENT_LIST_DIR})
if(SEASOCKS_LIB)
	target_compile_definitions(${PROJECT_NAME}_generic PUBLIC HAS_WEB_SOCKETS)
	target_link_libraries(${PROJECT_NAME}_generic PUBLIC ${SEASOCKS_LIB})
endif()
target_link_libraries(${PROJECT_NAME}_generic PUBLIC scc)
FILE(GLOB GenericHeaders ${CMAKE_CURRENT_SOURCE_DIR}/generic/*.h)
set_target_properties(${PROJECT_NAME}_generic PROPERTIES
  VERSION ${PROJECT_VERSION}
  FRAMEWORK FALSE
  PUBLIC_HEADER "${GenericHeaders}" # specify the public headers
)

###############################################################################
# SiFive
###############################################################################
add_library(${PROJECT_NAME}_sifive
	sifive/aon.cpp
	sifive/clint.cpp
	sifive/plic.cpp
	sifive/prci.cpp
	sifive/pwm.cpp
	sifive/spi.cpp
	sifive/uart.cpp
 	sifive/uart_terminal.cpp
	sifive/gpio.cpp
)
target_include_directories(${PROJECT_NAME}_sifive PUBLIC ${CMAKE_CURRENT_LIST_DIR})
if(SEASOCKS_LIB)
	target_compile_definitions(${PROJECT_NAME}_sifive PUBLIC HAS_WEB_SOCKETS)
	target_link_libraries(${PROJECT_NAME}_sifive PUBLIC ${PROJECT_NAME}_generic)
endif()
target_link_libraries(${PROJECT_NAME}_sifive PUBLIC scc)

FILE(GLOB SiFiveHeaders ${CMAKE_CURRENT_SOURCE_DIR}/sifive/*.h ${CMAKE_CURRENT_SOURCE_DIR}/sifive/*/*.h)
set_target_properties(${PROJECT_NAME}_sifive PROPERTIES
  VERSION ${PROJECT_VERSION}
  FRAMEWORK FALSE
  PUBLIC_HEADER "${SiFiveHeaders}" # specify the public headers
)

add_library(${PROJECT_NAME}_minres
	minres/gpio.cpp
	minres/presc.cpp
	minres/tim.cpp
	minres/uart_terminal.cpp
)
target_include_directories(${PROJECT_NAME}_minres PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME}_minres PUBLIC scc)

FILE(GLOB MinresHeaders ${CMAKE_CURRENT_SOURCE_DIR}/minres/*.h ${CMAKE_CURRENT_SOURCE_DIR}/minres/*/*.h)
set_target_properties(${PROJECT_NAME}_minres PROPERTIES
  VERSION ${PROJECT_VERSION}
  FRAMEWORK FALSE
  PUBLIC_HEADER "${MinresHeaders}" # specify the public headers
)

###############################################################################
# Pulp
###############################################################################
add_library(${PROJECT_NAME}_pulp
  pulp/experimental/external_uart_device.cpp
)
target_include_directories(${PROJECT_NAME}_pulp PUBLIC  ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME}_pulp PUBLIC scc)

if(PULP_EXPERIMENTAL)
  target_link_libraries(${PROJECT_NAME}_pulp Boost::filesystem Boost::iostreams)
endif()

FILE(GLOB PulpHeaders ${CMAKE_CURRENT_SOURCE_DIR}/pulp/*.h ${CMAKE_CURRENT_SOURCE_DIR}/minres/*/*.h)
set_target_properties(${PROJECT_NAME}_pulp PROPERTIES
  VERSION ${PROJECT_VERSION}
  FRAMEWORK FALSE
  PUBLIC_HEADER "${PulpHeaders}" # specify the public headers
)

endif()
