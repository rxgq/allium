#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
  TOKEN_SELECT,
  TOKEN_FROM,
  TOKEN_AS,
  TOKEN_CREATE,
  TOKEN_TABLE,
  TOKEN_DROP,
  TOKEN_INSERT,
  TOKEN_INTO,
  TOKEN_VALUES,
  TOKEN_UPDATE,
  TOKEN_DELETE,
  TOKEN_SET,
  TOKEN_TOP,
  TOKEN_PERCENT,
  TOKEN_DISTINCT,
  TOKEN_WHERE,
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_NOT,
  TOKEN_IS,
  TOKEN_NULL,

  TOKEN_STAR,
  TOKEN_COMMA,
  TOKEN_LEFT_PAREN,
  TOKEN_RIGHT_PAREN,
  TOKEN_SEMI_COLON,
  TOKEN_SINGLE_EQUALS,
  TOKEN_SINGLE_QUOTE,

  TOKEN_GREATER_THAN,
  TOKEN_LESS_THAN,
  TOKEN_GREATER_THAN_EQUALS,
  TOKEN_LESS_THAN_EQUALS,
  TOKEN_NOT_EQUAL,

  TOKEN_IDENTIFIER,
  TOKEN_NUMERIC,
  TOKEN_BAD,

  // the last token in the parsed list
  TOKEN_EOF,

  // used as a delimeter in the keywords map
  TOKEN_NONE,
} TokenType;

typedef struct {
  char *lexeme;
  TokenType type;
  int line;
} Token;

typedef struct {
  char *value;
  TokenType type;
} TokenMapEntry;

extern char *token_type_to_str(TokenType type);

#endif