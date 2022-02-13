#
# Copyright (c) 2019 -2021 MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)

project(vpvper VERSION 1.1.0)

if(TARGET scc)
add_subdirectory(generic)
add_subdirectory(sifive)
add_subdirectory(minres)
add_subdirectory(pulp)
add_subdirectory(pulpissimo)
endif()
