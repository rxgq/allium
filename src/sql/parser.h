#ifndef PARSER_H
#define PARSER_H

#include "token.h"

typedef struct {
  int current;
  Token *tokens;
} ParserState;

extern ParserState* parse_ast(Token* tokens);

#endif