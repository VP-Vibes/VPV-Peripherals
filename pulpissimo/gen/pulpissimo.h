#ifndef _PULPISSIMO_MAP_H_
#define _PULPISSIMO_MAP_H_
// need double braces, see https://stackoverflow.com/questions/6893700/how-to-construct-stdarray-object-with-initializer-list#6894191
const std::array<scc::target_memory_map_entry<32>, 6> pulpissimo_map = {{
    {i_fll.socket, 0x1a100000, 0x10},
    {i_gpio.socket, 0x1a101000, 0x48},
    {i_udma.socket, 0x1a102000, 0x464},
    {i_adv_timer.socket, 0x1a105000, 0x108},
    {i_interrupt.socket, 0x1a109000, 0x28},
    {i_timer.socket, 0x1a10b000, 0x28},
}};

#endif /* _PULPISSIMO_MAP_H_ */
