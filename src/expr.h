#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include "parser.h"
#include "tree.h"

/*
** Enum to represent the different types of valid SQL expressions
*/
typedef enum {
  EXPR_CREATE_TABLE_STMT,
  EXPR_DROP_TABLE_STMT,
  EXPR_SELECT_STMT,
  EXPR_SELECT_CLAUSE,
  EXPR_FROM_CLAUSE,
  EXPR_WHERE_CLAUSE,
  EXPR_INSERT_STMT,
  EXPR_BINARY,
  EXPR_UNARY,
  EXPR_ALIAS,
  EXPR_IDENTIFIER,
  EXPR_NUMERIC,
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
  int value;
} NumericExpr;

typedef struct {
  SqlExpr *expr;
  SqlExpr *identifier;
} AliasExpr;

typedef struct {
  SqlExpr *expr;
} FromClause;

typedef struct {
  int is_distinct;
  SqlExpr *top_count;
  int is_top_percent; // either 'top x' or 'top x percent'
  SqlExpr *options;
  int options_count;
} SelectClause;

typedef struct {
  SqlExpr *clauses;
  int clause_count;
} SelectStmt;

typedef struct {
  SqlExpr *name;
} DropTableStmt;

typedef struct {
  SqlExpr **values;
  int value_count;
} InsertValues;

typedef struct {
  SqlExpr *table_name;
  SqlExpr **column_names;
  int column_count;

  InsertValues **rows;
  int row_count;
} InsertStmt;

typedef struct {
  SqlExpr *name;
  ColumnExpr *columns;
  int column_count;
} CreateTableStmt;

typedef enum {
  OP_AND,
  OP_OR,
  OP_NOT,
} Op;

typedef struct {
  SqlExpr *left;
  Op op;
  SqlExpr *right;
} BinaryExpr;

typedef struct {
  SqlExpr *expr;
  Op op;
} UnaryExpr;

typedef struct {
  SqlExpr *condition;
} WhereClause;

/*
** Data structure for representing a single SQL statement or clause.
*/
struct SqlExpr {
  SqlExprType type;

  union {
    CreateTableStmt create_table;
    DropTableStmt drop_table;
    SelectStmt select;
    InsertStmt insert;
    
    SelectClause select_clause;
    FromClause from_clause;
    WhereClause where_clause;

    AliasExpr alias;
    AllColumnsExpr all_columns;
    BinaryExpr binary_expr;
    UnaryExpr unary_expr;

    IdentifierExpr identifier;
    NumericExpr numeric;
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
extern char *expr_type_to_str(SqlExprType type);

#endif