#include "instructions.h"
#include "datastructures.h"
#include <stdint.h>


#define WBR_OPCODE 0b0000
#define WSM_OPCODE 0b0001
#define WBM_OPCODE 0b0010
#define DP_OPCODE  0b0011

unsigned char *assembleInstructionWBR(
    unsigned char reg, unsigned char R, unsigned char G, unsigned char B) {


    static unsigned char palavra[8] = {0};


    //no primeiro byte, coloque os 4 bits menos significativos do byte que terá o opcode
    palavra[0] = (WBR_OPCODE & 0xF);
    
    //no primeiro byte, coloque 4 dos 5 bits relativos ao registrador
    palavra[0] |= (reg & 0xF) << 4;
    
    //bit restante do registrador
    palavra[1] |= (reg & 0x10) >> 4;


    /*no quinto byte(começa a segunda metade da palavra de 64 bits),
    coloque os 3bits do byte do R (tipo R = 00000111, so pega os 3 1's, usando uma mascara)*/


    palavra[4] |= (R & 0x07); //palavra[4] = 00000111 nesse momento.


    /*ainda no quinto byte, faz o mesmo que antes para o vermelho (digamos que seja R=00000001, so pega 001),
    mas deslocando 3 para o lado e juntando*/
    palavra[4] |= (G & 0x07) << 3; //palavra[4] = 00001111 nesse momento.


    /*ainda no quinto byte, faz o mesmo que antes para o azul (digamos que seja R=00000111,
    so pega agora os 2 1's iniciais, por causa da nova mascara 0x03)
    */
    palavra[4] |= (B & 0x03) << 6; //palavra[4]=11001111 (so pegou 2 bits do B, sobra um pro proximo byte)


    /* o sexto byte vai pegar somente o terceiro bit do azul */
    palavra[5] |= (B & 0x04) >> 2;  //palavra[5] = 00000001
   
    return palavra;
}

unsigned char* assembleInstructionWBR_2(
    unsigned char reg, unsigned int offset, unsigned int Coord_y, unsigned int Coord_x, unsigned char sp) {
    // atenção para os unsigned int, visto que unsigned chars não suportariam os 10 bits de Coord_x e Coord_y

   static unsigned char palavra[8] = {0};


    //no primeiro byte, coloque os 4 bits menos significativos do byte que terá o opcode
    palavra[0] = (WBR_OPCODE & 0xF);
    
    //no primeiro byte, coloque 4 dos 5 bits relativos ao registrador
    palavra[0] |= (reg & 0xF) << 4;
    
    //bit restante do registrador
    palavra[1] |= (reg & 0x10) >> 4;
    
    palavra[4] = (offset & 0xFF);
    
    palavra[5] = (offset & 0x100) >> 8;
    
    palavra[5] |= (Coord_y & 0x7F) << 1;
    
    palavra[6] = (Coord_y & 0x380) >> 7;
    
    palavra[6] |= (Coord_x & 0x1F) << 3;
   
    palavra[7] = (Coord_x & 0x3E0) >> 5;
   
    palavra[7] |= (sp & 0x1) << 5;
   
    return palavra;
}


unsigned char* assembleInstructionWBM(
    unsigned int address, unsigned char R, unsigned char G, unsigned char B){


    static unsigned char palavra[8] = {0};


    //no primeiro byte, coloque os 4 bits menos significativos do byte que terá o opcode
    palavra[0] = (WBM_OPCODE & 0xF);
    
   
    palavra[0] |= (address & 0xF) << 4;
    
    palavra[1] =  (address & 0xFF0) >> 4;

    palavra[4] |= (R & 0x07); 

    palavra[4] |= (G & 0x07) << 3; 

    palavra[4] |= (B & 0x03) << 6; 

    palavra[5] |= (B & 0x04) >> 2;  
   
    return palavra;
}


unsigned char* assembleInstructionWSM(
    unsigned int address, unsigned char R, unsigned char G, unsigned char B){


    static unsigned char palavra[8] = {0};


    //no primeiro byte, coloque os 4 bits menos significativos do byte que terá o opcode
    palavra[0] = (WSM_OPCODE & 0xF);
    
   
    palavra[0] |= (address & 0xF) << 4;
    
    palavra[1] =  (address & 0xFF0) >> 4;
    
    palavra[2] = (address & 0x3000) >> 12;


    palavra[4] |= (R & 0x07); 

    palavra[4] |= (G & 0x07) << 3; 

    palavra[4] |= (B & 0x03) << 6; 

    palavra[5] |= (B & 0x04) >> 2;  
   
    return palavra;
}

unsigned char* assembleInstructionDP (
    unsigned int ref_point_X, unsigned int ref_point_Y, unsigned char address, Polygon polygon){


    static unsigned char palavra[8] = {0};

    //no primeiro byte, coloque os 4 bits menos significativos do byte que terá o opcode
    palavra[0] = (DP_OPCODE & 0xF);
    
    //no primeiro byte, coloque 4 dos 5 bits relativos ao registrador
    palavra[0] |= (address & 0xF) << 4;
    
    //bit restante do registrador
    palavra[1] |= (address & 0x10) >> 4;
    
    palavra[4] = (ref_point_X & 0xFF);

    palavra[5] = (ref_point_X & 0x100) >> 8;

    palavra[5] |= (ref_point_Y & 0x7F) << 1;

    palavra[6] = (ref_point_Y & 0x180) >> 7;
    
    palavra[6] |= polygon.size << 2;
    
    palavra[6] |= (polygon.R & 0x3) << 6;
    
    palavra[7] = (polygon.R & 0x1);
    
    palavra[7] |= (polygon.G) << 1;
    
    palavra[7] |= (polygon.B) << 4;
    
    palavra[7] |= (polygon.shape) << 7;
    
   return palavra;
}

