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
if(USE_SC_SIGNAL4IRQ)
    target_compile_definitions(${PROJECT_NAME}_sifive PUBLIC SC_SIGNAL_IF)
endif()
target_include_directories(${PROJECT_NAME}_sifive PUBLIC ${vpvper_SOURCE_DIR})
if(SEASOCKS_LIB)
	target_compile_definitions(${PROJECT_NAME}_sifive PUBLIC HAS_WEB_SOCKETS)
	target_link_libraries(${PROJECT_NAME}_sifive PUBLIC ${PROJECT_NAME}_generic)
endif()
target_link_libraries(${PROJECT_NAME}_sifive PUBLIC ${PROJECT_NAME}_generic scc)
add_library(${PROJECT_NAME}::sifive ALIAS ${PROJECT_NAME}_sifive)

install(TARGETS ${PROJECT_NAME}_sifive COMPONENT ${PROJECT_NAME}_sifive
  EXPORT ${PROJECT_NAME}_genericTargets                         # for downstream dependencies
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}         # for mac
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}      # headers
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPONENT ${PROJECT_NAME}_sifive
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
        )
