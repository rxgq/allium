#include <stdio.h>
#include <fcntl.h>

#include "sql/lexer.h"
#include "store/page.h"
#include "sql/parser.h"

int main() {
  FILE *fptr = fopen("data.db", "w");
  if (!fptr) {
    perror("error opening file");
    return 1;
  }

  char *x = "create table test ( int x, int y )";
  LexerState *lexer = tokenize(x);

  SqlQueryTree *ast = parse_ast(lexer->tokens);

  fclose(fptr);

  free_lexer(lexer);

  return 0;
}