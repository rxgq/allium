#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "expr.h"

static ParserState *parser;

static SqlExpr *parse_stmt();


void print_depth(int depth) {
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
}

char *op_to_string(Op op) {
  switch (op) {
    case OP_AND: return "AND";
    case OP_OR: return "OR";
    case OP_NOT: return "NOT";
  }

  return "UNKNOWN OP";
}

void print_expr(SqlExpr *expr, int depth) {
  print_depth(depth);

  switch (expr->type) {
    case EXPR_SELECT_STMT:
      printf("SELECT:\n");
      for (int i = 0; i < 2; i++) {
        print_expr(&expr->as.select.clauses[i], depth + 1);
      }
      return;

    case EXPR_SELECT_CLAUSE:
      printf("SELECT CLAUSE:\n");
      for (int i = 0; i < expr->as.select_clause.options_count; i++) {
        print_expr(&expr->as.select_clause.options[i], depth + 1);
      }

      print_depth(depth + 1);
      printf("DISTINCT: %s\n", expr->as.select_clause.is_distinct ? "true" : "false");

      if (expr->as.select_clause.top_count) {
        print_depth(depth + 1);
        printf("TOP%s:\n", expr->as.select_clause.is_top_percent ? " PERCENT" : "");
        print_expr(expr->as.select_clause.top_count, depth + 2);
      }
      return;

    case EXPR_INSERT_STMT:
      printf("INSERT INTO:\n");
      print_expr(expr->as.insert.table_name, depth + 1);

      print_depth(depth + 1);
      printf("COLUMNS:\n");
      for (int i = 0; i < expr->as.insert.column_count; i++) {
        print_depth(depth);
        print_expr(expr->as.insert.column_names[i], depth + 1);
      }

      print_depth(depth + 1);
      printf("VALUES (%d):\n", expr->as.insert.row_count);
      for (int i = 0; i < expr->as.insert.row_count; i++) {
        print_depth(depth + 2);
        printf("VALUES (%d):\n", i);

        for (int j = 0; j < expr->as.insert.rows[i]->value_count; j++) {
          print_depth(depth + 1);
          print_expr(expr->as.insert.rows[i]->values[j], depth + 1);
        }
      }

      return;

    case EXPR_IDENTIFIER:
      printf("IDENTIFIER: %s\n", expr->as.identifier.value);
      return;

    case EXPR_NUMERIC:
      printf("NUMERIC: %d\n", expr->as.numeric.value);
      return;

    case EXPR_FROM_CLAUSE:
      printf("FROM CLAUSE:\n");
      print_expr(expr->as.from_clause.expr, depth + 1);
      return;

    case EXPR_ALIAS:
      printf("ALIAS:\n");
      print_expr(expr->as.alias.expr, depth + 1);
      print_expr(expr->as.alias.identifier, depth + 1);
      return;

    case EXPR_ALL_COLUMNS:
      printf("ALL COLUMNS: *\n");
      return;

    case EXPR_CREATE_TABLE_STMT:
      printf("CREATE TABLE:\n");
      for (int i = 0; i < expr->as.create_table.column_count; i++) {
        print_depth(depth + 1);
        printf("%s %s\n", expr->as.create_table.columns[i].type, expr->as.create_table.columns[i].name);
      }
      return;

    case EXPR_DROP_TABLE_STMT:
      printf("DROP TABLE:\n");
      print_expr(expr->as.drop_table.name, depth + 1);
      return;

    case EXPR_WHERE_CLAUSE:
      printf("WHERE:\n");
      print_expr(expr->as.where_clause.condition, depth + 1);
      return;

    case EXPR_UNARY:
      printf("UNARY %s:\n", op_to_string(expr->as.unary_expr.op));
      print_expr(expr->as.unary_expr.expr, depth + 1);
      return;

    case EXPR_BINARY:
      printf("BINARY %s:\n", op_to_string(expr->as.binary_expr.op));
      print_expr(expr->as.binary_expr.left, depth + 1);
      print_expr(expr->as.binary_expr.right, depth + 1);
      return;

    case BAD_EXPR:
      printf("BAD EXPR\n");
      return;
      
    default: 
      printf("unknown type when printing ast: '%d'\n", expr->type);
  }
}

void parser_out(ParserState *parser) {
  printf("\n\nSQL Parse Tree:\n");
  for (int i = 0; i < parser->ast->statement_count; i++) {
    print_expr(&parser->ast->statements[i], 1);
  }
  printf("\n\n");
}

ParserState *init_parser(Token *tokens, int token_count) {
  ParserState *parser = malloc(sizeof(ParserState));
  parser->ast = init_sql_tree();
  parser->tokens = tokens;
  parser->token_count = token_count;
  parser->error_token = NULL;
  parser->current = 0;

  return parser;
}

void free_parser(ParserState *parser) {
  // to be continued

  free(parser);
}

