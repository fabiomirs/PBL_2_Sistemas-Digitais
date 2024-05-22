#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include "address_map_arm.h"


//CONSTANTES
#define DATA_A  0x80
#define DATA_B  0x70
#define START 0xc0

 void * LW_virtual; // Lightweight bridge base address
 volatile int *DATA_A_PTR, *DATA_B_PTR, *START_PTR; // virtual addresses

 
 static int __init init_kernelteste(void) {
    // generate a virtual address for the FPGA lightweight bridge
    LW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);



   // Set virtual address pointer to I/O port
   DATA_A_PTR = (unsigned int *) (LW_virtual + DATA_A);
   DATA_B_PTR = (unsigned int *) (LW_virtual + DATA_B);   
   START_PTR = (unsigned int *) (LW_virtual + START);   
   // Add 1 to the I/O register
   *START_PTR = 0;
   *DATA_A_PTR = 0b0;
   *DATA_B_PTR = 38;
   *START_PTR = 1;
   //*START_PTR = 0;



   return 0;
 }

 
 static void __exit exit_kernelteste(void) {
   *START_PTR = 0;
    iounmap (LW_virtual);
}
module_init(init_kernelteste);
module_exit(exit_kernelteste);
