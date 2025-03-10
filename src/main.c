#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "sql/lexer.h"
#include "store/page.h"
#include "sql/parser.h"

int main() {
  FILE *fptr = fopen("source.txt", "r");
  if (!fptr) {
    perror("error opening file");
    return 1;
  }

  fseek(fptr, 0, SEEK_END);
  unsigned long sz = ftell(fptr);
  rewind(fptr);

  char *buff = malloc(sz + 1);
  size_t read_size = fread(buff, 1, sz, fptr);
  buff[sz] = '\0';

  if (read_size != sz) {
    buff[read_size] = '\0'; 
  }

  LexerState *lexer = tokenize(buff);
  free(buff);

  // SqlQueryTree *ast = parse_ast(lexer->tokens);

  fclose(fptr);

  // free_lexer(lexer);

  return 0;
}