static inline Token *current() {
  return &parser->tokens[parser->current];
}

static inline void advance() {
  parser->current++;
}

static inline int match(TokenType type) {
  return current()->type == type ? 1 : 0;
}

static int expect(TokenType type) {
  if (match(type)) {
    advance();
    return 1;
  }

  return 0;
}

static inline void parser_error() {
  parser->error_token = current();
}

static inline SqlExpr *bad_expr() {
  parser_error();
  return init_expr(BAD_EXPR);
}

static inline int is_bad(SqlExpr *expr) {
  return expr->type == BAD_EXPR;
}

short get_num_len(int n) {
  if (n < 0) return get_num_len((n == INT_MIN) ? INT_MAX : -n);
  if (n < 10) return 1;
  return 1 + get_num_len(n / 10);
}

static void output_error() {
  int errTokLine = parser->error_token->line;
  // int errTokCol = 0;

  printf("line %d: ", errTokLine);
  // int prefixLen = get_num_len(errTokLine) + 5;

  // for (int i = 0; i < parser->token_count; i++) {
  //   if (parser->tokens[i].line != errTokLine) continue;
  //   if (parser->tokens[i].type == TOKEN_EOF) continue;

  //   if (&parser->tokens[i] == parser->error_token) {
  //     errTokCol = prefixLen;
  //   }

  //   printf("%s ", parser->tokens[i].lexeme);
  //   prefixLen += strlen(parser->tokens[i].lexeme) + 1;
  // }
  // printf("\n");

  // for (int i = 0; i < prefixLen; i++) {
  //   printf(" ");
  // }
  // printf("^\n");

  TokenType err_token_type = parser->error_token->type;

  // prevents the 'EOF' token from being in the 'near x' output.
  // only applies if the error_token type is TOKEN_EOF
  if (err_token_type == TOKEN_EOF) {
    parser->error_token = &parser->tokens[parser->token_count - 2];
  }

  printf("Syntax error on line %d, near '%s'\n", errTokLine, parser->error_token->lexeme);
}

static void add_expr(SqlExpr *expr) {
  if (parser->ast->statement_capacity >= parser->ast->statement_count) {
    parser->ast->statement_capacity *= 2;
    parser->ast->statements = realloc(parser->ast->statements, sizeof(SqlExpr) * parser->ast->statement_capacity);
  }

  parser->ast->statements[parser->ast->statement_count] = *expr;
  parser->ast->statement_count++;
}

static SqlExpr *parse_primary() {
  Token *token = current();

  if (token->type == TOKEN_IDENTIFIER) {
    advance();

    SqlExpr *expr = init_expr(EXPR_IDENTIFIER);
    expr->as.identifier.value = strdup(token->lexeme);

    return expr;
  }
  else if (token->type == TOKEN_STAR) {
    advance();

    SqlExpr *expr = init_expr(EXPR_ALL_COLUMNS);
    expr->as.all_columns.value = token->lexeme;

    return expr;
  }
  else if (token->type == TOKEN_NUMERIC) {
    advance();

    SqlExpr *expr = init_expr(EXPR_NUMERIC);
    expr->as.numeric.value = atoi(token->lexeme);

    return expr;
  }
  else if (token->type == TOKEN_INT) {
    advance();

    SqlExpr *expr = init_expr(EXPR_IDENTIFIER);
    expr->as.identifier.value = strdup(token->lexeme);

    return expr;
  }
  else if (token->type == TOKEN_STRING) {
    advance();

    SqlExpr *expr = init_expr(EXPR_IDENTIFIER);
    expr->as.identifier.value = strdup(token->lexeme);

    return expr;
  }

  return bad_expr();
}

static SqlExpr *parse_alias() {
  SqlExpr *expr = parse_primary();
  if (is_bad(expr)) return expr;

  if (match(TOKEN_AS)) {
    advance();

    SqlExpr *alias = init_expr(EXPR_ALIAS);
    alias->as.alias.expr = expr;
    alias->as.alias.identifier = parse_primary();
    if (is_bad(alias->as.alias.identifier)) return alias->as.alias.identifier;

    return alias;
  }

  return expr;
}

static SqlExpr *parse_expr() {
  return parse_alias();
}

static SqlExpr *parse_from_clause() {
  if (!expect(TOKEN_FROM)) {
    return bad_expr();
  }

  SqlExpr *from = init_expr(EXPR_FROM_CLAUSE);

  SqlExpr *expr;
  if (match(TOKEN_LEFT_PAREN)) {
    advance();

    expr = parse_stmt();
    if (!expect(TOKEN_RIGHT_PAREN)) {
      return bad_expr();
    }
  } else {
    expr = parse_expr();
  }

  if (is_bad(expr)) return expr;
  from->as.from_clause.expr = expr;

  return from;
}

