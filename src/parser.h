#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "tree.h"

typedef struct {
  unsigned int current;
  unsigned int token_count;
  Token *tokens;
  Token *error_token;   // the token that caused the error, null if no error
  SqlQueryTree *ast;
} ParserState;

extern ParserState *parse_ast(int debug, Token* tokens, int token_count);
extern void free_parser(ParserState *parser);
extern void parser_out(ParserState *parser);

#endif