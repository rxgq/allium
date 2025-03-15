#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "db.h"
#include "codes.h"
#include "file.h"

int run_db(AlliumDb *allium, char *query) {
  LexerState *lexer = tokenize(allium->debug, query);
  ParserState *parser = parse_ast(allium->debug, lexer->tokens, lexer->token_count);
  free_lexer(lexer);

  if (parser->error_token) {
    free_parser(parser);
    return ALLIUM_PARSER_FAIL;
  }

  AlliumCode result = execute_query(allium, parser->ast);
  if (result != ALLIUM_SUCCESS) {
    free_parser(parser);
    return ALLIUM_DB_FAIL;
  }

  printf("\nquery completed");
  free_parser(parser);

  return ALLIUM_SUCCESS;
}

void execute_file_query(AlliumDb *allium, char *filename) {
  char *content = read_file(filename);
  if (!content) {
    return;
  }

  char *query = strtok(content, ";");
  while (query) {
    while (isspace(*query)) query++;

    if (*query) {
      printf("\n\nexecuting: %s\n", query);
      run_db(allium, query);
    }
    query = strtok(NULL, ";");
  }

  free(content);
}

int main() {
  int debug = 1;
  AlliumDb *allium = init_allium(debug);

  char inp_buff[256];
  while (1) {
    printf("\n\n@user> ");
    fflush(stdout);
    
    if (fgets(inp_buff, sizeof(inp_buff), stdin) == NULL) {
      break;
    }
    size_t len = strlen(inp_buff);
    if (len > 0 && inp_buff[len - 1] == '\n') {
      inp_buff[len - 1] = '\0';
    }

    if (strcmp(inp_buff, "quit") == 0) {
      break;
    } 

    if (strncmp(inp_buff, ":read ", 6) == 0) {
      char *filename = inp_buff + 6;
      while (*filename == ' ') filename++;

      if (!has_sql_extension(filename)) {
        printf("only .sql files are allowed.\n");
        continue;
      }

      execute_file_query(allium, filename);
    } 

    run_db(allium, inp_buff);
  }

  free_allium(allium);

  return 0;
}