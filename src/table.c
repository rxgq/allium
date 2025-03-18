#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "table.h"
#include "expr.h"
#include "tree.h"

Table *init_table(const char *name) {
  Table *table = malloc(sizeof(Table));
  strncpy(table->name, name, sizeof(table->name) - 1);
  table->name[sizeof(table->name) - 1] = '\0';
  table->column_count = 0;
  table->row_count = 0;
  
  table->rows = malloc(sizeof(TableRow));

  return table;
}

TableRow *init_table_row(InsertValues *values) {
  TableRow *row = malloc(sizeof(TableRow));

  for (int i = 0; i < values->value_count; i++) {
    row->values[i] = values->values[i];
  }

  return row;
}

ColumnType map_column_type(const char *type) {
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

TableColumn *init_table_column(ColumnExpr *column) {
  TableColumn *table_column = malloc(sizeof(TableColumn));
  table_column->type = map_column_type(column->type);
  strcpy(table_column->name, column->name);

  return table_column;
}