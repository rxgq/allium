#ifndef TOKEN_H
#define TOKEN_H

typedef enum TokenType {
  SELECT,
  FROM,

  STAR,

  IDENTIFIER
} TokenType;

typedef struct Token {
  char *lexeme;
  TokenType type;
} Token;

extern Token *init_token(char *lexeme, TokenType type);

#endif