/* Louis POTTIER 2411430 3WB */
/* Lucas Rodrigues 1912905 3WA */

#include <stdio.h>
#include "converteUtf.h"

// Função que converte Big Endian para Little Endian
unsigned int convbigtolittle(unsigned int codepoint, unsigned int bytes);

// Função que verifica o valor do BOM
int get_BomValue(unsigned int bom);

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida) {
    unsigned int BOM = 0xFFFE0000; // Little Endian

    // Escrevendo o BOM ao início do arquivo
    if (fwrite(&BOM, sizeof(BOM), 1, arquivo_saida) != 1) {
        printf("Erro na escritura do BOM\n");
        fclose(arquivo_saida);
        return 1;
    }

    int c; // Para a leitura
    unsigned int bytes = 0; // Para conhecer o número de bytes na codificação UTF-8
    unsigned int codepoint = 0; // Para extrair o caractere codificado UTF-32

    while ((c = fgetc(arquivo_entrada)) != EOF) {
        if ((c & 0x80) == 0x00) {
            bytes = 1;
            codepoint = c;
        } else if ((c & 0xE0) == 0xC0) {
            bytes = 2;
            codepoint = (c & 0x1F) << 6;
        } else if ((c & 0xF0) == 0xE0) {
            bytes = 3;
            codepoint = (c & 0x0F) << 12; // Corrigido para o deslocamento correto
        } else if ((c & 0xF8) == 0xF0) {
            bytes = 4;
            codepoint = (c & 0x07) << 18; // Corrigido para o deslocamento correto
        }

        for (int i = 1; i < bytes; i++) {
            c = fgetc(arquivo_entrada); // 10xx xxxx
            if (c == EOF) {
                printf("Erro: arquivo de entrada terminou inesperadamente.\n");
                return 1;
            }
            codepoint = (codepoint << 6) | (c & 0x3F); // Ajustado o cálculo do codepoint
        }

        // Conversão do Big Endian para o Little Endian
        codepoint = convbigtolittle(codepoint, bytes);

        // Escrita no arquivo de saída
        if (fwrite(&codepoint, sizeof(codepoint), 1, arquivo_saida) != 1) {
            printf("Erro na escritura\n");
            return 0;
        }
    }
    return 1;
}

int convUtf32p8(FILE *arq_entrada, FILE *arq_saida) {
    unsigned int inBuffer;
    unsigned int bom;
    unsigned long currentBytes;
    unsigned long writtenBytes;
    unsigned char outBuffer[4] = {0};
    int bytesCounter;

    // Leitura do BOM
    if (fread(&inBuffer, sizeof(unsigned int), 1, arq_entrada) != 1) {
        fprintf(stderr, "\nErro E/S: leitura do arquivo de entrada");
        return -1;
    }

    // Verificação do BOM e conversão de endian se necessário
    bom = get_BomValue(inBuffer);
    
    if (bom == -1) {
        fprintf(stderr, "\nErro E/S: leitura do caractere BOM");
        return -1;
    } else if (bom == 2) {
        // Se for Little Endian, converte para Big Endian
        inBuffer = convbigtolittle(inBuffer, 4);
    }

    // Loop para conversão dos caracteres
    while ((currentBytes = fread(&inBuffer, sizeof(unsigned int), 1, arq_entrada)) == 1) {
        // Se o arquivo for Little Endian, converte para Big Endian
        if (bom == 2) {
            inBuffer = convbigtolittle(inBuffer, 4);
        }

        // Calcula quantos bytes são necessários para representar o codepoint em UTF-8
        if (inBuffer <= 0x7F) {
            bytesCounter = 1;
            outBuffer[0] = (unsigned char) inBuffer;
        } else if (inBuffer <= 0x7FF) {
            bytesCounter = 2;
            outBuffer[0] = 0xC0 | ((inBuffer >> 6) & 0x1F);
            outBuffer[1] = 0x80 | (inBuffer & 0x3F);
        } else if (inBuffer <= 0xFFFF) {
            bytesCounter = 3;
            outBuffer[0] = 0xE0 | ((inBuffer >> 12) & 0x0F);
            outBuffer[1] = 0x80 | ((inBuffer >> 6) & 0x3F);
            outBuffer[2] = 0x80 | (inBuffer & 0x3F);
        } else if (inBuffer <= 0x10FFFF) {
            bytesCounter = 4;
            outBuffer[0] = 0xF0 | ((inBuffer >> 18) & 0x07);
            outBuffer[1] = 0x80 | ((inBuffer >> 12) & 0x3F);
            outBuffer[2] = 0x80 | ((inBuffer >> 6) & 0x3F);
            outBuffer[3] = 0x80 | (inBuffer & 0x3F);
        } else {
            fprintf(stderr, "\nErro: codepoint inválido");
            return -1;
        }

        // Escrita do caractere convertido no arquivo de saída
        writtenBytes = fwrite(outBuffer, sizeof(unsigned char), bytesCounter, arq_saida);
        if (writtenBytes != bytesCounter) {
            fprintf(stderr, "Erro E/S: escrita do arquivo de saída");
            return -1;
        }
    }

    if (ferror(arq_entrada)) {
        fprintf(stderr, "Erro E/S durante a leitura do arquivo de entrada");
        return -1;
    }

    printf("Conversão de UTF-32 para UTF-8 bem-sucedida\n");
    return 0;
}

unsigned int convbigtolittle(unsigned int codepoint, unsigned int bytes) {
    unsigned int result = 0;

    for (unsigned int i = 0; i < bytes; i++) {
        result |= (codepoint & 0xFF) << (i * 8);
        codepoint >>= 8;
    }

    return result;
}

int get_BomValue(unsigned int bom) {
    if (bom == 0xFEFF)
        return 1; // Big Endian
    if (bom == 0xFFFE0000)
        return 2; // Little Endian
    return -1; // Nenhum BOM reconhecido
}
