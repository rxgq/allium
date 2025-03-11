#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "expr.h"

struct ParserState {
  unsigned int current;
  unsigned int token_count;
  Token *tokens;
  Token *error_token;   // the token that caused the error, null if no error
  SqlQueryTree *ast;
};

extern ParserState *parse_ast(Token* tokens, int token_count);
extern void free_parser(ParserState *parser);

#endif