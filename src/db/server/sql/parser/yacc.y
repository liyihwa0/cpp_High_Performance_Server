// 头部分 ======================================================================================================================================
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  unique_ptr<ParsedSqlNode> error_sql_node = make_unique<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

UnboundAggregateExpr *create_aggregate_expression(const char *aggregate_name,
                                           Expression *child,
                                           const char *sql_string,
                                           YYLTYPE *llocp)
{
  UnboundAggregateExpr *expr = new UnboundAggregateExpr(aggregate_name, child);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

%}
// 规则部分 ======================================================================================================================================

%define api.pure full
%define parse.error verbose
/** 启用位置标识 **/
%locations
%lex-param { yyscan_t scanner }
/** 这些定义了在yyparse函数中的参数 **/
%parse-param { const char * sql_string }
%parse-param { ParsedSqlResult * sql_result }
%parse-param { void * scanner }

//标识tokens
%token  SEMICOLON
        BY
        CREATE
        DROP
        GROUP
        TABLE
        TABLES
        INDEX
        CALC
        SELECT
        DESC
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        BEGIN
        START
        TRANSACTION
        COMMIT
        ROLLBACK
        TYPE_INT
        TYPE_STRING
        TYPE_FLOAT
        TYPE_VECTOR
        HELP
        EXIT
        DOT
        INTO
        VALUES
        FROM
        WHERE
        AND
        SET
        ON
        LOAD
        DATA
        INFILE
        EXPLAIN
        STORAGE
        FORMAT
        EQ
        LT
        GT
        LE
        GE
        NE
        ADD
        SUB
        MUL
        DIV
        NULL
%token  <int_>    INT
%token  <float_>    FLOAT
%token  <string_>   ID
%token  <string_>   STRING

/* union 中定义各种数据类型，真实生成的代码也是union类型，所以不能有非POD类型的数据*/
%union{
    ParsedSqlNode *                             parsedSqlNode_;
    ConditionSqlNode *                          condition_;
    Expression *                                expression_;
    Vector<UP<Expression>> *                    expressionList_;
    enum CompareOperator                        compareOperator_;

    FieldSqlNode*                               field_;
    Vector<UP<FieldSqlNode>>*                   fields_;

    Table*                                      table_;
    Vector<UP<Table>>*                          tables_;

    Float                                       float_;
    Int                                         int_;
    String*                                     string_;
}



//type union中的类型          变量名

%type   <compareOperator_>              compareOperator
%type   <expression_>                   expression
%type   <expressionList_>               expressionList      // INSERT INTO table fields VALUES expressionList;

%type   <field_>                        field
%type   <fields_>                       fieldList

%type   <table_>                        table
%type   <tables_>                       tableList

%type   <expression_>                   condition
%type   <expressionList_>               conditionList      // SELECT fields FROM tables WHERE conditions ORDER BY fields;



%type   <parsedSqlNode_>                calc_stmt
%type   <parsedSqlNode_>                select_stmt
%type   <parsedSqlNode_>                insert_stmt
%type   <parsedSqlNode_>                update_stmt
%type   <parsedSqlNode_>                delete_stmt
%type   <parsedSqlNode_>                create_table_stmt
%type   <parsedSqlNode_>                drop_table_stmt
%type   <parsedSqlNode_>                show_tables_stmt
%type   <parsedSqlNode_>                desc_table_stmt
%type   <parsedSqlNode_>                create_index_stmt
%type   <parsedSqlNode_>                drop_index_stmt
%type   <parsedSqlNode_>                sync_stmt
%type   <parsedSqlNode_>                begin_stmt
%type   <parsedSqlNode_>                commit_stmt
%type   <parsedSqlNode_>                rollback_stmt
%type   <parsedSqlNode_>                load_data_stmt
%type   <parsedSqlNode_>                explain_stmt
%type   <parsedSqlNode_>                set_variable_stmt
%type   <parsedSqlNode_>                help_stmt
%type   <parsedSqlNode_>                exit_stmt
%type   <parsedSqlNode_>                command_wrapper

%left ADD SUB MUL DIV
%nonassoc UMINUS
%%

commands: command_wrapper opt_semicolon  //commands or sqls. parser starts here.
  {
    unique_ptr<ParsedSqlNode> sql_node = unique_ptr<ParsedSqlNode>($1);
    sql_result->add_sql_node(std::move(sql_node));
  }
  ;

