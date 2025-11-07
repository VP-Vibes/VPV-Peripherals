#
# Copyright (c) 2019 -2021 MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)

###############################################################################
# Pulp
###############################################################################
add_library(${PROJECT_NAME}_pulpissimo
  adv_timer.cpp
  fll.cpp
  gpio.cpp
  interrupt.cpp
  timer.cpp
  udma.cpp
  soc_ctrl.cpp
  soc_event.cpp
)
target_include_directories(${PROJECT_NAME}_pulpissimo PUBLIC ${vpvper_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}_pulpissimo PUBLIC scc)
add_library(${PROJECT_NAME}::pulpissimo ALIAS ${PROJECT_NAME}_pulpissimo)

install(TARGETS ${PROJECT_NAME}_pulpissimo COMPONENT ${PROJECT_NAME}_pulpissimo
  EXPORT ${PROJECT_NAME}_pulpissimoTargets                         # for downstream dependencies
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}         # for mac
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}      # headers
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPONENT ${PROJECT_NAME}_pulpissimo
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
        )
