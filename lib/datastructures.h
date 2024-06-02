#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

typedef enum {
  BACKGROUND,
  SPRITE,
  POLYGON
} GraphicElementType;

typedef enum { // para definição do formato dos polígonos 
  SQUARE   = 0,
  TRIANGLE = 1
} Shape;

typedef enum size { // tamanhos disponíveis para os polígonos da gpu
  off      = 0b0000,
  s20x20   = 0b0001, 
  s30x30   = 0b0010,
  s40x40   = 0b0011, 
  s50x50   = 0b0100,
  s60x60   = 0b0101,
  s70x70   = 0b0110,
  s80x80   = 0b0111,
  s90x90   = 0b1000,
  s100x100 = 0b1001,
  s110x110 = 0b1010,
  s120x120 = 0b1011,
  s130x130 = 0b1100, 
  s140x140 = 0b1101,
  s150x150 = 0b1110,
  s160x160 = 0b1111
} Size;

typedef struct {
  unsigned char address;
  unsigned char R;
  unsigned char G;
  unsigned char B;
} Background;

typedef struct {
  unsigned char address;
  Size size;
  unsigned char R;
  unsigned char G;
  unsigned char B;
  Shape shape;
  unsigned char rel_x;
  unsigned char rel_y;
} Polygon;

typedef struct {
  unsigned char address;
  unsigned char variation;
  unsigned char rel_x;
  unsigned char rel_y;
} Sprite;

typedef struct {
  GraphicElementType type;
  union {
    Polygon;
    Sprite;
    Background;
  } data;
} GraphicElement;

#endif
