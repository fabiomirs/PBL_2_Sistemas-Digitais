#include "instructions.h"
#include "datastructures.h"
#include <stdint.h>
#include <stdio.h>

//CONSTANTES
#define WBR_OPCODE 0b0000
#define WSM_OPCODE 0b0001
#define WBM_OPCODE 0b0010
#define DP_OPCODE  0b0011
#define BACKGROUND_REG 0b0000

//as seguintes funções retornam um ponteiro para a primeisa posição de um vetor de 8 char's,
//que são organizados no kernel de modo a gerar separadamente as strings para serem enviadas ao DATA_A e ao DATA_B da GPU

unsigned char *assembleInstructionWBR(
    unsigned char R, unsigned char G, unsigned char B) {

    static unsigned char palavra[8] = {0};


    //no primeiro byte, 4 bits menos significativos do byte que terá o opcode
    palavra[0] = (WBR_OPCODE & 0xF);
    
    //no primeiro byte, 4 dos 5 bits relativos ao registrador
    palavra[0] |= (BACKGROUND_REG & 0xF) << 4;
    
    //bit restante do registrador
    palavra[1] |= (BACKGROUND_REG & 0x10) >> 4;


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


    //no primeiro byte, os 4 bits menos significativos do byte que terá o opcode
    palavra[0] = (WBR_OPCODE & 0xF);
    
    //no primeiro byte, 4 dos 5 bits relativos ao registrador
    palavra[0] |= (reg & 0xF) << 4;
    
    //bit restante do registrador
    palavra[1] |= (reg & 0x10) >> 4;

    // 8 bits menos significativos do offset
    palavra[4] = (offset & 0xFF);

    // bit mais significativo do offset no byte seguinte do buffer
    palavra[5] = (offset & 0x100) >> 8;

    //7 bits menos significativos da coordenada y onde o sprite é escrito
    palavra[5] |= (Coord_y & 0x7F) << 1;

    //3 bits mais significativos da coordenada y onde o sprite é escrito	
    palavra[6] = (Coord_y & 0x380) >> 7;

    //5 bits menos significativos da coordenada x onde o sprite é escrito
    palavra[6] |= (Coord_x & 0x1F) << 3;
    
    //3 bits mais significativos da coordenada x onde o sprite é escrito
    palavra[7] = (Coord_x & 0x3E0) >> 5;

    //bit de visibilidade do sprite
    palavra[7] |= (sp & 0x1) << 5;
   
    return palavra;
}


unsigned char* assembleInstructionWBM(
    unsigned int address, unsigned char R, unsigned char G, unsigned char B){


    static unsigned char palavra[8] = {0};


    //no primeiro byte, os 4 bits menos significativos do byte que terá o opcode
    palavra[0] = (WBM_OPCODE & 0xF);
    
    //4 bits menos significativos do endereço do bloco a ser modificado
    palavra[0] |= (address & 0xF) << 4;
    
    //8 dos 12 bits menos significativos do endereço do bloco a ser modificado
    palavra[1] =  (address & 0xFF0) >> 4;
	
    //2 bits mais significativos do endereço do bloco a ser modificado
    palavra[2] = (address & 0x3000) >> 12;
	
    //3 bits do pigmento de cor R
    palavra[4] |= (R & 0x07); 
	
    //3 bits do pigmento de cor G
    palavra[4] |= (G & 0x07) << 3;
	
    //2 bits menos significativos do pigmento de cor B
    palavra[4] |= (B & 0x03) << 6;
	
    //bit mais significativo do pigmento de cor B
    palavra[5] |= (B & 0x04) >> 2;  
   
    return palavra;
}


unsigned char* assembleInstructionWSM(
		unsigned int address, unsigned char R, unsigned char G, unsigned char B){


    static unsigned char palavra[8] = {0};


    //no primeiro byte, 4 bits menos significativos do byte que terá o opcode
    palavra[0] = (WSM_OPCODE & 0xF);
    
    //4 bits menos significativos do endereço do bloco a ser modificado
    palavra[0] |= (address & 0xF) << 4;

    //8 dos 12 bits menos significativos do endereço do bloco a ser modificado
    palavra[1] =  (address & 0xFF0) >> 4;

    //2 bits mais significativos do endereço do bloco a ser modificado
    palavra[2] = (address & 0x3000) >> 12;

    //3 bits do pigmento de cor R
    palavra[4] |= (R & 0x07); 

    //3 bits do pigmento de cor G
    palavra[4] |= (G & 0x07) << 3; 

    //2 bits menos significativos do pigmento de cor B
    palavra[4] |= (B & 0x03) << 6; 

    //bit mais significativo do pigmento de cor B
    palavra[5] |= (B & 0x04) >> 2;  
   
    return palavra;
}

unsigned char* assembleInstructionDP (
		unsigned int ref_point_X, unsigned int ref_point_Y, unsigned char address,
    unsigned char size, unsigned char R, unsigned char G, unsigned char B, unsigned char shape) {
    static char palavra[8] = {0};
	
    //no primeiro byte, 4 bits menos significativos do byte que terá o opcode
    palavra[0] = DP_OPCODE;

    //bits de endereçamento do DP
    palavra[0] |= (address & 0xF) << 4;

    //8 bits menos significativos da coordenada X 
    palavra[4] = (ref_point_X & 0xFF);

    //bit mais significativo da coordenada X 
    palavra[5] = (ref_point_X & 0x100) >> 8;

    //7 bits menos significativos da coordenada Y 
    palavra[5] |= (ref_point_Y & 0x7F) << 1;

    //2 bits mais significativos da coordenada Y
    palavra[6] = (ref_point_Y & 0x180) >> 7;

    // 4 bits de tamanho do polígono
    palavra[6] |= (size & 0xF) << 2;

    //2 bits menos significativos do pigmento de cor R
    palavra[6] |= (R & 0x3) << 6;
	
    //bit mais significativo do pigmento de cor R
    palavra[7] = (R & 0x4) >> 2;

    //3 bits do pigmento de cor G
    palavra[7] |= (G) << 1;

    //3 bits do pigmento de cor B
    palavra[7] |= (B) << 4;

    //bit do formato do polígono (quadrado ou triângulo)
    palavra[7] |= (shape) << 7;

   return palavra;
}
