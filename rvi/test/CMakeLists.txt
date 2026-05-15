#
# Copyright (c) MINRES Technologies GmbH
#
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.12)

add_executable(test_plic test_plic.cpp)
target_link_libraries(test_plic PRIVATE ${PROJECT_NAME}_rvi)
add_test(NAME plic_functionality COMMAND test_plic)
