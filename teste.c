#include <stdio.h>
#include <errno.h>
#include "converteUtf.h"

#define ENTRADA8_FILENAME "/Users/lucasrodrigues/Documents/code/inf1018_T1/INF1018Trabalho1/arq_entrada8.txt"

#define ENTRADA32_FILENAME "/Users/lucasrodrigues/Documents/code/inf1018_T1/INF1018Trabalho1/arq_entrada32.txt"

#define SAIDA_FILENAME "/Users/lucasrodrigues/Documents/code/inf1018_T1/INF1018Trabalho1/arq_saida.txt"

int main (void) {
    FILE *entrada,*saida;
    errno = 0;
    
    entrada = fopen(ENTRADA8_FILENAME,"rb");
    saida = fopen(SAIDA_FILENAME, "wb");
    
    if (entrada == NULL) {
        printf("Error %d \n", errno);
        printf("Erro no arquivo de entrada\n");
        return 1;
    }
    
    convUtf8p32(entrada,saida);
    return 0;
}