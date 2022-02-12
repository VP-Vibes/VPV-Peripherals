#
# Copyright (c) 2019 -2021 MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)

###############################################################################
# Generic
###############################################################################
if(TARGET CONAN_PKG::seasocks)
 	set(WEBSOCKET_SRC
 		sc_comm_singleton.cpp
 	)
 	set(SEASOCKS_LIB CONAN_PKG::seasocks)
else()
	find_package(Seasocks QUIET)
	if(TARGET Seasocks::seasocks)
	 	set(WEBSOCKET_SRC
	 		sc_comm_singleton.cpp
	 	)
	 	set(SEASOCKS_LIB Seasocks::seasocks)
	endif()	
endif()

add_library(${PROJECT_NAME}_generic terminal.cpp ${WEBSOCKET_SRC})
target_include_directories(${PROJECT_NAME}_generic PUBLIC ${vpvper_SOURCE_DIR})
if(SEASOCKS_LIB)
	target_compile_definitions(${PROJECT_NAME}_generic PUBLIC HAS_WEB_SOCKETS)
	target_link_libraries(${PROJECT_NAME}_generic PUBLIC ${SEASOCKS_LIB})
endif()
target_link_libraries(${PROJECT_NAME}_generic PUBLIC scc)
FILE(GLOB GenericHeaders ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
set_target_properties(${PROJECT_NAME}_generic PROPERTIES
  VERSION ${PROJECT_VERSION}
  FRAMEWORK FALSE
  PUBLIC_HEADER "${GenericHeaders}" # specify the public headers
)

