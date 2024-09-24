#include <stdio.h>
#include "converteutf832.h"

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida){
  
  int BOM = 0xFFFE0000; /*Little Endian*/
  if (fwrite(&BOM, sizeof(BOM), 1, arquivo_saida) != 1) {
    printf("Erro na escritura do BOM\n");
    fclose(arquivo_saida);
    return 1;
  }

  int c;
  unsigned int codepoint = 0;
  unsigned int bytes = 0;

  while ((c = fgetc(arquivo_entrada)) != EOF) {
    if ((c & 0x80) == 0x00){
      bytes = 1;
      codepoint = c;
    }

    else if ((c & 0xE0) == 0xC0){
      bytes = 2;
      codepoint = (c & 0x1F) << 6;
    }

    else if ((c & 0x0F0) == 0xE0){
      bytes = 3;
      codepoint = (c & 0x0F) << 6;      
    }

    else if ((c & 0xF8) == 0xF0){
      bytes = 4;
      codepoint = (c & 0x07) << 6;
    }

    for (int i=1; i<bytes; i++){
      
      c = fgetc(arquivo_entrada); /* 10xx xxxx */
      int aux = c | 0xC0; /* 11xx xxxx */
      aux = aux & 0x3F; /* 00xx xxxx */
      codepoint = codepoint | aux; /* ... XXxx xxxx */
      if (i != (bytes-1)){
        codepoint = codepoint << 6;
      }
    }

    codepoint = convbigtolittle(codepoint, bytes);


    if (fwrite(&codepoint, sizeof(codepoint), 1, arquivo_saida) != 1){
      printf("Erro na escritura\n");
      return 0;
    }
  
  }
  return 1;
}


unsigned int convbigtolittle(unsigned int codepoint, unsigned int bytes){
  int mask1, mask2, mask3;
  mask1 = (codepoint & 0x000000FF) << 24;
  mask2 = (codepoint & 0X0000FF00) << 8;
  mask3 = (codepoint & 0X00FF0000) >> 8;

  if (bytes == 1){
    codepoint = mask1;
  }

  if ((bytes == 2) | (bytes == 3)){
    codepoint = mask1 | mask2;
  }

  if (bytes == 4){
    codepoint = mask1 | mask2 | mask3;
  }

  return codepoint;
}
