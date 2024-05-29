// #include <linux/kernel.h>
// #include <linux/module.h>
// #include <linux/init.h>
// #include <asm/io.h>
// #include "address_map_arm.h"
#include <stdint.h>
#include "instructions.h"


// void * LW_virtual; // Lightweight bridge base address

void assembleInstructionWBR(volatile int *DATA_A_PTR, volatile int *DATA_B_PTR, 
    uint8_t reg, uint8_t R, uint8_t G, uint8_t B) {
    *DATA_A_PTR =(0b0000) |((uint8_t)reg << 4);
    *DATA_B_PTR =((uint8_t)R << 0) | ((uint8_t)G << 3) | ((uint8_t)B << 6);
}

void assembleInstructionWBR_2(volatile int *DATA_A_PTR, volatile int *DATA_B_PTR,
    uint8_t opcode, uint8_t reg, uint8_t offset, uint8_t Coord_y, uint8_t Coord_x, uint8_t sp) {
    *DATA_A_PTR =((uint8_t)opcode << 0) |((uint8_t)reg << 4);
    *DATA_B_PTR =((uint8_t)offset << 0) | ((uint8_t)Coord_y << 9) | ((uint8_t)Coord_x << 19) | ((uint8_t)sp << 29);
}

void assembleInstructionWSM(volatile int *DATA_A_PTR, volatile int *DATA_B_PTR,
    uint8_t opcode,uint8_t address, uint8_t R, uint8_t G, uint8_t B) {
    *DATA_A_PTR = ((uint64_t)opcode << 0) | ((uint32_t)address << 4);
    *DATA_B_PTR = ((uint32_t)R << 0) | ((uint32_t)G << 3)|((uint32_t)B << 6);
}

void assembleInstructionDP(volatile int *DATA_A_PTR, volatile int *DATA_B_PTR,
    uint8_t ref_point_X, uint8_t ref_point_Y, uint8_t address, Polygon polygon){
    *DATA_A_PTR = (0b0011) | ((uint32_t)address << 4);
    *DATA_B_PTR = ((uint32_t)ref_point_X << 0) | ((uint32_t)ref_point_Y << 9)|((uint32_t)polygon.size << 18)|
        ((uint32_t)polygon.R << 22)|((uint32_t)polygon.G << 25)|((uint32_t)polygon.B << 28)|((uint32_t)polygon.shape << 31);
}

void mask32BitsTo4Chars(uint32_t value, char chars[4]) {
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
}


// static int __init init_kernelteste(void) {
//     // generate a virtual address for the FPGA lightweight bridge
//     LW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
//    // Set virtual address pointer to I/O port
//    DATA_A_PTR = (unsigned int *) (LW_virtual + DATA_A);
//    DATA_B_PTR = (unsigned int *) (LW_virtual + DATA_B);   
//    START_PTR = (unsigned int *) (LW_virtual + START);   

//   Polygon polygon;
//   polygon.ref_point_X = 10;
//   polygon.ref_point_Y = 10;
//   polygon.size = 0b001;
//   polygon.R = 0;
//   polygon.G = 7;
//   polygon.B = 0;
//   polygon.shape = 0b1;
//   assembleInstructionDP(0b0,polygon);

//    return 0;
// }

// static void __exit exit_kernelteste(void) {
//    *START_PTR = 0;
//     iounmap (LW_virtual);
// }

// module_init(init_kernelteste);
// module_exit(exit_kernelteste);
