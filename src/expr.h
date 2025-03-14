#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include "parser.h"
#include "tree.h"

/*
** Forward definitions of expression structs
*/
// typedef struct SqlExpr SqlExpr;

/*
** Enum to represent the different types of valid SQL expressions
*/
typedef enum {
  EXPR_CREATE_TABLE_STMT,
  EXPR_DROP_TABLE_STMT,
  EXPR_SELECT_STMT,
  EXPR_SELECT_CLAUSE,
  EXPR_FROM_CLAUSE,
  EXPR_ALIAS,
  EXPR_IDENTIFIER,
  EXPR_ALL_COLUMNS,
  BAD_EXPR,
} SqlExprType;

typedef struct {
  char *type;
  char *name;
} ColumnExpr;

typedef struct {
  char *value;
} AllColumnsExpr;

typedef struct {
  char *value;
} IdentifierExpr;

typedef struct {
  SqlExpr *expr;
  SqlExpr *identifier;
} AliasExpr;

typedef struct {
  SqlExpr *expr;
} FromClause;

typedef struct {
  SqlExpr *options; // 'distinct', etc
  int options_count;
} SelectClause;

typedef struct {
  SqlExpr *clauses;
} SelectStmt;

typedef struct {
  SqlExpr *name;
} DropTableStmt;

typedef struct {
  SqlExpr *name;
  ColumnExpr *columns;
  int column_count;
} CreateTableStmt;


/*
** Data structure for representing a single SQL statement or clause.
*/
struct SqlExpr {
  SqlExprType type;

  union {
    CreateTableStmt create_table;
    DropTableStmt drop_table;
    SelectStmt select;
    
    SelectClause select_clause;
    FromClause from_clause;

    AliasExpr alias;
    AllColumnsExpr all_columns;

    IdentifierExpr identifier;
  } as;
};

/*
** Represents the top level data structure that holds the sql statements.
*/
struct SqlQueryTree {
  int statement_count;
  int statement_capacity;
  SqlExpr* statements;
};

extern SqlExpr *init_expr(SqlExprType type);
extern SqlQueryTree *init_sql_tree();

#endif