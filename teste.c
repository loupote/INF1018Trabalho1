#include <stdio.h>
#include <errno.h>
#include "converteUtf.h"

#define ENTRADA8_FILENAME "/Users/lucasrodrigues/Documents/code/inf1018_T1/INF1018Trabalho1/arq_entrada8.txt"

#define ENTRADA32_FILENAME "/Users/lucasrodrigues/Documents/code/inf1018_T1/INF1018Trabalho1/arq_entrada32.txt"

#define SAIDA_UTF_32_FILENAME "/Users/lucasrodrigues/Documents/code/inf1018_T1/INF1018Trabalho1/arq_saida8_32.txt"

#define SAIDA_UTF_8_FILENAME "/Users/lucasrodrigues/Documents/code/inf1018_T1/INF1018Trabalho1/arq_saida32_8.txt"

int main (void) {
    FILE *entradaUtf8, *entradaUtf32 ,*saida_Utf32, *saida_Utf8;
    errno = 0;
    
    entradaUtf8 = fopen(ENTRADA8_FILENAME,"rb");
    entradaUtf32 = fopen(ENTRADA32_FILENAME,"rb");
    saida_Utf32 = fopen(SAIDA_UTF_32_FILENAME, "wb");
    saida_Utf8 = fopen(SAIDA_UTF_8_FILENAME, "wb");
    
    if (entradaUtf8 == NULL) {
        printf("Error %d \n", errno);
        printf("Erro no arquivo de entradaUtf8\n");
        return 1;
    }
    
    convUtf8p32(entradaUtf8,saida_Utf32);

    convUtf32p8(entradaUtf8,saida_Utf8); /*provocando erro ao colocar arquivo de entrada do utf 8*/

    convUtf32p8(entradaUtf32,saida_Utf8);


    return 0;
}