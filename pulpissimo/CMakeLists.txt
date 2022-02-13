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
)
target_include_directories(${PROJECT_NAME}_pulpissimo PUBLIC ${vpvper_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}_pulpissimo PUBLIC scc)
FILE(GLOB SccPulpHeaders ${CMAKE_CURRENT_SOURCE_DIR}/pulp/scc/*.h ${CMAKE_CURRENT_SOURCE_DIR}/pulp/scc/gen/*.h)
set_target_properties(${PROJECT_NAME}_pulp PROPERTIES
  VERSION ${PROJECT_VERSION}
  FRAMEWORK FALSE
  PUBLIC_HEADER "${PulpHeaders}" # specify the public headers
)
