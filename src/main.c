#include <stdio.h>
#include <fcntl.h>

#include "sql/lexer.h"

int main() {
  char *x = "select * from this table";
  LexerState *lexer = tokenize(x);

  free_lexer(lexer);
  return 0;
}