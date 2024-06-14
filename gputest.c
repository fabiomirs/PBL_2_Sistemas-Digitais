#include "lib/gpulib.h"
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main() {
    //criando modelo de sprite do alien a ser replicado
    Sprite sprite;
    sprite.address = 1;
    sprite.rel_x= 150;
    sprite.rel_y= 70;
    sprite.variation = ALIEN_1;
    sprite.visible = 1;

    int i;

    //imprimindo 1° fileira de aliens
    for(i = 0; i< 8; i++){
        setSpriteOnScreen(sprite);
        sprite.address++;
        sprite.rel_x +=40;
    }

    //imprimindo 2° fileira de aliens
    sprite.rel_y += 40;
    sprite.rel_x= 150;

    for(i = 0; i< 8; i++){
        setSpriteOnScreen(sprite);
        sprite.address++;
        sprite.rel_x += 40;
    }

    //imprimindo 3° fileira de aliens
    sprite.rel_y += 40;
    sprite.rel_x= 150;

    for(i = 0; i< 8; i++){
        setSpriteOnScreen(sprite);
        sprite.address++;
        sprite.rel_x += 40;
    }

    Color star = {7,7,7}; //cor das estrelas (blocos de background a serem editados)
    
    
    //configurando avião do player
    Sprite plane;
    plane.address = 27;
    plane.rel_x= 310;
    plane.rel_y= 450;
    plane.variation = PLANE_UP;
    plane.visible = 1;

    setSpriteOnScreen(plane);

    //configurando tiro do avião do player
    Sprite beam;
    beam.address = 26;
    beam.rel_x = 310;
    beam.rel_y = 420;
    beam.variation = BEAM_VERTICAL;
    beam.visible = 1;

    setSpriteOnScreen(beam);

    //obstáculos triangulares entre player e aliens
    Polygon p;
    p.address = 0;
    p.rel_x = 450;
    p.rel_y = 350;
    p.size = s40x40;
    Color p_color = {0, 7, 0};
    p.color = p_color;
    p.shape = TRIANGLE;

    setPolygon(p);

    for(i = 1; i < 4; i++){
        p.address = i;
        p.rel_x -= 100;
        setPolygon(p);
    }


    Color sky = {0,0,1};

    //executando edição de pixel em memória de sprites
    setPixelOnSpriteMemory(ALIEN_1*400, sky);
    
    //algoritmo de aleatorização das estrelas no background
    srand(time(NULL));
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

