#
# Copyright (c) 2019 -2021 MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)

###############################################################################
# Pulp
###############################################################################
FILE(GLOB PulpinoHeaders ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/experimental/*.h)
if(PULP_EXPERIMENTAL)
  add_library(${PROJECT_NAME}_pulpino
    experimental/external_uart_device.cpp
  )
  target_include_directories(${PROJECT_NAME}_pulpino PUBLIC ${CMAKE_CURRENT_LIST_DIR})
  target_link_libraries(${PROJECT_NAME}_pulpino PUBLIC scc)
  target_link_libraries(${PROJECT_NAME}_pulpino PRIVATE Boost::filesystem Boost::iostreams)
else()
  add_library(${PROJECT_NAME}_pulpino INTERFACE)
  target_include_directories(${PROJECT_NAME}_pulpino INTERFACE ${vpvper_SOURCE_DIR})
  target_link_libraries(${PROJECT_NAME}_pulpino INTERFACE scc)
endif()
add_library(${PROJECT_NAME}::pulpino ALIAS ${PROJECT_NAME}_pulpino)

install(TARGETS ${PROJECT_NAME}_pulpino COMPONENT ${PROJECT_NAME}_pulpino
  EXPORT ${PROJECT_NAME}_pulpinoTargets                         # for downstream dependencies
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}         # for mac
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}      # headers
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPONENT ${PROJECT_NAME}_pulpino
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
        )
