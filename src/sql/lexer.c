#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

static LexerState *lexer;

static const TokenMapEntry keywords[] = {
  {"select", TOKEN_SELECT},
  {"from", TOKEN_FROM},
  {"as", TOKEN_AS},
  {NULL, TOKEN_NONE},
};

static const TokenMapEntry symbols[] = {
  {"*", TOKEN_STAR},
  {",", TOKEN_COMMA},
};

static void lexer_out() {
  printf("Lexer Result:\n");
  printf("  token count:    %d\n", lexer->token_count);
  printf("  token capacity: %d\n", lexer->token_capacity);
  printf("  lexer current:  %d\n", lexer->current);
  printf("  source length:  %zd\n", strlen(lexer->source));
  printf("\nToken List:\n");

  for (int i = 0; i < lexer->token_count; i++) {
    Token token = lexer->tokens[i];
    printf("  %d| '%s': %s\n", i, token.lexeme, token_type_to_str(token.type));
  }
  printf("\n");
}

void free_lexer(LexerState *lexer) {
  for (int i = 0; i < lexer->token_count; i++) {
    free(lexer->tokens[i].lexeme);
  }

  free(lexer->tokens);
  free(lexer);
}

static void init_lexer(char *source) {
  lexer = (LexerState *)malloc(sizeof(LexerState));
  lexer->source = strdup(source);
  lexer->token_count = 0;
  lexer->token_capacity = 1;
  lexer->current = 0;
  lexer->tokens = (Token *)malloc(sizeof(Token) * lexer->token_capacity);
}

static void add_token(Token *token) {
  if (lexer->token_count >= lexer->token_capacity) {
    lexer->token_capacity *= 2;
    lexer->tokens = (Token *)realloc(lexer->tokens, sizeof(Token) * lexer->token_capacity);
  }

  lexer->tokens[lexer->token_count] = *token;
  lexer->token_count++;
}

static inline void advance() {
  lexer->current++;
}

static inline char current() {
  return lexer->source[lexer->current];
}

static Token* parse_identifier() {
  int start = lexer->current;

  while (isalpha(current())) {
    advance();
  }

  long size = lexer->current - start;
  char *lexeme = malloc(size + 1);
  strncpy(lexeme, lexer->source + start, size);
  lexeme[size] = '\0';

  TokenType type = TOKEN_IDENTIFIER;
  for (int i = 0; keywords[i].value != NULL; i++) {
    if (strcmp(keywords[i].value, lexeme) == 0) {
      type = keywords[i].type;
      break;
    }
  }

  Token *token = init_token(lexeme, type);
  free(lexeme);
  
  return token;
}

static Token *bad_token() {
  Token *token = (Token *)malloc(sizeof(Token));
  token->lexeme = strdup("");
  token->type = TOKEN_BAD;

  return token;
}

static Token *parse_symbol() {
  char symbol[] = { current(), '\0' };

  for (int i = 0; symbols[i].value != NULL; i++) {
    if (strcmp(symbols[i].value, symbol) == 0) {
      advance();
      return init_token(symbol, symbols[i].type);
    }
  }

  return bad_token();
}

static Token *parse_token() {
  while (current() == ' ') {
    advance();
  }

  if (isalpha(current())) {
    return parse_identifier();
  }

  return parse_symbol();
}

LexerState *tokenize(char *source) {
  init_lexer(source);

  while (lexer->current < strlen(lexer->source)) {
    Token *token = parse_token();
    add_token(token);
  }
  add_token(init_token("EOF", TOKEN_EOF));

  // lexer_out();

  return lexer;
}