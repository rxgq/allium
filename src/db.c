#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "expr.h"
#include "db.h"
#include "codes.h"

// forward definitions
static ColumnType map_column_type(const char *type);


static Database *init_db() {
  Database *db = malloc(sizeof(Database));
  db->table_count = 0;
  db->tables = malloc(sizeof(Table) * MAX_TABLES);

  return db;
}

static Table *init_table(const char *name) {
  Table *table = malloc(sizeof(Table));
  strcpy(table->name, name);
  table->column_count = 0;

  return table;
}

static TableColumn *init_table_column(ColumnExpr *column) {
  TableColumn *table_column = malloc(sizeof(TableColumn));
  table_column->type = map_column_type(column->name);
  strcpy(table_column->name, column->name);

  return table_column;
}

static AlliumCode stmt_fail(SqlExpr *expr) {
  printf("unknown statement type: %d", expr->type);
  return ALLIUM_DB_FAIL;
}

static ColumnType map_column_type(const char *type) {
  if (!type) {
    return COLUMN_TYPE_UNKNOWN;
  }

  if (strcasecmp(type, "int") == 0) {
    return COLUMN_TYPE_INT;
  }
  if (strcasecmp(type, "string") == 0) {
    return COLUMN_TYPE_STRING;
  }

  return COLUMN_TYPE_UNKNOWN;
}

static void register_table(Database *db, Table *table) {
  if (db->table_count == MAX_TABLES) {
    return;
  }
  
  db->tables[db->table_count] = *table;
}

static void add_column_to_table(Table *table, ColumnExpr *column) {
  TableColumn *table_column = init_table_column(column);
  table->columns[table->column_count] = *table_column;
}

static AlliumCode execute_create_table(Database *db, SqlExpr *expr) {
  CreateTableStmt *create_table = &expr->as.create_table;

  Table *table = init_table(create_table->name->as.identifier.value);
  
  for (int i = 0; i < create_table->column_count; i++) {
    TableColumn *table_column = init_table_column(&create_table->columns[i]);
    table->columns[table->column_count] = *table_column;
  }

  register_table(db, table);
  
  return ALLIUM_SUCCESS;
}

static void execute_select(Database *db, SqlExpr *expr) {

}

AlliumCode execute_statement(Database *db, SqlExpr *expr) {
  switch (expr->type) {
    case EXPR_CREATE_TABLE_STMT:
      return execute_create_table(db, expr);
      
    default: return stmt_fail(expr);
  }
}

AlliumCode execute(SqlQueryTree *ast) {
  Database *db = init_db();

  for (int i = 0; i < ast->statement_count; i++) {
    AlliumCode code = execute_statement(db, &ast->statements[i]);

    if (code != ALLIUM_SUCCESS) {
      return ALLIUM_DB_FAIL;
    }
  }

  return ALLIUM_SUCCESS;
}