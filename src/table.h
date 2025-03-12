#ifndef TABLE_H
#define TABLE_H

#define MAX_COLUMNS 256     // maximum number of columns per table
#define MAX_TABLE_NAME 255  // maximum table name length
#define MAX_COLUMN_NAME 255 // maximum column name length

typedef enum {
  INT,
  STRING
} ColumnType;

typedef struct {
  char name[MAX_COLUMN_NAME];
  ColumnType type;
} TableColumn;

typedef struct {
  TableColumn columns[MAX_COLUMNS];
  int column_count;
  char name[MAX_TABLE_NAME];
} Table;

#endif