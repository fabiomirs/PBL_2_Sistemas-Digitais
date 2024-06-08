#include "datastructures.h"

int setBackground(Color color);

int setPixelOnSpriteMemory(unsigned int mem_address, Color color);

int editBlockOnBackgroundMemory(unsigned int block, Color color);

int setSpriteOnScreen(Sprite sprite);

int setPolygon(Polygon polygon);

int eraseBackground();