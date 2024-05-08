#include <stdio.h>
#include <stdint.h>

void imprimirEmBinario(uint64_t instrucao) {
        for (int i = 63; i >= 0; i--) {
            printf("%lu", (instrucao >> i) & 1);
            if (i == 4 || i == 9 || i == 12 || i == 15 || i == 18) {
                printf(" ");
            }
        }
        printf("\n");
    }

uint64_t montarInstrucaoWBR(uint8_t opcode, uint8_t registrador, uint8_t R, uint8_t G, uint8_t B) {
    uint64_t instrucao = ((uint64_t)registrador << 4) | ((uint64_t)R << 9) | ((uint64_t)G << 12) | ((uint64_t)B << 15);
    return instrucao;
}

int main() {
    uint8_t opcode = 0, registrador = 31, R = 4, G = 1, B = 7;

    uint64_t instrucao = montarInstrucaoWBR(opcode, registrador, R, G, B);

    printf("Instrução WBR em formato binário: ");

    imprimirEmBinario(instrucao);

    return 0;
}
