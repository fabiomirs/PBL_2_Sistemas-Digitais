
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

#define P 0b01110011
#define B 0b01111100
#define L 0b00110000
#define traco 0b01000000
#define S 0b01101101
#define D 0b01011110
#define espaco 0b00000000
#define t 0b01111000
#define zero 0b00111111
#define um 0b00000110

int concatena_bits(uint8_t* vetor, int tamanho) {
    uint64_t resultado;
    for(int i = 0; i< tamanho; i++)
        resultado = (resultado << 8) || vetor[i];
    return resultado;
}

void rotaciona_esquerda(int arr[], int posicoes) {
    int temp[posicoes];
    
    for (int i = 0; i < posicoes; i++) {
        temp[i] = arr[i];
    }

    for (int i = posicoes; i < 11; i++) {
        arr[i - posicoes] = arr[i];
    }

    for (int i = 0; i < posicoes; i++) {
        arr[11 - posicoes + i] = temp[i];
    }
}

/* This program increments the contents of the red LED parallel port */
int main(void)
{
   uint8_t[] text = {P,B,L,traco,S,D,espaco,t,P,zero,um};
   volatile int * DISPLAY_ptr;
   volatile int * DISPLAY_ptrum;

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
  
  
   *DISPLAY_ptr = 0; 
   *DISPLAY_ptrum = 0; 
   
   
//    *DISPLAY_ptrum = 0b0111001101111100;
   
//    *DISPLAY_ptr = 0b01110011011111000011000001000000011011010101111001111000011100110011111101001111;
   
   for(int i = 0; i < 11; i++)
   {
        *DISPLAY_ptrum = concatena_bits(text, 2);
        *DISPLAY_ptr = concatena_bits(&text[2], 9);
        rotaciona_esquerda(text, 1);
        sleep(1);
   }
   
    
   
   unmap_physical (LW_virtual, LW_BRIDGE_SPAN);   // release the physical-memory mapping
   close_physical (fd);   // close /dev/mem
   return 0;
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
