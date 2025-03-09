#ifndef EXPR_H
#define EXPR_H

#define MAX_COLUMNS 256 // maximum number of columns per table

#include "token.h"

/*
** Forward definitions of expression structs
*/
typedef struct SqlExpr SqlExpr;

/*
** Enum to represent the different types of valid SQL expressions
*/
typedef enum {
  EXPR_SELECT_STMT,
  EXPR_SELECT_CLAUSE,
  EXPR_FROM_CLAUSE,
  EXPR_ALIAS,
  EXPR_IDENTIFIER,
  BAD_EXPR,
} SqlExprType;

typedef struct {
  char *value;
} IdentifierExpr;

typedef struct {
  SqlExpr *expr;
  IdentifierExpr *identifier;
} AliasExpr;

typedef struct {
  SqlExpr *expr;
} FromClause;

typedef struct {
  SqlExpr *options; // 'distinct', etc
} SelectClause;

typedef struct {
  SqlExpr *clauses;
} SelectStmt;

/*
** Data structure for representing a single SQL statement or clause.
*/
struct SqlExpr {
  SqlExprType type;

  union {
    SelectStmt select_stmt;
    
    SelectClause select_clause;
    FromClause from_clause;

    AliasExpr alias;

    IdentifierExpr identifier;
  } as;
};

/*
** Represents the top level data structure that holds the sql statements.
*/
typedef struct {
  int statement_count;
  int statement_capacity;
  SqlExpr* statements;
} SqlQueryTree;

extern SqlExpr *init_expr(SqlExprType type);
extern SqlQueryTree *init_sql_tree();

#endif