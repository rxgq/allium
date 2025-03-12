#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "lexer.h"
#include "parser.h"
#include "db.h"
#include "codes.h"

char *read_file(char *path) {
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

  fclose(fptr);
  
  return buff;
}

int run_db(Database *db, char *query) {
  LexerState *lexer = tokenize(query);
  printf("test");

  ParserState *parser = parse_ast(lexer->tokens, lexer->token_count);
  free_lexer(lexer);

  AlliumCode result = execute(db, parser->ast);
  if (result != ALLIUM_SUCCESS) {
    printf("execution failed");
    return 1;
  }

  printf("query completed");

  free_parser(parser);
}

int main() {
  int exitRequested = 0;
  char inp_buff[256];

  Database *db = init_db();
    
  printf("allium db");
  while (!exitRequested) {
    printf("\n> ");
    fflush(stdout);
    
    if (fgets(inp_buff, sizeof(inp_buff), stdin) == NULL) {
      break;
    }

    size_t len = strlen(inp_buff);
    if (len > 0 && inp_buff[len - 1] == '\n') {
      inp_buff[len - 1] = '\0';
    }

    if (strcmp(inp_buff, "quit") == 0) {
      exitRequested = 1;
      continue;
    } 
    
    run_db(db, inp_buff);
  }

  return 0;
}