#
# Copyright (c) MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)

add_executable(test_plic test_plic.cpp)
add_executable(test_aplic test_aplic.cpp)
target_link_libraries(test_plic PRIVATE ${PROJECT_NAME}_rvi)
target_link_libraries(test_aplic PRIVATE ${PROJECT_NAME}_rvi)

add_test(NAME plic_functionality COMMAND $<TARGET_FILE:test_plic>)
add_test(NAME aplic_functionality COMMAND $<TARGET_FILE:test_aplic>)
