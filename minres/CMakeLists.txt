#
# Copyright (c) 2019 -2021 MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)

add_library(${PROJECT_NAME}_minres
	gpio.cpp
	timer.cpp
	qspi.cpp
	irq.cpp
	uart.cpp
  aclint.cpp
  dma.cpp
)
target_include_directories(${PROJECT_NAME}_minres PUBLIC ${vpvper_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}_minres PUBLIC scc)

install(TARGETS ${PROJECT_NAME}_minres COMPONENT ${PROJECT_NAME}_minres
  EXPORT ${PROJECT_NAME}_minresTargets                         # for downstream dependencies
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}         # for mac
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}      # headers
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPONENT ${PROJECT_NAME}_minres
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
        )
