#
# Copyright (c) 2025 MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)

###############################################################################
# RVI
###############################################################################
add_library(${PROJECT_NAME}_rvi
	aplic.cpp
)
if(USE_SC_SIGNAL4IRQ)
    target_compile_definitions(${PROJECT_NAME}_rvi PUBLIC SC_SIGNAL_IF)
endif()
target_include_directories(${PROJECT_NAME}_rvi PUBLIC ${vpvper_SOURCE_DIR})
if(SEASOCKS_LIB)
	target_compile_definitions(${PROJECT_NAME}_rvi PUBLIC HAS_WEB_SOCKETS)
	target_link_libraries(${PROJECT_NAME}_rvi PUBLIC ${PROJECT_NAME}_generic)
endif()
target_link_libraries(${PROJECT_NAME}_rvi PUBLIC ${PROJECT_NAME}_generic scc)
add_library(${PROJECT_NAME}::rvi ALIAS ${PROJECT_NAME}_rvi)

install(TARGETS ${PROJECT_NAME}_rvi COMPONENT ${PROJECT_NAME}_rvi
  EXPORT ${PROJECT_NAME}_genericTargets                         # for downstream dependencies
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}         # for mac
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}      # headers
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPONENT ${PROJECT_NAME}_rvi
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
        )
