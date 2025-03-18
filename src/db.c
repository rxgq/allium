#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "expr.h"
#include "db.h"
#include "codes.h"
#include "table.h"


AlliumDb *init_allium(int debug) {
  AlliumDb *allium = malloc(sizeof(AlliumDb));
  if (!allium) {
    return NULL;
  }

  allium->debug = debug;
  allium->err = NULL;

  allium->db = malloc(sizeof(Database));
  if (!allium->db) {
    free(allium);
    return NULL;
  }

  allium->db->table_count = 0;
  allium->db->tables = malloc(sizeof(Table) * MAX_TABLES);
  if (!allium->db->tables) {
    free(allium->db);
    free(allium);
    return NULL;
  }

  return allium;
}

static AlliumCode stmt_fail(AlliumDb *allium, SqlExpr *expr) {
  if (!allium->debug) return ALLIUM_DB_FAIL;

  printf("ALLIUM_DB: unknown statement type: '%s'\n", expr_type_to_str(expr->type));
  return ALLIUM_DB_FAIL;
}

static int is_err(AlliumCode code) {
  return code != ALLIUM_SUCCESS;
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
  
  if (db->err) {
    free(db->err);
  }
  db->err = strdup(buffer);
}

static AlliumCode query_message_success(char *message) {
  printf("query: %s", message);
  return ALLIUM_SUCCESS;
}

static void free_table(Table *table) {
  if (!table) return;
  free(table);
}

static void free_database(Database *db) {
  if (!db) return;

  for (int i = 0; i < db->table_count; i++) {
    free_table(&db->tables[i]);
  }
  free(db);
}

static AlliumCode register_table(AlliumDb *allium, Table *table) {
  if (strlen(table->name) > 255) {
    set_err(allium, "table name too long, must be < 256 characters ");
    free_table(table);
    return ALLIUM_TABLE_NAME_TOO_LONG_ERR;
  }

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
    ColumnType type = map_column_type(create_table->columns[i].type);
    if (type == COLUMN_TYPE_UNKNOWN) {
      set_err(allium, "invalid data type '%s'", create_table->columns[i].type);
      free_table(table);
      return ALLIUM_INVALID_COLUMN_DATA_TYPE_ERR;
    }

    TableColumn *table_column = init_table_column(&create_table->columns[i]);
    table->columns[table->column_count++] = *table_column;
  }

  AlliumCode code = register_table(allium, table);
  if (is_err(code)) return code;
  
  return query_message_success("CREATE TABLE");
}

static AlliumCode execute_drop_table(AlliumDb *allium, SqlExpr *expr) {
  DropTableStmt drop_table = expr->as.drop_table;

  Table *table = get_table(allium->db, drop_table.name->as.identifier.value);
  if (!table) {
    set_err(allium, "table not found '%s'", drop_table.name->as.identifier.value);
    return ALLIUM_TABLE_NOT_FOUND_ERR;
  }

  int idx = -1;
  for (int i = 0; i < allium->db->table_count; i++) {
    if (&allium->db->tables[i] == table) {
      idx = i;
      break;
    }
  }
  
  for (int i = idx; i < allium->db->table_count - 1; i++) {
    allium->db->tables[i] = allium->db->tables[i + 1];
  }

  allium->db->table_count--;

  return query_message_success("DROP TABLE");
}

static inline SqlExpr *as_from(SqlExpr *expr) {
  return expr->as.from_clause.expr;
}

