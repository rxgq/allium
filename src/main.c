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
    return NULL;
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

int run_db(AlliumDb *allium, char *query) {
  LexerState *lexer = tokenize(allium->debug, query);

  ParserState *parser = parse_ast(allium->debug, lexer->tokens, lexer->token_count);
  free_lexer(lexer);

  if (parser->error_token) {
    return ALLIUM_PARSER_FAIL;
  }

  AlliumCode result = execute(allium, parser->ast);
  if (result != ALLIUM_SUCCESS) {
    return ALLIUM_DB_FAIL;
  }

  printf("\nquery completed");
  free_parser(parser);

  return ALLIUM_SUCCESS;
}

int main() {
  int exitRequested = 0;
  char inp_buff[256];

  int debug = 0;

  AlliumDb *allium_db = init_allium(debug);

  printf("allium db");
  while (!exitRequested) {
    printf("\n\n> ");
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
    
    AlliumCode result = run_db(allium_db, inp_buff);
  }

  return 0;
}