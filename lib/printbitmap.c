#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpu.h"

int setNewSpriteOnMemory(unsigned char initial_mem_address) {
    const char *filename = "bitmap.gpu";
    size_t num_chars_to_read = 9;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    unsigned char *buffer = (unsigned char *)malloc((num_chars_to_read + 1) * sizeof(unsigned char));
    if (buffer == NULL) {
        perror("Erro ao alocar memória");
        fclose(file);
        return EXIT_FAILURE;
    }

    for(int i = 0; i < 400; i++){
        memset(buffer, 0, num_chars_to_read + 1);
        size_t num_read = fread(buffer, sizeof(unsigned char), num_chars_to_read, file);
        if (num_read != num_chars_to_read) {
            if (feof(file)) {
                printf("Fim do arquivo alcançado na primeira leitura.\n");
            } else if (ferror(file)) {
                perror("Erro ao ler o arquivo na primeira leitura");
            }   
        }
        printf("Caracteres lidos (como string): %s\n", buffer);
        unsigned char R = (*buffer & 0x1C0) >> 6;
        unsigned char G = (*buffer & 0x38) >> 3;
        unsigned char B = (*buffer & 0x7);
        setPixelOnSpriteMemory(initial_mem_address+i, R, G, B);
    
   }
}

int main() {
    setNewSpriteOnMemory(0);
    return EXIT_SUCCESS;
}