command_wrapper:
    calc_stmt
  | select_stmt
  | insert_stmt
  | update_stmt
  | delete_stmt
  | create_table_stmt
  | drop_table_stmt
  | show_tables_stmt
  | desc_table_stmt
  | create_index_stmt
  | drop_index_stmt
  | sync_stmt
  | begin_stmt
  | commit_stmt
  | rollback_stmt
  | load_data_stmt
  | explain_stmt
  | set_variable_stmt
  | help_stmt
  | exit_stmt
    ;

exit_stmt:
    EXIT {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      $$ = new ParsedSqlNode(SqlCommandType::EXIT);
    };

help_stmt:
    HELP {
      $$ = new ParsedSqlNode(SqlCommandType::HELP);
    };

sync_stmt:
    SYNC {
      $$ = new ParsedSqlNode(SCF_SYNC);
    }
    ;

begin_stmt:
    BEGIN  {
      $$ = new ParsedSqlNode(SqlCommandType::BEGIN);
    }
    ;

commit_stmt:
    COMMIT {
      $$ = new ParsedSqlNode(SqlCommandType::COMMIT);
    }
    ;

rollback_stmt:
    ROLLBACK  {
      $$ = new ParsedSqlNode(SqlCommandType::ROLLBACK);
    }
    ;

drop_table_stmt:    /*drop table 语句的语法解析树*/
    DROP TABLE ID {
      $$ = new ParsedSqlNode(SqlCommandType::DROP_TABLE);
      $$->drop_table.relation_name = $3;
    };

show_tables_stmt:
    SHOW TABLES {
      $$ = new ParsedSqlNode(SqlCommandType::SHOW_TABLES);
    }
    ;

desc_table_stmt:
    DESC ID  {
      $$ = new ParsedSqlNode(SqlCommandType::DESC_TABLE);
      $$->desc_table.relation_name = $2;
    }
    ;

create_index_stmt:    /*create index 语句的语法解析树*/
    CREATE INDEX ID ON ID LBRACE ID RBRACE
    {
      $$ = new ParsedSqlNode(SqlCommandType::CREATE_INDEX);
      CreateIndexSqlNode &create_index = $$->create_index;
      create_index.index_name = $3;
      create_index.relation_name = $5;
      create_index.attribute_name = $7;
    }
    ;

drop_index_stmt:      /*drop index 语句的语法解析树*/
    DROP INDEX ID ON ID
    {
      $$ = new ParsedSqlNode(SqlCommandType::DROP_INDEX);
      $$->drop_index.index_name = $3;
      $$->drop_index.relation_name = $5;
    }
    ;
create_table_stmt:    /*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format
    {
      $$ = new ParsedSqlNode(SqlCommandType::CREATE_TABLE);
      CreateTableSqlNode &create_table = $$->create_table;
      create_table.relation_name = $3;
      //free($3);

      vector<FieldInfo> *src_attrs = $6;

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
        delete src_attrs;
      }
      create_table.attr_infos.emplace_back(*$5);
      reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete $5;
      if ($8 != nullptr) {
        create_table.storage_format = $8;
      }
    }
    ;
attr_def_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA attr_def attr_def_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new vector<FieldInfo>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;

attr_def:
    ID type LBRACE number RBRACE
    {
      $$ = new FieldInfo;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = $4;
    }
    | ID type
    {
      $$ = new FieldInfo;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = 4;
    }
    ;
number:
    NUMBER {$$ = $1;}
    ;
type:
    INT_T      { $$ = static_cast<int>(AttrType::INTS); }
    | STRING_T { $$ = static_cast<int>(AttrType::CHARS); }
    | FLOAT_T  { $$ = static_cast<int>(AttrType::FLOATS); }
    | VECTOR_T { $$ = static_cast<int>(AttrType::VECTORS); }
    ;
insert_stmt:        /*insert   语句的语法解析树*/
    INSERT INTO ID VALUES LBRACE value value_list RBRACE
    {
      $$ = new ParsedSqlNode(SqlCommandType::INSERT);
      $$->insertion.relation_name = $3;
      if ($7 != nullptr) {
        $$->insertion.values.swap(*$7);
        delete $7;
      }
      $$->insertion.values.emplace_back(*$6);
      reverse($$->insertion.values.begin(), $$->insertion.values.end());
      delete $6;
    }
    ;

