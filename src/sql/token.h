#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
  TOKEN_SELECT,
  TOKEN_FROM,

  TOKEN_STAR,

  TOKEN_IDENTIFIER,
  TOKEN_BAD,

  // used as a delimeter in the keywords map
  TOKEN_NONE,
} TokenType;

typedef struct {
  char *lexeme;
  TokenType type;
} Token;

typedef struct {
  char *value;
  TokenType type;
} TokenMapEntry;

extern Token *init_token(char *lexeme, TokenType type);
extern char *token_type_to_str(TokenType type);

#endif