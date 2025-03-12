#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "lexer.h"
#include "parser.h"
#include "db.h"
#include "codes.h"

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

  ParserState *parser = parse_ast(lexer->tokens, lexer->token_count);
  free_lexer(lexer);

  AlliumCode result = execute(parser->ast);
  if (result != ALLIUM_SUCCESS) {
    printf("execution failed");
    return 1;
  }

  printf("query completed");

  free_parser(parser);
  fclose(fptr);

  return 0;
}