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
	set(CMAKE_CXX_STANDARD 11)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
	set(CMAKE_INSTALL_RPATH "$\{ORIGIN\}")
	include(CheckCXXCompilerFlag)
    include(GNUInstallDirs)

	if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
	  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
	  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
	                "${CMAKE_BINARY_DIR}/conan.cmake"
	                TLS_VERIFY ON)
	endif()
	
	include(${CMAKE_BINARY_DIR}/conan.cmake)
    conan_check()
    set(CONAN_PACKAGE_LIST seasocks/1.4.4)
    set(CONAN_PACKAGE_OPTIONS seasocks:shared=False seasocks:fPIC=True)
    if(NOT DEFINED ENV{SYSTEMC_HOME})
        list(APPEND CONAN_PACKAGE_LIST systemc/2.3.3 systemc-cci/1.0.0)
        #list(APPEND CONAN_PACKAGE_OPTIONS systemc-cci:shared=True)
    endif()
	list(APPEND CONAN_PACKAGE_LIST boost/1.75.0)    	
	list(APPEND CONAN_PACKAGE_OPTIONS
		boost:fPIC=True
		boost:shared=True
		boost:header_only=False 
		boost:without_contract=True
		boost:without_graph=True
		boost:without_graph_parallel=True
		boost:without_iostreams=True
		boost:without_json=True
		boost:without_locale=True
		boost:without_log=True
		boost:without_math=True
		boost:without_mpi=True
		boost:without_nowide=True
		boost:without_python=True
		boost:without_random=True
		boost:without_regex=True
		boost:without_stacktrace=True
		boost:without_test=True
		boost:without_timer=True
		boost:without_type_erasure=True
		boost:without_wave=True)
    if(NOT IS_DIRECTORY ${SCC_INSTALL_DIR}/include)
    	list(APPEND CONAN_PACKAGE_LIST scc/2022.08)
    endif()
    
	conan_cmake_configure(REQUIRES ${CONAN_PACKAGE_LIST}
						  OPTIONS ${CONAN_PACKAGE_OPTIONS}
	                      GENERATORS cmake_find_package)
	
	conan_cmake_autodetect(settings)
	
	conan_cmake_install(PATH_OR_REFERENCE .
	                    BUILD missing
	                    REMOTE conancenter
	                    SETTINGS ${settings})
                        
    set(Boost_NO_BOOST_CMAKE ON) #  Don't do a find_package in config mode before searching for a regular boost install.
    
    find_package(SystemCLanguage)
    find_package(systemc-cci)
    if(IS_DIRECTORY ${SCC_INSTALL_DIR}/include)
    	add_library(scc INTERFACE)
    	target_include_directories(scc INTERFACE ${SCC_INSTALL_DIR}/include)
    	target_link_directories(scc INTERFACE ${SCC_INSTALL_DIR}/lib)
		target_link_libraries(scc INTERFACE busses scc-sysc scc-util scv-tr)	
		target_link_libraries(scc INTERFACE systemc-cci::systemc-cci SystemC::SystemC)	
    else()
    	find_package(scc)
        add_library(scc INTERFACE IMPORTED)
	    set_property(TARGET scc APPEND PROPERTY
                 INTERFACE_LINK_LIBRARIES scc::scc)
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
