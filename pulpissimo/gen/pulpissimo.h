/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Created on: Sat Jan 07 23:53:33 CET 2023
 *             *      pulpissimo.h Author: <RDL Generator>
 *
 */

#pragma once

// need double braces, see https://stackoverflow.com/questions/6893700/how-to-construct-stdarray-object-with-initializer-list#6894191
const std::array<scc::target_memory_map_entry<32>, 8> pulpissimo_map = {{
    {i_fll.socket, 0x1a100000, 0x10},
    {i_gpio.socket, 0x1a101000, 0x70},
    {i_udma.socket, 0x1a102000, 0x464},
    {i_soc_ctrl.socket, 0x1a104000, 0xcc},
    {i_adv_timer.socket, 0x1a105000, 0x108},
    {i_soc_event.socket, 0x1a106000, 0x8c},
    {i_interrupt.socket, 0x1a109000, 0x28},
    {i_timer.socket, 0x1a10b000, 0x28},
}};

