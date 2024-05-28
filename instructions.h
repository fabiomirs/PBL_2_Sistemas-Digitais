//CONSTANTES
#define DATA_A  0x80
#define DATA_B  0x70
#define START 0xc0

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>


typedef enum shape {
    SQUARE = 0,
    TRIANGLE = 1
} Shape;

typedef enum size {
    off = 0b0000,
    s20x20 = 0b0001, 
    s30x30 = 0b0010,
    s40x40 = 0b0011, 
    s50x50 = 0b0100,
    s60x60 = 0b0101,
    s70x70 = 0b0110,
    s80x80 = 0b0111,
    s90x90 = 0b1000,
    s100x100 = 0b1001,
    s110x110 = 0b1010,
    s120x120 =0b1011,
    s130x130 = 0b1100, 
    s140x140 = 0b1101,
    s150x150 = 0b1110,
    s160x160 = 0b1111
} Size;

typedef struct polygon
{
  Size size;
  uint8_t R;
  uint8_t G;
  uint8_t B;
  Shape shape;

} Polygon;



void assembleInstructionWBR(volatile int *DATA_A_PTR, volatile int *DATA_B_PTR,
    uint8_t reg, uint8_t R, uint8_t G, uint8_t B);

void assembleInstructionWBR_2(volatile int *DATA_A_PTR, volatile int *DATA_B_PTR,
    uint8_t opcode, uint8_t reg, uint8_t offset, uint8_t Coord_y, uint8_t Coord_x, uint8_t sp);

void assembleInstructionWSM(volatile int *DATA_A_PTR, volatile int *DATA_B_PTR,
    uint8_t opcode, uint8_t address, uint8_t R, uint8_t G, uint8_t B);

void assembleInstructionDP(volatile int *DATA_A_PTR, volatile int *DATA_B_PTR,
    uint8_t ref_point_X, uint8_t ref_point_Y, uint8_t address, Polygon polygon);

#endif
