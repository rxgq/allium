#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "expr.h"
#include "db.h"
#include "codes.h"

// forward definitions
static ColumnType map_column_type(const char *type);


AlliumDb *init_allium(int debug) {
  AlliumDb *allium = malloc(sizeof(AlliumDb));
  allium->debug = debug;
  allium->err = NULL;

  allium->db = malloc(sizeof(Database));
  allium->db->table_count = 0;
  allium->db->tables = malloc(sizeof(Table) * MAX_TABLES);

  return allium;
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

static int is_err(AlliumCode code) {
  return code != ALLIUM_SUCCESS;
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

static Table *get_table(Database *db, const char *table_name) {
  for (int i = 0; i < db->table_count; i++) {
    if (strcmp(db->tables[i].name, table_name) == 0) {
        return &db->tables[i];
    }
  }

  return NULL;
}

static void set_err(AlliumDb *db, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  
  char buffer[1024];
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  
  va_end(args);
  
  free(db->err);
  db->err = strdup(buffer);
}

static void free_table(Table *table) {
  for (int i = 0; i < table->column_count; i++) {
    free(&table->columns[i]);
  }

  // free(&table->name); // warning: 'free' called on pointer 'table' with nonzero offset
  free(table);
}

static AlliumCode register_table(AlliumDb *allium, Table *table) {
  if (allium->db->table_count == MAX_TABLES) {
    set_err(allium, "max tables of reached (%d)", MAX_TABLES);
    free_table(table);
    return ALLIUM_MAX_TABLES_REACHED_ERR;
  }

  if (get_table(allium->db, table->name)) {
    set_err(allium, "table '%s' already exists", table->name);
    free_table(table);
    return ALLIUM_TABLE_ALREADY_EXISTS_ERR;
  }
  
  allium->db->tables[allium->db->table_count++] = *table;
  return ALLIUM_SUCCESS;
}

static AlliumCode execute_create_table(AlliumDb *allium, SqlExpr *expr) {
  CreateTableStmt *create_table = &expr->as.create_table;

  char *table_name = create_table->name->as.identifier.value;
  Table *table = init_table(table_name);
  
  for (int i = 0; i < create_table->column_count; i++) {
    TableColumn *table_column = init_table_column(&create_table->columns[i]);
    table->columns[table->column_count] = *table_column;
  }

  AlliumCode code = register_table(allium, table);
  if (is_err(code)) return code;
  
  printf("create table"); // standardize
  return ALLIUM_SUCCESS;
}

static AlliumCode execute_select(AlliumDb *allium, SqlExpr *expr) {
  SelectStmt *select = &expr->as.select;
  
  char *table_name = select->clauses[1].as.from_clause.expr->as.identifier.value;
  Table *table = get_table(allium->db, table_name);
  if (!table) {
    printf("table '%s' not found", table_name);
  }

  printf("found table");

  return ALLIUM_SUCCESS;
}

AlliumCode execute_statement(AlliumDb *allium, SqlExpr *expr) {
  switch (expr->type) {
    case EXPR_CREATE_TABLE_STMT:
      return execute_create_table(allium, expr);

    case EXPR_SELECT_STMT:
      return execute_select(allium, expr);
      
    default: return stmt_fail(expr);
  }
}

AlliumCode execute(AlliumDb *allium, SqlQueryTree *ast) {
  for (int i = 0; i < ast->statement_count; i++) {
    AlliumCode code = execute_statement(allium, &ast->statements[i]);

    if (is_err(code)) {
      printf(allium->err);
      return ALLIUM_DB_FAIL;
    }
  }

  return ALLIUM_SUCCESS;
}