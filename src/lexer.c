#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

static const TokenMapEntry keywords[] = {
  {"select", TOKEN_SELECT},
  {"from", TOKEN_FROM},
  {"as", TOKEN_AS},
  {"create", TOKEN_CREATE},
  {"table", TOKEN_TABLE},
  {"drop", TOKEN_DROP},
  {"distinct", TOKEN_DISTINCT},
  {"top", TOKEN_TOP},
  {"percent", TOKEN_PERCENT},
  {"where", TOKEN_WHERE},
  {"and", TOKEN_AND},
  {"or", TOKEN_OR},
  {"not", TOKEN_NOT},
  {"is", TOKEN_IS},
  {"null", TOKEN_NULL},
  {"insert", TOKEN_INSERT},
  {"into", TOKEN_INTO},
  {"values", TOKEN_VALUES},
  {"delete", TOKEN_DELETE},
  {NULL, TOKEN_NONE},
};

static const TokenMapEntry single_symbols[] = {
  {"*", TOKEN_STAR},
  {",", TOKEN_COMMA},
  {"(", TOKEN_LEFT_PAREN},
  {")", TOKEN_RIGHT_PAREN},
  {";", TOKEN_SEMI_COLON},
  {"\'", TOKEN_SINGLE_QUOTE},
  {"=", TOKEN_SINGLE_EQUALS},
  {">", TOKEN_GREATER_THAN},
  {"<", TOKEN_LESS_THAN},
  {NULL, TOKEN_NONE},
};

static const TokenMapEntry double_symbols[] = {
  {">=", TOKEN_GREATER_THAN_EQUALS},
  {"<=", TOKEN_LESS_THAN_EQUALS},
  {"<>", TOKEN_NOT_EQUAL},
  {NULL, TOKEN_NONE},
};

static void lexer_out(LexerState *lexer) {
  printf("Lexer Result:\n");
  printf("  token count:    %d\n", lexer->token_count);
  printf("  token capacity: %d\n", lexer->token_capacity);
  printf("  lexer current:  %d\n", lexer->current);
  printf("  source length:  %lld\n", strlen(lexer->source));
  printf("\nToken List:\n");

  for (int i = 0; i < lexer->token_count; i++) {
    Token token = lexer->tokens[i];
    printf("  %d| '%s': %s, line %d\n", i, token.lexeme, token_type_to_str(token.type), token.line);
  }
  printf("\n");
}

void free_lexer(LexerState *lexer) {
  if (!lexer) return;

  for (int i = 0; i < lexer->token_count; i++) {
    free(lexer->tokens[i].lexeme);
  }

  free(lexer->tokens);
  free(lexer->source);
  free(lexer);
}

static Token *init_token(LexerState *lexer, char *lexeme, TokenType type) {
  Token *token = (Token *)malloc(sizeof(Token));
  token->lexeme = strdup(lexeme);
  token->type = type;
  token->line = lexer->line;

  return token;
}

static LexerState *init_lexer(char *source) {
  LexerState *lexer = (LexerState *)malloc(sizeof(LexerState));
  lexer->source = strdup(source);
  lexer->token_count = 0;
  lexer->token_capacity = 1;
  lexer->current = 0;
  lexer->line = 1;
  lexer->tokens = (Token *)malloc(sizeof(Token) * lexer->token_capacity);

  return lexer;
}

static void add_token(LexerState *lexer, Token *token) {
  if (lexer->token_count >= lexer->token_capacity) {
    lexer->token_capacity *= 2;
    lexer->tokens = (Token *)realloc(lexer->tokens, sizeof(Token) * lexer->token_capacity);
  }

  lexer->tokens[lexer->token_count] = *token;
  lexer->token_count++;
}

static Token *bad_token(LexerState *lexer) {
  return init_token(lexer, "", TOKEN_BAD);
}

static inline int is_end(LexerState *lexer) {
  if (lexer->current < (int)strlen(lexer->source)) {
    return 0;
  }

  return 1;
}

static inline void advance(LexerState *lexer) {
  lexer->current++;
}

static inline char current(LexerState *lexer) {
  return lexer->source[lexer->current];
}

static Token* parse_identifier(LexerState *lexer) {
  int start = lexer->current;

  while (!is_end(lexer) && (isalpha(current(lexer)) || current(lexer) == '_')) {
    advance(lexer);
  }

  unsigned long size = lexer->current - start;
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

  Token *token = init_token(lexer, lexeme, type);
  free(lexeme);
  
  return token;
}

static Token *parse_numeric(LexerState *lexer) {
  int start = lexer->current;

  while (!is_end(lexer) && isdigit(current(lexer))) {
    advance(lexer);
  }

  unsigned long size = lexer->current - start;
  char *lexeme = malloc(size + 1);
  strncpy(lexeme, lexer->source + start, size);
  lexeme[size] = '\0';

  Token *token = init_token(lexer, lexeme, TOKEN_NUMERIC);
  return token;
}

static Token *parse_symbol(LexerState *lexer) {
  char symbol[3] = { current(lexer), '\0', '\0' };
  advance(lexer);

  if (!is_end(lexer)) {
    symbol[1] = current(lexer);
    symbol[2] = '\0';
    advance(lexer);

    for (int i = 0; double_symbols[i].value != NULL; i++) {
      if (strcmp(double_symbols[i].value, symbol) == 0) {
        return init_token(lexer, symbol, double_symbols[i].type);
      }
    }
    
    symbol[1] = '\0';
    lexer->current--;
  }

  for (int i = 0; single_symbols[i].value != NULL; i++) {
    if (strcmp(single_symbols[i].value, symbol) == 0) {
      return init_token(lexer, symbol, single_symbols[i].type);
    }
  }

  return bad_token(lexer);
}

static Token *parse_token(LexerState *lexer) {
  if (isalpha(current(lexer))) {
    return parse_identifier(lexer);
  }
  else if (isdigit(current(lexer))) {
    return parse_numeric(lexer);
  }

  return parse_symbol(lexer);
}

LexerState *tokenize(int debug, char *source) {
  for(int i = 0; source[i]; i++){
    source[i] = tolower(source[i]);
  }

  LexerState *lexer = init_lexer(source);

  while (!is_end(lexer)) {
    while (!is_end(lexer) && isspace(current(lexer))) {
      if (current(lexer) == '\n') lexer->line++;
      advance(lexer);
    }
    if (is_end(lexer)) break;
    
    Token *token = parse_token(lexer);
    add_token(lexer, token);
  }

  add_token(lexer, init_token(lexer, "EOF", TOKEN_EOF));

  if (debug) lexer_out(lexer);

  return lexer;
}