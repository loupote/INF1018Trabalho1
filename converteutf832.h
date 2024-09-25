#include <stdio.h>

/* UTF-8  para UTF-32 */

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida);
unsigned int convbigtolittle(unsigned int codepoint, unsigned int bytes);


/* UTF-32 para UTF-8 */
