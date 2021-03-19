cmake_minimum_required(VERSION 3.12)

project(vpvper VERSION 1.0.0)

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
	target_link_libraries(${PROJECT_NAME}_sifive PUBLIC CONAN_PKG::seasocks)
endif()
target_link_libraries(${PROJECT_NAME}_sifive PUBLIC scc)

FILE(GLOB HiFiveHeaders ${CMAKE_CURRENT_SOURCE_DIR}/incl/sysc/*.h ${CMAKE_CURRENT_SOURCE_DIR}/incl/sysc/*/*.h)
set_target_properties(${PROJECT_NAME}_sifive PROPERTIES
  VERSION ${PROJECT_VERSION}
  FRAMEWORK FALSE
  PUBLIC_HEADER "${HiFiveHeaders}" # specify the public headers
)
