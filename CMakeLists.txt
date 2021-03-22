cmake_minimum_required(VERSION 3.12)

project(vpvper VERSION 1.0.0)

###############################################################################
# Generic
###############################################################################
if(TARGET CONAN_PKG::seasocks)
 	set(WEBSOCK_GEN_SRC
 		generic/sc_comm_singleton.cpp
 	 	generic/terminal.cpp
 	)
endif()
add_library(${PROJECT_NAME}_generic
	${WEBSOCK_GEN_SRC}
)
target_include_directories(${PROJECT_NAME}_generic PUBLIC ${CMAKE_CURRENT_LIST_DIR})
if(TARGET CONAN_PKG::seasocks)
	target_link_libraries(${PROJECT_NAME}_generic PUBLIC CONAN_PKG::seasocks)
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
if(TARGET CONAN_PKG::seasocks)
 	set(WEBSOCK_SRC
 	 	sifive/gpio.cpp
 	)
endif()
add_library(${PROJECT_NAME}_sifive
	sifive/aon.cpp
	sifive/clint.cpp
	sifive/plic.cpp
	sifive/prci.cpp
	sifive/pwm.cpp
	sifive/spi.cpp
	sifive/uart.cpp
	${WEBSOCK_SRC}
)
target_include_directories(${PROJECT_NAME}_sifive PUBLIC ${CMAKE_CURRENT_LIST_DIR})
if(TARGET CONAN_PKG::seasocks)
	target_link_libraries(${PROJECT_NAME}_sifive PUBLIC ${PROJECT_NAME}_generic)
endif()
target_link_libraries(${PROJECT_NAME}_sifive PUBLIC scc)

FILE(GLOB SiFiveHeaders ${CMAKE_CURRENT_SOURCE_DIR}/incl/sysc/*.h ${CMAKE_CURRENT_SOURCE_DIR}/incl/sysc/*/*.h)
set_target_properties(${PROJECT_NAME}_sifive PROPERTIES
  VERSION ${PROJECT_VERSION}
  FRAMEWORK FALSE
  PUBLIC_HEADER "${SiFiveHeaders}" # specify the public headers
)
