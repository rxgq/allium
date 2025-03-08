#ifndef EXPR_H
#define EXPR_H

#define MAX_COLUMNS 256 // maximum number of columns per table

#include "token.h"

/*
** Enum to represent the different types of valid SQL expressions
*/
typedef enum {
  EXPR_SELECT,
  EXPR_FROM,
} SqlExprType;


typedef struct {
  SqlExpr *clauses;
} SelectStmt;

typedef struct {
  SqlExpr *expr;
} FromClause;

typedef struct {
  Token *options; // 'distinct', etc
} SelectClause;

typedef struct {
  SqlExpr *expr;
  char *identifier;
} AliasExpr;

/*
** Data structure for representing a single SQL statement or clause.
*/
typedef struct {
  SqlExprType type;

  union {
    SelectStmt select_stmt;
    SelectClause select_clause;
    FromClause from_clause;
  } as;
} SqlExpr;

/*
** Represents the top level data structure that holds the sql statements.
*/
typedef struct {
  SqlExpr* statements;
} SqlQuery;

#endif