static SqlExpr *parse_select_clause() {
  if (!expect(TOKEN_SELECT)) {
    return bad_expr();
  }

  SqlExpr *expr = init_expr(EXPR_SELECT_CLAUSE);
  expr->as.select_clause.options_count = 0;
  expr->as.select_clause.options = malloc(sizeof(SqlExpr));
  expr->as.select_clause.is_distinct = 0;
  expr->as.select_clause.top_count = NULL;
  expr->as.select_clause.is_top_percent = 0;

  if (match(TOKEN_DISTINCT)) {
    advance();
    expr->as.select_clause.is_distinct = 1;
  }

  if (match(TOKEN_TOP)) {
    advance();

    SqlExpr *top_count = parse_expr();
    if (is_bad(top_count)) return top_count;

    expr->as.select_clause.top_count = top_count;

    if (match(TOKEN_PERCENT)) {
      advance();
      expr->as.select_clause.is_top_percent = 1;
    }
  }

  parser->current--;

  int capacity = 1;
  do {
    advance();

    SqlExpr *identifier = parse_expr();
    if (is_bad(identifier)) return identifier;

    if (expr->as.select_clause.options_count >= capacity) {
      capacity *= 2;
      expr->as.select_clause.options = realloc(expr->as.select_clause.options, sizeof(SqlExpr) * capacity);
    }

    expr->as.select_clause.options[expr->as.select_clause.options_count] = *identifier;
    expr->as.select_clause.options_count++;

  } while (match(TOKEN_COMMA));

  return expr;
}

static SqlExpr *parse_not_expr() {
  if (match(TOKEN_NOT)) {
    advance();
    SqlExpr *unary_expr = parse_not_expr();
    if (is_bad(unary_expr)) return unary_expr;

    SqlExpr *expr = init_expr(EXPR_UNARY);
    expr->as.unary_expr.expr = unary_expr;
    expr->as.unary_expr.op = OP_NOT;

    return expr;
  }

  return parse_primary();
}

static SqlExpr *parse_and_expr() {
  SqlExpr *left = parse_not_expr();
  if (is_bad(left)) return left;

  if (match(TOKEN_AND)) {
    advance();

    SqlExpr *right = parse_and_expr();
    if (is_bad(right)) return right;

    SqlExpr *expr = init_expr(EXPR_BINARY);
    expr->as.binary_expr.left = left;
    expr->as.binary_expr.op = OP_AND;
    expr->as.binary_expr.right = right;

    return expr;
  }

  return left;
}

static SqlExpr *parse_or_expr() {
  SqlExpr *left = parse_and_expr();
  if (is_bad(left)) return left;

  if (match(TOKEN_OR)) {
    advance();

    SqlExpr *right = parse_or_expr();
    if (is_bad(right)) return right;
    
    SqlExpr *expr = init_expr(EXPR_BINARY);
    expr->as.binary_expr.left = left;
    expr->as.binary_expr.op = OP_OR;
    expr->as.binary_expr.right = right;

    return expr;
  }

  return left;
}

static SqlExpr *parse_where_clause() { // where not x
  if (!expect(TOKEN_WHERE)) {
    return bad_expr();
  }

  SqlExpr *expr = parse_or_expr();

  SqlExpr *where = init_expr(EXPR_WHERE_CLAUSE);
  where->as.where_clause.condition = expr;

  return where;
}

static SqlExpr *parse_select_stmt() {
  SqlExpr *select_clause = parse_select_clause();
  if (is_bad(select_clause)) return select_clause;

  SqlExpr *from_clause = parse_from_clause();
  if (is_bad(from_clause)) return from_clause;

  SqlExpr *select = init_expr(EXPR_SELECT_STMT);

  select->as.select.clauses = malloc(sizeof(SqlExpr) * 2);
  select->as.select.clauses[0] = *select_clause;
  select->as.select.clauses[1] = *from_clause;

  select->as.select.clause_count = 2;

  return select;
}

static SqlExpr *parse_create_table_stmt() {
  if (!expect(TOKEN_CREATE)) {
    return bad_expr();
  }

  if (!expect(TOKEN_TABLE)) {
    return bad_expr();
  }

  SqlExpr *expr = init_expr(EXPR_CREATE_TABLE_STMT);

  SqlExpr *table_name = parse_expr();
  if (is_bad(table_name)) return table_name;

  expr->as.create_table.name = table_name;
  expr->as.create_table.column_count = 0;
  expr->as.create_table.columns = malloc(sizeof(ColumnExpr));

  if (!expect(TOKEN_LEFT_PAREN)) {
    return bad_expr();
  }

  int capacity = 1;
  parser->current--;
  do {
    advance();

    SqlExpr *column_name = parse_expr();
    if (is_bad(column_name)) return column_name;

    SqlExpr *type = parse_expr();
    if (is_bad(type)) return type;

    if (expr->as.create_table.column_count >= capacity) {
      capacity *= 2;
      expr->as.create_table.columns = realloc(expr->as.create_table.columns, capacity * sizeof(ColumnExpr));
    }

    ColumnExpr *column = malloc(sizeof(ColumnExpr));
    column->name = column_name->as.identifier.value;
    column->type = type->as.identifier.value;

    expr->as.create_table.columns[expr->as.create_table.column_count] = *column;
    expr->as.create_table.column_count++;

  } while (match(TOKEN_COMMA));

  if (!expect(TOKEN_RIGHT_PAREN)) {
    return bad_expr();
  }

  return expr;
}

