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

add_library(${PROJECT_NAME}_generic terminal.cpp ${WEBSOCKET_SRC} spi_mem.cpp)
target_include_directories(${PROJECT_NAME}_generic PUBLIC ${vpvper_SOURCE_DIR})
if(SEASOCKS_LIB)
	target_compile_definitions(${PROJECT_NAME}_generic PUBLIC HAS_WEB_SOCKETS)
	target_link_libraries(${PROJECT_NAME}_generic PUBLIC ${SEASOCKS_LIB})
endif()
target_link_libraries(${PROJECT_NAME}_generic PUBLIC scc)

install(TARGETS ${PROJECT_NAME}_generic COMPONENT ${PROJECT_NAME}_generic
  EXPORT ${PROJECT_NAME}_genericTargets                         # for downstream dependencies
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}         # for mac
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}      # headers
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPONENT ${PROJECT_NAME}_generic
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
        )
