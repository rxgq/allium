#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include "parser.h"
#include "tree.h"

/*
** Forward definitions of expression structs
*/
typedef struct ParserState ParserState;
typedef struct SqlExpr SqlExpr;

/*
** Enum to represent the different types of valid SQL expressions
*/
typedef enum {
  EXPR_CREATE_TABLE_STMT,
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
} ColumnDefinition;

typedef struct {
  char *value;
} AllColumns;

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
  unsigned int options_count;
} SelectClause;

typedef struct {
  SqlExpr *clauses;
} SelectStmt;

typedef struct {
  SqlExpr *name;
  ColumnDefinition *columns;
  int column_count;
} CreateTableStmt;


/*
** Data structure for representing a single SQL statement or clause.
*/
struct SqlExpr {
  SqlExprType type;

  union {
    CreateTableStmt create_table;
    SelectStmt select;
    
    SelectClause select_clause;
    FromClause from_clause;

    AliasExpr alias;
    AllColumns all_columns;

    IdentifierExpr identifier;
  } as;
};

/*
** Represents the top level data structure that holds the sql statements.
*/
typedef struct {
  unsigned int statement_count;
  unsigned int statement_capacity;
  SqlExpr* statements;
} SqlQueryTree;

extern SqlExpr *init_expr(SqlExprType type);
extern SqlQueryTree *init_sql_tree();
extern void parser_out(ParserState *parser);

#endif