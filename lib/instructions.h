#include "datastructures.h"

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H


unsigned char *assembleInstructionWBR(
    unsigned char reg, unsigned char R, unsigned char G, unsigned char B);

unsigned char* assembleInstructionWBR_2(
    unsigned char reg, unsigned int offset,
    unsigned int Coord_y, unsigned int Coord_x, unsigned char sp);

unsigned char *assembleInstructionWSM(
    unsigned int address, unsigned char R, unsigned char G, unsigned char B);

unsigned char* assembleInstructionWBM(
    unsigned int address, unsigned char R, unsigned char G, unsigned char B);

unsigned char *assembleInstructionDP(
    unsigned int ref_point_X, unsigned int ref_point_Y, unsigned char address, Polygon polygon);

#endif
