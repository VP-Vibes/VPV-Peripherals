#
# Copyright (c) 2019 -2021 MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)
set(SOURCES 
	gpio.cpp
	timer.cpp
	qspi.cpp
	irq.cpp
	uart.cpp
  aclint.cpp
  dma.cpp
  ethmac.cpp
)
###############################################################################
#
###############################################################################
add_library(${PROJECT_NAME}_minres_tlm ${SOURCES})
target_include_directories(${PROJECT_NAME}_minres_tlm PUBLIC ${vpvper_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}_minres_tlm PUBLIC scc)
add_library(${PROJECT_NAME}::minres_tlm ALIAS ${PROJECT_NAME}_minres_tlm)

install(TARGETS ${PROJECT_NAME}_minres_tlm COMPONENT ${PROJECT_NAME}_minres_tlm
  EXPORT ${PROJECT_NAME}_minresTargets                         # for downstream dependencies
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}         # for mac
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}      # headers
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPONENT ${PROJECT_NAME}_minres_tlm
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
        )
###############################################################################
#
###############################################################################
add_library(${PROJECT_NAME}_minres_sig ${SOURCES})
target_compile_definitions(${PROJECT_NAME}_minres_sig PUBLIC SC_SIGNAL_IF)
target_include_directories(${PROJECT_NAME}_minres_sig PUBLIC ${vpvper_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}_minres_sig PUBLIC scc)
add_library(${PROJECT_NAME}::minres_sig ALIAS ${PROJECT_NAME}_minres_sig)

install(TARGETS ${PROJECT_NAME}_minres_sig COMPONENT ${PROJECT_NAME}_minres_sig
  EXPORT ${PROJECT_NAME}_minresTargets                         # for downstream dependencies
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}         # for mac
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}      # headers
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPONENT ${PROJECT_NAME}_minres_sig
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
        )
###############################################################################
#
###############################################################################
set(USE_SC_SIGNAL4IRQ OFF CACHE BOOL "Enable the use of sc_signals for interrupt delivery")
if(USE_SC_SIGNAL4IRQ)
    add_library(${PROJECT_NAME}_minres ALIAS ${PROJECT_NAME}_minres_sig)
else()
    add_library(${PROJECT_NAME}_minres ALIAS ${PROJECT_NAME}_minres_tlm)
endif()
