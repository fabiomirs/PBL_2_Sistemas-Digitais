#include "lib/gpulib.h"
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main() {
    // Color color = {0b000, 0b111, 0b000};
    // setBackground(color);
    Sprite sprite;
    sprite.address = 1;
    sprite.rel_x= 60;
    sprite.rel_y= 40;
    sprite.variation = ALIEN_1;
    sprite.visible = 1;

    int i, j;

    
    
    for(i = 0; i <= 32; i++){
        sprite.address = i;
        sprite.visible = 0;
        setSpriteOnScreen(sprite);
    }

    sprite.address = 1;
    sprite.visible = 1;
    sprite.rel_x= 60;
    for(i = 0; i< 17; i++){
        setSpriteOnScreen(sprite);
        sprite.address++;
        sprite.rel_x +=20;

    }
    // sprite.rel_y += 40;
    // sprite.rel_x= 60;
    // for(i = 0; i< 10; i++){
    //     setSpriteOnScreen(sprite);
    //     sprite.address++;
    //     sprite.rel_x += 40;
    // }
    

    // setSpriteOnScreen(sprite);
    // sprite.address++;
    
    // sprite.rel_x += 40;
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    // setSpriteOnScreen(sprite);

    // sprite.rel_y += 40;
    // sprite.rel_x = 60;


    // sprite.visible = 1;
    // setSpriteOnScreen(sprite);
    // sprite.address++;

    // sprite.rel_x += 40;
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    
    // sprite.rel_x += 40;
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    // setSpriteOnScreen(sprite);
    // sprite.address++;
    // sprite.rel_x += 40;
    // setSpriteOnScreen(sprite);


    // for (i = 0; i < 10; i++) {
    //     sprite.rel_x = 40*i;
    //     for (j = 0; j < 3; j++){
    //         sprite.rel_y = 40*j;
    //         setSpriteOnScreen(sprite);
    //         sprite.address++;
    //     }

    // }


    // for(i = 0; i < 11; i++) {
    //     setSpriteOnScreen(sprite);
    //     sprite.rel_x += 40;
    //     sprite.address++;
    // }


    // sprite.rel_y = 80;
    // sprite.rel_x= 60;
    
    // for(i = 0; i < 10; i++) {
    //     setSpriteOnScreen(sprite);
    //     sprite.rel_x += 40;
    //     sprite.address++;
    // }

    // sprite.rel_y = 120;
    // sprite.rel_x= 60;
    
    // for(i = 0; i < 10; i++) {
    //     setSpriteOnScreen(sprite);
    //     sprite.rel_x += 40;
    //     sprite.address++;
    // }

    Color star = {7,7,7};

    setPixelOnSpriteMemory(25*400, star);
    Sprite starSprite;
    starSprite.address = 15;
    starSprite.rel_x= 310;
    starSprite.rel_y= 450;
    starSprite.variation = PLANE_UP;
    starSprite.visible = 1;

    setSpriteOnScreen(starSprite);



    // sprite.address = 31;
    // sprite.rel_x= 60;
    // sprite.rel_y= 40;
    // setSpriteOnScreen(sprite);


    // Polygon p;
    // p.address = 0b1;
    // p.rel_x = 511;
    // p.rel_y = 10;
    // p.size = 0b001;
    // Color pColor = {7, 0, 0};
    // p.color = pColor;
    // p.shape = TRIANGLE;
    // setPolygon(p);

    srand(time(NULL));

    Color sky = {0,0,1};


    setBackground(sky);
    
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

