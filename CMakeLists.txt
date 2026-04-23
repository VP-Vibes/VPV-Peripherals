#
# Copyright (c) 2019 -2022 MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

project(vpvper VERSION 1.1.0)

set(SCC_INSTALL_DIR "" CACHE PATH "installation location of SCC library")

if(CMAKE_PROJECT_NAME STREQUAL "vpvper")
    message(STATUS "Building VPVper in standalone mode")
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
	set(CMAKE_INSTALL_RPATH "$\{ORIGIN\}")
	include(CheckCXXCompilerFlag)
    include(GNUInstallDirs)

    set(Boost_NO_BOOST_CMAKE ON)

    if(IS_DIRECTORY "${SCC_INSTALL_DIR}")
        list(PREPEND CMAKE_PREFIX_PATH "${SCC_INSTALL_DIR}")
    endif()

    # SCC's installed cmake config uses find_dependency(boost_filesystem) / find_dependency(boost_date_time)
    # (the per-component convention from a cmake-native Boost install).
    # Conan's boost provides a single BoostConfig.cmake, so we drop in thin bridge configs.
    foreach(_boost_comp filesystem date_time)
        if(NOT EXISTS "${CMAKE_BINARY_DIR}/boost_${_boost_comp}Config.cmake")
            file(WRITE "${CMAKE_BINARY_DIR}/boost_${_boost_comp}Config.cmake"
                "find_package(Boost REQUIRED COMPONENTS ${_boost_comp})\nset(boost_${_boost_comp}_FOUND TRUE)\n")
        endif()
    endforeach()

    find_package(scc QUIET)

    if(NOT TARGET scc)
        message(STATUS "SCC not found via find_package — fetching from GitHub")
        include(FetchContent)
        FetchContent_Declare(scc
            GIT_REPOSITORY https://github.com/Minres/SystemC-Components.git
            GIT_TAG        main
            GIT_SHALLOW    TRUE
        )
        FetchContent_MakeAvailable(scc)
    endif()

endif()

if(TARGET scc)
	add_subdirectory(generic)
	add_subdirectory(sifive)
	add_subdirectory(minres)
	add_subdirectory(pulpino)
	add_subdirectory(pulpissimo)
	add_library(${PROJECT_NAME} INTERFACE)
	target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}_generic ${PROJECT_NAME}_minres ${PROJECT_NAME}_sifive ${PROJECT_NAME}_pulpino  ${PROJECT_NAME}_pulpissimo)	
	
	install(TARGETS ${PROJECT_NAME}
	        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}${SCC_ARCHIVE_DIR_MODIFIER}
	        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	        )
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/minres.h ${CMAKE_CURRENT_SOURCE_DIR}/pulpissimo.h ${CMAKE_CURRENT_SOURCE_DIR}/sifive.h ${CMAKE_CURRENT_SOURCE_DIR}/generic.h
			COMPONENT ${PROJECT_NAME}
	        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # target directory
	        )
endif()
