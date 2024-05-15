#include <stdio.h>
#include <stdint.h>

void imprimirEmBinario(uint64_t instrucao) {
        for (int i = 63; i >= 0; i--) {
            printf("%lu", (instrucao >> i) & 1);
            if (i == 4 || i == 8 || i == 17 || i==26 || i==30 || i==33 || i==36 || i==39) {
                printf(" ");
            }
        }
        printf("\n");
    }



uint64_t montarInstrucaoWBR(uint8_t opcode, uint8_t registrador, uint8_t R, uint8_t G, uint8_t B) {
    uint64_t instrucao =((uint64_t)opcode << 0) |((uint64_t)registrador << 4) | ((uint64_t)R << 9) | ((uint64_t)G << 12) | ((uint64_t)B << 15);
    return instrucao;
}


uint64_t montarInstrucaoWBR_2(uint8_t opcode, uint8_t registrador, uint8_t offset, uint8_t Cord_y, uint8_t Cord_x,uint8_t sp) {
    uint64_t instrucao = ((uint64_t)opcode << 0) | ((uint64_t)registrador << 4) | ((uint64_t)offset << 9) | ((uint64_t)Cord_y << 18) | ((uint64_t)Cord_x << 28)|((uint64_t)sp << 38);
    return instrucao;
}



uint64_t montarInstrucaoWSM(uint8_t opcode,uint8_t adress, uint8_t R, uint8_t G,uint8_t B) {
    uint64_t instrucao = ((uint64_t)opcode << 0) | ((uint64_t)adress << 4) | ((uint64_t)R << 18) | ((uint64_t)G << 21)|((uint64_t)B << 24);
    return instrucao;
}


uint64_t montarInstrucaoDP(uint8_t opcode,uint8_t adress, uint8_t ref_point_X, uint8_t ref_point_Y,uint8_t tamanho,uint8_t R, uint8_t G,uint8_t B,uint8_t forma) {
    uint64_t instrucao = ((uint64_t)opcode << 0) | ((uint64_t)adress << 4) | ((uint64_t)ref_point_X << 8) | ((uint64_t)ref_point_Y << 17)|((uint64_t)tamanho << 26)|((uint64_t)R << 30)|((uint64_t)G << 33)|((uint64_t)B << 36)|((uint64_t)forma << 39);
    return instrucao;
}




int main() {
    uint8_t opcode = 1, registrador = 31, R = 4, G = 1, B = 7,Cord_y=120, Cord_x=110,offset = 10,sp=1,adress = 8,ref_point_X=59,ref_point_Y=63,tamanho=6,forma=1;

    uint64_t instrucao = montarInstrucaoDP(opcode,adress,ref_point_X,ref_point_Y,tamanho,R,G,B,forma);

    printf("Instrução WBR em formato binário: ");

    imprimirEmBinario(instrucao);

    return 0;
}
