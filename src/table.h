#ifndef TABLE_H
#define TABLE_H

#include "expr.h"

#define MAX_COLUMNS 256     // maximum number of columns per table
#define MAX_TABLE_NAME 255  // maximum table name length
#define MAX_COLUMN_NAME 255 // maximum column name length

typedef enum {
  COLUMN_TYPE_INT,
  COLUMN_TYPE_STRING,
  COLUMN_TYPE_UNKNOWN
} ColumnType;

typedef struct {
  void *values[MAX_COLUMNS];
} TableRow;

typedef struct {
  char name[MAX_COLUMN_NAME];
  ColumnType type;
} TableColumn;

typedef struct {
  TableColumn columns[MAX_COLUMNS];
  int column_count;
  char name[MAX_TABLE_NAME];

  TableRow *rows;
  int row_count;
} Table;

extern TableColumn *init_table_column(ColumnExpr *column);
extern TableRow *init_table_row(InsertValues *values);
extern Table *init_table(const char *name);
extern ColumnType map_column_type(const char *type);

#endif