#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "expr.h"

struct ParserState {
  unsigned int current;
  Token *tokens;
  Token *error_token; // the token that caused the error, null if no error
  SqlQueryTree *ast;
};

extern SqlQueryTree* parse_ast(Token* tokens);

#endif