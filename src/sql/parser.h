#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "expr.h"

typedef struct {
  int current;
  Token *tokens;
  SqlQueryTree *ast;
} ParserState;

extern SqlQueryTree* parse_ast(Token* tokens);

#endif