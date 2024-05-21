#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm.h"

/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

#define P 0b1110011
#define B 0b1111100
#define L 0b0110000
#define traco 0b1000000
#define S 0b1101101
#define D 0b1011110
#define espaco 0b0000000
#define t 0b1111000
#define zero 0b0111111
#define tres 0b1001111

#define botao 0x0000005C

/* This program increments the contents of the red LED parallel port */
int main(void)
{
    int letras[17];// ={P,B,L,traco,S,D,espaco,t,P,zero,um};
    letras[0] = espaco;
    letras[1] = espaco;
    letras[2] = espaco;
    letras[3] = P;
    letras[4] = B;
    letras[5] = L;
    letras[6] = espaco;
    letras[7] = S;
    letras[8] = D;
    letras[9] = traco;
    letras[10] = t;
    letras[11] = P;
    letras[12] = zero;
    letras[13] = tres;
    letras[14] = espaco;
    letras[15] = espaco;
    letras[16] = espaco;
    
 
		
  
    volatile int * DISPLAY_ptr;
    volatile int * DISPLAY_ptrum;
    volatile int * chave;

    int fd = -1;               // used to open /dev/mem for access to physical addresses
    void *LW_virtual;          // used to map physical addresses for the light-weight bridge
    
   // Create virtual memory access to the FPGA light-weight bridge
   if ((fd = open_physical (fd)) == -1)
      return (-1);
   if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
      return (-1);

   // Set virtual address pointer to I/O port
   DISPLAY_ptr = (unsigned int *) (LW_virtual + HEX3_HEX0_BASE);
   
   DISPLAY_ptrum = (unsigned int *) (LW_virtual + HEX5_HEX4_BASE);
    
   chave = (unsigned int *) (LW_virtual + botao);
   *DISPLAY_ptr = 0; 
   *DISPLAY_ptrum = 0; 
    int botao_pressionado=0;
    int pause=0;
    *chave=15;

    int i = 0;

    while(1){

        if(*chave==1){
                botao_pressionado++;
                *chave=15;
            }

        if(botao_pressionado==2){
                pause=0;
                botao_pressionado=0;
        }
        

        for(i; i < 12 && pause==0; i++) {
            
            if(*chave==1){
                botao_pressionado++;
                *chave=15;
            }

            if(botao_pressionado==1)
                 pause=1;
            
            
                


            printf("%d",pause);
            * DISPLAY_ptrum= letras[i+1] << 0 | letras[i] << 8;
            * DISPLAY_ptr = letras[i+5] << 0 | letras[i+4] << 8 | letras[i+3] << 16 | letras[i+2] << 24;
            sleep(1);
        }

        if(i>=12) i = 0;

    
    }
}



// Open /dev/mem, if not already done, to give access to physical addresses
int open_physical (int fd) {
    if (fd == -1)
        if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1) {
            printf ("ERROR: could not open \"/dev/mem\"...\n");
            return (-1);
        }
    return fd;
}

// Close /dev/mem to give access to physical addresses
void close_physical (int fd) {
    close (fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base, and
 * extending by span bytes.
 */
void* map_physical(int fd, unsigned int base, unsigned int span) {
    void *virtual_base;

    // Get a mapping from physical addresses to virtual addresses
    virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
    if (virtual_base == MAP_FAILED) {
        printf ("ERROR: mmap() failed...\n");
        close (fd);
        return (NULL);
    }
    return virtual_base;
}

/*
 * Close the previously-opened virtual address mapping
 */
int unmap_physical(void * virtual_base, unsigned int span) {
    if (munmap (virtual_base, span) != 0) {
        printf ("ERROR: munmap() failed...\n");
        return (-1);
    }
    return 0;
}
