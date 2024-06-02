#include "instructions.h"
#include "datastructures.h"
#include "gpu.h"
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
unsigned char lastRegister = 0;

void printBits(unsigned char *palavra) {
    for (int i = 7; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (palavra[i] >> j) & 1);
        }
        printf(" "); // Espaço a cada byte
    }
    printf("\n");
}

int setPixelOnSpriteMemory(unsigned int mem_address, unsigned char R, 
    unsigned char G, unsigned char B) {

    unsigned char *palavra = assembleInstructionWSM(mem_address, R, G, B);
    printBits(palavra);


    int fd;
    if ((fd = open_physical (fd)) == -1)
        return (-1);

    ssize_t buffer =  write(fd,palavra,sizeof(palavra));


   printf("\n");


   unsigned char lendo[8];
   ssize_t bytesRead=read(fd,lendo,sizeof(lendo));
    if (bytesRead<0) {
        printf("falhou");
        close_physical (fd);
        return bytesRead;
    } else {
        printBits(lendo);
    }

    close_physical (fd);
    return 0;   
}


Sprite* setSpriteOnScreen(unsigned char variation, unsigned int x, unsigned int y) {
    unsigned char *palavra;
    if (lastRegister <= 31)
        palavra = assembleInstructionWBR_2(++lastRegister, variation, x, y, 1);
    else {
        printf("Número máximo de registradores utilizados atingido.");
        return NULL;
    }

    int fd;
    if ((fd = open_physical (fd)) == -1)
        return NULL;

    ssize_t buffer = write(fd,palavra,sizeof(palavra));

    unsigned char lendo[8];
    ssize_t bytesRead=read(fd,lendo,sizeof(lendo));
    if (bytesRead<0){
        printf("instrução de escrita de sprite falhou");
        close_physical (fd);
        return NULL;
    }

    close_physical (fd);
    Sprite* sprite = (Sprite *)malloc(sizeof(Sprite));
    sprite->address == lastRegister;
    sprite->variation = variation;
    sprite->rel_x = x;
    sprite->rel_y = y;

    return sprite;   
}

int eraseSpriteOnScreen(Sprite sprite) {
    unsigned char *palavra = assembleInstructionWBR_2(sprite.address, 
    sprite.variation, sprite.rel_x, sprite.rel_y, 0); // única diferença no sp

    int fd;
    if ((fd = open_physical (fd)) == -1)
        return (-1);

    ssize_t buffer =  write(fd,palavra,sizeof(palavra));
        
    unsigned char lendo[8];
    ssize_t bytesRead=read(fd,lendo,sizeof(lendo));
    if (bytesRead<0){
        printf("operação de remoção do sprite falhou");
        close_physical (fd);
        return bytesRead;
    }
    close_physical (fd);
    return 0;   
}

Background* setBackground(unsigned char B, unsigned char G, unsigned char R) {
    unsigned char *palavra = assembleInstructionWBR(0b00000, R, G, B);

    int fd;
    if ((fd = open_physical (fd)) == -1)
        return NULL;

    ssize_t buffer =  write(fd,palavra,sizeof(palavra));

   unsigned char lendo[8];
   ssize_t bytesRead=read(fd,lendo,sizeof(lendo));
    if (bytesRead<0){
        printf("falhou");
        close_physical (fd);
        return NULL;
    }

    close_physical (fd);
    Background *bg = (Background *)malloc(sizeof(Background));
    bg->address = 0;
    bg->R = R;
    bg->G = G;
    bg->B = B;
    return bg;   
}

int eraseBackground() {
    return (setBackground(0, 0, 0) != NULL);
}

int setPolygon(unsigned char rel_x, unsigned char rel_y, Polygon* polygon) {
    unsigned char *palavra;
    if (lastRegister <= 31)
        palavra = assembleInstructionDP(rel_x, rel_y, ++lastRegister, *polygon);
    else {
        printf("Número máximo de registradores utilizados atingido.");
        return -1;
    }
    
    int fd;
    if ((fd = open_physical (fd)) == -1)
        return (-1);

    ssize_t buffer =  write(fd,palavra,sizeof(palavra));

    unsigned char lendo[8];
    ssize_t bytesRead=read(fd,lendo,sizeof(lendo));
    if (bytesRead<0) {  
        printf("erro na escrita de um polígono");
        close_physical(fd);
        return bytesRead;
    }

    polygon->rel_x = rel_x;
    polygon->rel_y = rel_y;
    polygon->address = lastRegister;

    close_physical (fd);
    return 0;   
}

int main() {
    
    return 0;
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