static SqlExpr *parse_drop_table_stmt() {
  if (!expect(TOKEN_DROP)) {
    return bad_expr();
  }

  if (!expect(TOKEN_TABLE)) {
    return bad_expr();
  }

  SqlExpr *expr = init_expr(EXPR_DROP_TABLE_STMT);

  SqlExpr *table_name = parse_expr();
  if (is_bad(table_name)) return table_name;

  expr->as.drop_table.name = table_name;
  return expr;
}

static SqlExpr *parse_insert_into_stmt() {
  if (!expect(TOKEN_INSERT)) {
    return bad_expr();
  }

  if (!expect(TOKEN_INTO)) {
    return bad_expr();
  }

  SqlExpr *table_name = parse_primary();
  if (is_bad(table_name)) return table_name;

  if (!expect(TOKEN_LEFT_PAREN)) {
    return bad_expr();
  }

  SqlExpr *expr = init_expr(EXPR_INSERT_STMT);
  expr->as.insert.table_name = table_name;
  expr->as.insert.column_names = malloc(sizeof(IdentifierExpr));
  expr->as.insert.column_count = 0;

  int capacity = 1;
  parser->current--;
  do {
    advance();

    if (expr->as.insert.column_count >= capacity) {
      capacity *= 2;
      expr->as.insert.column_names = realloc(expr->as.insert.column_names, capacity * sizeof(IdentifierExpr));
    }

    SqlExpr *column_name = parse_primary();
    if (is_bad(column_name)) return column_name;

    expr->as.insert.column_names[expr->as.insert.column_count++] = column_name;

  } while (match(TOKEN_COMMA));


  if (!expect(TOKEN_RIGHT_PAREN)) {
    return bad_expr();
  }

  if (!expect(TOKEN_VALUES)) {
    return bad_expr();
  }

  int row_capacity = 1;
  expr->as.insert.row_count = 0;
  expr->as.insert.rows = malloc(sizeof(SqlExpr));

  parser->current--;
  do {
    advance();

    if (!expect(TOKEN_LEFT_PAREN)) { // TODO! realloc for rows
      return bad_expr();
    }

    expr->as.insert.rows[expr->as.insert.row_count] = malloc(sizeof(SqlExpr));

    expr->as.insert.rows[expr->as.insert.row_count]->values = malloc(sizeof(SqlExpr));
    expr->as.insert.rows[expr->as.insert.row_count]->value_count = 0;

    int value_capacity = 1;
    parser->current--;
    do {
      advance();

      if (expr->as.insert.rows[expr->as.insert.row_count]->value_count >= value_capacity) {
        value_capacity *= 2;
        expr->as.insert.rows[expr->as.insert.row_count]->values = realloc(expr->as.insert.rows[expr->as.insert.row_count]->values, value_capacity * sizeof(SqlExpr));
      }

      SqlExpr *value = parse_primary();
      if (is_bad(value)) return value;

      expr->as.insert.rows[expr->as.insert.row_count]->values[expr->as.insert.rows[expr->as.insert.row_count]->value_count++] = value;
    } while (match(TOKEN_COMMA));

    if (!expect(TOKEN_RIGHT_PAREN)) {
      return bad_expr();
    }

    expr->as.insert.row_count++;
    
  } while (match(TOKEN_COMMA));

  return expr;
}

static SqlExpr *parse_stmt() {
  TokenType type = current()->type;

  switch (type) {
    case TOKEN_SELECT: 
      return parse_select_stmt();
    case TOKEN_CREATE:
      return parse_create_table_stmt();
    case TOKEN_DROP:
      return parse_drop_table_stmt();
    case TOKEN_WHERE:
      return parse_where_clause();
    case TOKEN_INSERT:
      return parse_insert_into_stmt();
    default:
      return bad_expr();
  }
}

ParserState *parse_ast(int debug, Token *tokens, int token_count) {
  parser = init_parser(tokens, token_count);

  while (!match(TOKEN_EOF)) {
  SqlExpr *expr = parse_stmt();
  add_expr(expr);

    if (is_bad(expr)) {
      output_error();
      break;
    }
  }

  if (debug) parser_out(parser);

  return parser;
} 