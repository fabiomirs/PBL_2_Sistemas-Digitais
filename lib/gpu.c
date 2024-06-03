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
unsigned char availableRegisters[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

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


    int fd;
    if ((fd = open_physical (fd)) == -1)
        return (-1);

    ssize_t buffer =  write(fd,palavra,sizeof(palavra));

    unsigned char lendo[8];
    ssize_t bytesRead=read(fd,lendo,sizeof(lendo));
    if (bytesRead<0) {
        printf("falhou na escrita do pixel.");
        close_physical (fd);
        return bytesRead;
    }

    close_physical (fd);
    return 0;   
}

int editBlockOnBackgroundMemory(unsigned int block, unsigned char R, 
                                unsigned char G, unsigned char B) {
    unsigned char *palavra = assembleInstructionWBM(block, R, G, B);

    int fd;
    if ((fd = open_physical (fd)) == -1)
        return (-1);

    ssize_t buffer =  write(fd,palavra,sizeof(palavra));

    unsigned char lendo[8];
    ssize_t bytesRead=read(fd,lendo,sizeof(lendo));
    if (bytesRead<0) {
        printf("falhou na escrita de blocos de background.");
        close_physical (fd);
        return bytesRead;
    }

    close_physical (fd);
    return 0;   
} 


Sprite* setSpriteOnScreen(unsigned char variation, unsigned int x, unsigned int y) {
    unsigned char *palavra;
    if (lastRegister <= 31) {
        lastRegister++;
        printf("\n valor de last: %d\n", lastRegister);
        palavra = assembleInstructionWBR_2(lastRegister, variation, x, y, 1);
    }
    else {
        printf("Número máximo de registradores utilizados atingido.");
        return NULL;
    }
    printBits(palavra);


    // printf("///////////////////////////////////////////////////////\n");
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
    // printBits(lendo);

    close_physical (fd);
    Sprite* sprite = (Sprite *)malloc(sizeof(Sprite));
    sprite->address = lastRegister;
    sprite->variation = variation;
    sprite->rel_x = x;
    sprite->rel_y = y;

    GraphicElement ge = { SPRITE, .data.s = sprite };

    screenElements[sprite->address] = ge;

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

    GraphicElement ge = {BACKGROUND, .data.b = bg };
    screenElements[0] = ge;

    return bg;   
}

int eraseBackground() {
    setBackground(0b000, 0b000, 0b000);
    return 0;
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

    ssize_t buffer = write(fd,palavra,sizeof(palavra));

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

    GraphicElement ge = { POLYGON, .data.p = polygon };

    screenElements[lastRegister] = ge;

    return 0;   
}

int updateSprite(Sprite* sprite){
    unsigned char *palavra;
        palavra = assembleInstructionWBR_2(sprite->address,
    sprite->variation, sprite->rel_x, sprite->rel_y, sprite->sp);

    int fd;
    if ((fd = open_physical (fd)) == -1)
        return -1;

    ssize_t buffer = write(fd,palavra,sizeof(palavra));

    unsigned char lendo[8];
    ssize_t bytesRead = read(fd,lendo,sizeof(lendo));
    if (bytesRead<0){
        printf("instrução de atualização de sprite falhou");
        close_physical (fd);
        return -1;
    }

    close_physical (fd);

    GraphicElement ge = { SPRITE, .data.s = sprite };

    screenElements[sprite->address] = ge;

    return 0;
}

int updateBackground(Background* background){
    Background* ret = setBackground(background->R, background->G, background->B);
    if(ret == NULL) return -1;
    background = ret;
    return 0;
}

int updatePolygon(Polygon* polygon){
    unsigned char *palavra;
    palavra = assembleInstructionDP(polygon->rel_x, polygon->rel_y, polygon->address, *polygon);
    
    int fd;
    if ((fd = open_physical (fd)) == -1)
        return (-1);

    ssize_t buffer = write(fd,palavra,sizeof(palavra));

    unsigned char lendo[8];
    ssize_t bytesRead=read(fd,lendo,sizeof(lendo));
    if (bytesRead<0) {  
        printf("erro na escrita de um polígono");
        close_physical(fd);
        return bytesRead;
    }

    close_physical (fd);

    GraphicElement ge = { POLYGON, .data = polygon };

    screenElements[polygon->address] = ge;

    return 0;   
}

int updateGraphicElement(GraphicElement ge) {
    switch(ge.type) {
        case SPRITE:
            updateSprite(ge.data.s);
            break;
        case BACKGROUND:
            updateBackground(ge.data.b);
            break;
        case POLYGON:
            updatePolygon(ge.data.p);
            break;
        default:
            printf("Elemento não corresponde a nenhum GraphicElementType.");
            return -1;
    }
    return 0;
}

int main() {
    Background* bg = setBackground(0b111, 0b111, 0b111);
    Sprite* sp = setSpriteOnScreen(1, 0b10, 0b110);
    Sprite* sp2 = setSpriteOnScreen(0, 0, 0);
    sp2 = setSpriteOnScreen(0, 0, 0);
    sp2 = setSpriteOnScreen(0, 0, 0);
    // Polygon* p = (Polygon*) malloc(sizeof(Polygon));
    // printf("%d", lastRegister);
    // p->size = s30x30;
    // p->R = 0b111;
    // p->G = 0b111;
    // p->B = 0b111;
    // p->shape = TRIANGLE;
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

