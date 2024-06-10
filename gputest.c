#include "lib/gpulib.h"
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main() {
    // Color color = {0b000, 0b111, 0b000};
    // setBackground(color);
    Sprite sprite;
    sprite.address = 1;
    sprite.rel_x= 150;
    sprite.rel_y= 70;
    sprite.variation = ALIEN_1;
    sprite.visible = 1;

    int i, j;

    
    
    // for(i = 0; i <= 32; i++){
    //     sprite.address = i;
    //     sprite.visible = 0;
    //     setSpriteOnScreen(sprite);
    // }

    sprite.address = 1;
    sprite.visible = 1;
    sprite.rel_x= 150;
    for(i = 0; i< 8; i++){
        setSpriteOnScreen(sprite);
        sprite.address++;
        sprite.rel_x +=40;
    }
    sprite.rel_y += 40;
    sprite.rel_x= 150;
    for(i = 0; i< 8; i++){
        setSpriteOnScreen(sprite);
        sprite.address++;
        sprite.rel_x += 40;
    }
    sprite.rel_y += 40;
    sprite.rel_x= 150;
    for(i = 0; i< 8; i++){
        setSpriteOnScreen(sprite);
        sprite.address++;
        sprite.rel_x += 40;
    }

    Color star = {7,7,7};

    setPixelOnSpriteMemory(25*400, star);
    Sprite plane;
    plane.address = 27;
    plane.rel_x= 310;
    plane.rel_y= 450;
    plane.variation = PLANE_UP;
    plane.visible = 1;

    setSpriteOnScreen(plane);

    Sprite beam;
    beam.address = 26;
    beam.rel_x = 310;
    beam.rel_y = 420;
    beam.variation = BEAM_VERTICAL;
    beam.visible = 1;

    setSpriteOnScreen(beam);

    Polygon p;
    p.address = 0;
    p.rel_x = 450;
    p.rel_y = 350;
    p.size = s40x40;
    Color pColor = {0, 7, 0};
    p.color = pColor;
    p.shape = TRIANGLE;

    setPolygon(p);

    for(i = 1; i < 4; i++){
        p.address = i;
        p.rel_x -= 100;
        setPolygon(p);
    }


    srand(time(NULL));

    Color sky = {0,0,1};

    setPixelOnSpriteMemory(ALIEN_1*400, sky);
    
    for(i = 0; i < 4800; i++)
        editBlockOnBackgroundMemory(i, sky);

    for(i = 0; i <= 20; i++){
        editBlockOnBackgroundMemory(rand() % (1200 + 1), star);
    }
    for(i = 0; i <= 20; i++){
        editBlockOnBackgroundMemory(rand() % (2400 + 1 - 1200) + 1200, star);
    }
    for(i = 0; i <= 20; i++){
        editBlockOnBackgroundMemory(rand() % (3600 + 1 - 2400) + 2400, star);
    }
    for(i = 0; i <= 20; i++){
        editBlockOnBackgroundMemory(rand() % (4800 + 1 - 3600) + 3600, star);
    }


    
    return 0;
}