value_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA value value_list  {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new vector<Value>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;
value:
    INT {
      $$ = new Value((int)$1);
      @$ = @1;
    }
    |FLOAT {
      $$ = new Value((float)$1);
      @$ = @1;
    }
    |STRING {
      char *tmp = common::substr($1,1,strlen($1)-2);
      $$ = new Value(tmp);
      free(tmp);
    }
    ;
storage_format:
    /* empty */
    {
      $$ = nullptr;
    }
    | STORAGE FORMAT EQ ID
    {
      $$ = $4;
    }
    ;

delete_stmt:    /*  delete 语句的语法解析树*/
    DELETE FROM ID where
    {
      $$ = new ParsedSqlNode(SqlCommandType::DELETE);
      $$->deletion.relation_name = $3;
      if ($4 != nullptr) {
        $$->deletion.conditions.swap(*$4);
        delete $4;
      }
    }
    ;
update_stmt:      /*  update 语句的语法解析树*/
    UPDATE ID SET ID EQ value where
    {
      $$ = new ParsedSqlNode(SqlCommandType::UPDATE);
      $$->update.relation_name = $2;
      $$->update.attribute_name = $4;
      $$->update.value = *$6;
      if ($7 != nullptr) {
        $$->update.conditions.swap(*$7);
        delete $7;
      }
    }
    ;
select_stmt:        /*  select 语句的语法解析树*/
    SELECT expression_list FROM rel_list where group_by
    {
      $$ = new ParsedSqlNode(SqlCommandType::SELECT);
      if ($2 != nullptr) {
        $$->selection.expressions.swap(*$2);
        delete $2;
      }

      if ($4 != nullptr) {
        $$->selection.relations.swap(*$4);
        delete $4;
      }

      if ($5 != nullptr) {
        $$->selection.conditions.swap(*$5);
        delete $5;
      }

      if ($6 != nullptr) {
        $$->selection.group_by.swap(*$6);
        delete $6;
      }
    }
    ;
calc_stmt:
    CALC expression_list
    {
      $$ = new ParsedSqlNode(SqlCommandType::CALC);
      $$->calc.expressions.swap(*$2);
      delete $2;
    }
    ;

expression_list:
    expression
    {
      $$ = new vector<unique_ptr<Expression>>;
      $$->emplace_back($1);
    }
    | expression COMMA expression_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new vector<unique_ptr<Expression>>;
      }
      $$->emplace($$->begin(), $1);
    }
    ;

expression:
    expression ADD expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::ADD, $1, $3, sql_string, &@$);
    }
    | expression SUB expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::SUB, $1, $3, sql_string, &@$);
    }
    | expression MUL expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::MUL, $1, $3, sql_string, &@$);
    }
    | expression DIV expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::DIV, $1, $3, sql_string, &@$);
    }
    | LBRACE expression RBRACE {
      $$ = $2;
      $$->set_name(token_name(sql_string, &@$));
    }
    | SUB expression %prec UMINUS {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, $2, NULLPTR, sql_string, &@$);
    }
    | ID {
      FieldRefNode *node = $1;
      $$ = new UnboundFieldExpression(node);
      $$->set_name(token_name(sql_string, &@$));
    }
    | MUL {
      $$ = new StarExpr();
    }
    ;

fieldRef:
    ID {
      $$ = new FieldRefNode();
      $$->attribute_name = $1;
    }
    | ID DOT ID {
      $$ = new FieldRefNode();
      $$->relation_name  = $1;
      $$->attribute_name = $3;
    }
    ;

table:
    ID {
      $$ = $1;
    }
    ;
tables:
    relation {
      $$ = new vector<string>();
      $$->push_back($1);
    }
    | tables COMMA table {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new vector<string>;
      }

      $$->insert($$->begin(), $1);
    }
    ;

set_variable_stmt:
    SET ID EQ value
    {
      $$ = new ParsedSqlNode(SqlCommandType::SET_VARIABLE);
      $$->set_variable.name  = $2;
      $$->set_variable.value = *$4;
      delete $4;
    }
    ;

opt_semicolon: /*empty*/
    | SEMICOLON
    ;
%%
//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  std::vector<char *> allocated_strings;
  yylex_init_extra(static_cast<void*>(&allocated_strings),&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);

  for (char *ptr : allocated_strings) {
    free(ptr);
  }
  allocated_strings.clear();

  yylex_destroy(scanner);
  return result;
}