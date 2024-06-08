#include "instructions.h"
#include "datastructures.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define BYTE_BITS 8
#define DEVICE "/dev/gpu123"

int open_physical (int);
void close_physical (int);

GraphicElement screenElements[32] = {};
char availableRegisters[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

char lastRegister = 0;

void printBits(char *palavra) {
    int i;
    int j;
    for (i = 7; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            printf("%d", (palavra[i] >> j) & 1);
        }
        printf(" "); // Espaço a cada byte
    }
    printf("\n");
}

int writeBitsOnDeviceDriver(unsigned char* bits, char* error_msg){
    int fd;
    if ((fd = open_physical (fd)) == -1)
        return (-1);

    ssize_t buffer =  write(fd,bits,8);

    unsigned char lendo[8];
    ssize_t bytesRead=read(fd,lendo,8);
    if (bytesRead<0) {
        printf("%s", error_msg);
        close_physical (fd);
        return bytesRead;
    }

    close_physical (fd);
    return 0;   
}



int setBackground(Color color) {
    char *word = assembleInstructionWBR(color.R, color.G, color.B);
    return writeBitsOnDeviceDriver(word, "erro na escrita do background");
   
}

int setPixelOnSpriteMemory(unsigned int mem_address, Color color) {
    char* word = assembleInstructionWSM(mem_address, color.R, color.G, color.B);
    return writeBitsOnDeviceDriver(word, "erro na edição de pixel na memória de sprites");
}

int editBlockOnBackgroundMemory(unsigned int block, Color color) {
    unsigned char *word = assembleInstructionWBM(block, color.R, color.G, color.B);
    return writeBitsOnDeviceDriver(word, "erro na edição de bloco na memória de background");
}

int setSpriteOnScreen(Sprite sprite) {
    unsigned char *word = assembleInstructionWBR_2(sprite.address, sprite.variation, sprite.rel_y, sprite.rel_x, sprite.visible);
    return writeBitsOnDeviceDriver(word, "erro na escrita de sprite");
}

int setPolygon(Polygon polygon) {
    // unsigned char validate_x = polygon.rel_x + (polygon.size + 1) * 5;
    // unsigned char validate_y = polygon.rel_y + (polygon.size + 1) * 5;
    unsigned char *word = assembleInstructionDP(polygon.rel_x, polygon.rel_y, polygon.address,
    polygon.size, polygon.color.R, polygon.color.G, polygon.color.B, polygon.shape);
    return writeBitsOnDeviceDriver(word, "erro na escrita de polígono");
}

int eraseBackground() {
    Color color = {7,7,7};
    return setBackground(color);
}


int open_physical (int fd) {


    if ((fd = open( "/dev/gpu123", (O_RDWR | O_SYNC))) == -1) {
        printf ("ERROR: could not open \"/dev/mem\"...\n");
        return (-1);
    }
    return fd;
}




void close_physical (int fd) {
    close (fd);
}

