#include <stdio.h>
#include "converteutf832.h"

int main(){
  FILE *arquivo_entrada = fopen("ent.txt", "rb");
  FILE *arquivo_saida = fopen("saida.txt", "wb");
  int v = convUtf8p32(arquivo_entrada, arquivo_saida);
  printf("La valeur de v est: %d\n", v);
  fclose(arquivo_entrada);
  fclose(arquivo_saida);
  return 0; 
}