static AlliumCode execute_select(AlliumDb *allium, SqlExpr *expr) {
  SelectStmt *select = &expr->as.select;

  for (int i = 0; i < select->clause_count; i++) {
    SqlExpr *clause = &select->clauses[i];
    
    if (clause->type == EXPR_SELECT_CLAUSE) {

    }

    if (clause->type == EXPR_FROM_CLAUSE) {
      if (as_from(clause)->type == EXPR_SELECT_STMT) {
        return execute_select(allium, clause->as.from_clause.expr);
      }
      else if (as_from(clause)->type == EXPR_IDENTIFIER) {
        char *table_name = clause->as.from_clause.expr->as.identifier.value;
        Table *table = get_table(allium->db, table_name);
        if (!table) {
          set_err(allium, "table '%s' not found", table_name);
          return ALLIUM_TABLE_NOT_FOUND_ERR;
        }
        
        // collect the default width of the columns
        int *col_widths = malloc(table->column_count * sizeof(int));
        for (int j = 0; j < table->column_count; j++) {
          col_widths[j] = strlen(table->columns[j].name);
        }
        
        // stretch column width to the largest item in the row
        for (int i = 0; i < table->row_count; i++) {
          for (int j = 0; j < table->column_count; j++) {
            int item_length = strlen((char *)table->rows[i].values[j]);
            if (item_length > col_widths[j]) {
              col_widths[j] = item_length;
            }
          }
        }
        
        // output column headers
        printf("\n\n");
        printf("| ");
        for (int j = 0; j < table->column_count; j++) {
          printf("%-*s | ", col_widths[j], table->columns[j].name);
        }
        printf("\n");
        
        for (int j = 0; j < table->column_count; j++) {
          printf("+");
          for (int k = 0; k < col_widths[j] + 2; k++) {
            printf("-");
          }
        }
        printf("+\n");
        
        // output rows
        for (int i = 0; i < table->row_count; i++) {
          printf("| ");
          for (int j = 0; j < table->column_count; j++) {
            TableColumn *column = &table->columns[j];
        
            if (column->type == COLUMN_TYPE_INT) {
              printf("%-*d | ", col_widths[j], *(int *)table->rows[i].values[j]);
            } 
            else if (column->type == COLUMN_TYPE_STRING) {
              printf("%-*s | ", col_widths[j], (char *)table->rows[i].values[j]);
            }
          }
        
          printf("\n");
        }
        printf("\n");
      }
    }
  }

  return query_message_success("SELECT");
}



static AlliumCode execute_insert_into(AlliumDb *allium, SqlExpr *expr) {
  InsertStmt *insert = &expr->as.insert;

  Table *table = get_table(allium->db, insert->table_name->as.identifier.value);
  if (!table) {
    set_err(allium, "table '%s' not found", insert->table_name->as.identifier.value);
    return ALLIUM_TABLE_NOT_FOUND_ERR;
  }

  // do a count check here on column_count vs table column_count
  
  char *bad_column_name = NULL;
  for (int i = 0; i < insert->column_count; i++) {

    for (int j = 0; j < table->column_count; j++) {
      if (strcmp(insert->column_names[i]->as.identifier.value, table->columns[j].name) != 0) {
        bad_column_name = strdup(insert->column_names[i]->as.identifier.value);
        break;
      }
    }

    if (bad_column_name != NULL) {
      set_err(allium, "column '%s' not found", bad_column_name);
      return ALLIUM_COLUMN_MISMATCH_ERR;
    }
  }

  for (int i = 0; i < insert->row_count; i++) {
    TableRow *row = init_table_row(insert->rows[i]);
    
    // for (int i = 0; i < values->value_count; i++) {
    //   row->values[i] = values->values[i];
    // }

    table->rows[table->row_count++] = *row;
  }

  return query_message_success("INSERT INTO");
}

AlliumCode execute_statement(AlliumDb *allium, SqlExpr *expr) {
  switch (expr->type) {
    case EXPR_CREATE_TABLE_STMT:
      return execute_create_table(allium, expr);

    case EXPR_SELECT_STMT:
      return execute_select(allium, expr);

    case EXPR_DROP_TABLE_STMT:
      return execute_drop_table(allium, expr);

    case EXPR_INSERT_STMT:
      return execute_insert_into(allium, expr);
      
    default:
      return stmt_fail(allium, expr);
  }
}

void free_allium(AlliumDb *allium) {
  free_database(allium->db);
  free(allium->err);
  free(allium);
}

AlliumCode execute_query(AlliumDb *allium, SqlQueryTree *ast) {
  clock_t before = clock();

  for (int i = 0; i < ast->statement_count; i++) {
    AlliumCode code = execute_statement(allium, &ast->statements[i]);

    if (is_err(code)) {
      printf(allium->err);
      return ALLIUM_DB_FAIL;
    }
  }

  clock_t difference = clock() - before;
  double time_taken = (double)difference / CLOCKS_PER_SEC;

  printf("\nquery completed in %.3fs\n", time_taken);

  return ALLIUM_SUCCESS;
}