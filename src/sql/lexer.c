#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

LexerState *init_lexer(char *source) {
  LexerState *lexer = (LexerState *)malloc(sizeof(LexerState));
  lexer->source = source;
  lexer->token_count = 0;
  lexer->token_capacity = 1;
  lexer->current = 0;
  lexer->tokens = (Token *)malloc(sizeof(Token) * lexer->token_capacity);

  return lexer;
}

static inline void advance(LexerState *lexer) {
  lexer->current++;
}

static Token* parse_identifier(LexerState *lexer) {
  
  
  Token *token = init_token("", IDENTIFIER);
  return token;
}

static Token *parse_token(LexerState *lexer) {
  return parse_identifier(lexer);
}

void tokenize(LexerState *lexer) {
  while (lexer->current < strlen(lexer->source)) {
    Token *token = parse_token(lexer);
    lexer->tokens[lexer->token_count] = *token;
    lexer->token_count++;

    lexer->current++;
    break;
  }

  for (int i = 0; i < lexer->token_count; i++) {
    printf("%s", lexer->tokens[i].lexeme);
  }
}
