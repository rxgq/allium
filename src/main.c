#include <stdio.h>
#include <fcntl.h>

#include "sql/lexer.h"

int main() {
  char *x = "select";

  LexerState* lexer = init_lexer(x);
  tokenize(lexer);



  return 0;
}