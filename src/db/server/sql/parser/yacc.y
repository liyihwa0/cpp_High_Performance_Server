/* 头部分 ====================================================================================================================================== */
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
/* 规则部分 ====================================================================================================================================== */

%define api.pure full
%define parse.error verbose
/** 启用位置标识 **/
%locations
%lex-param { yyscan_t scanner }
/** 这些定义了在yyparse函数中的参数 **/
%parse-param { const char * sql_string }
%parse-param { ParsedSqlResult * sql_result }
%parse-param { void * scanner }

/*标识tokens*/
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
        IS
        NOT
        LIKE
%token  <int_>    INT
%token  <float_>    FLOAT
%token  <string_>   ID
%token  <string_>   STRING

/* union 中定义各种数据类型，真实生成的代码也是union类型，所以不能有非POD类型的数据*/
%union{
    ParsedSqlNode *                             parsedSqlNode_;
    Expression *                                expression_;
    Vector<UP<Expression>> *                    expressionList_;
    enum CompareOperator                        compareOperator_;

    FieldSqlNode*                               field_;
    Vector<UP<FieldSqlNode>>*                   fields_;

    FieldDefSqlNode*                            fieldDef_;
    Vector<UP<FieldDefSqlNode>>*                fieldDefList_;

    Float                                       float_;
    Int                                         int_;
    String*                                     string_;
    Vector<UP<String>>                          strings_;   
}



/*type union中的类型          规则名*/

%type   <compareOperator_>              compareOperator
%type   <expression_>                   expression
%type   <expressionList_>               expressionList  

%type   <field_>                        field
%type   <fields_>                       fieldList

%type   <string_>                       table
%type   <strings_>                      tableList

%type   <expression_>                   condition
%type   <expressionList_>               conditionList  

%type   <fieldDef_>                     fieldDef
%type   <fieldDefList_>                 fieldDefList

%type   <fieldList_>                    groupByStmt
%type   <expressionList_>               whereStmt


%type   <parsedSqlNode_>                calcStmt
%type   <parsedSqlNode_>                selectStmt
%type   <parsedSqlNode_>                insertStmt
%type   <parsedSqlNode_>                updateStmt
%type   <parsedSqlNode_>                deleteStmt
%type   <parsedSqlNode_>                createTableStmt
%type   <parsedSqlNode_>                dropTableStmt
%type   <parsedSqlNode_>                showTablesStmt
%type   <parsedSqlNode_>                descTableStmt
%type   <parsedSqlNode_>                createIndexStmt
%type   <parsedSqlNode_>                dropIndexStmt
%type   <parsedSqlNode_>                syncStmt
%type   <parsedSqlNode_>                beginStmt
%type   <parsedSqlNode_>                commitStmt
%type   <parsedSqlNode_>                rollbackStmt
%type   <parsedSqlNode_>                loadDataStmt
%type   <parsedSqlNode_>                explainStmt
%type   <parsedSqlNode_>                setVariableStmt
%type   <parsedSqlNode_>                helpStmt
%type   <parsedSqlNode_>                exitStmt
%type   <parsedSqlNode_>                commandWrapper

%left ADD SUB MUL DIV
%nonassoc UMINUS
%%

commands: command_wrapper opt_semicolon /*commands or sqls. parser starts here.*/
  {
    unique_ptr<ParsedSqlNode> sql_node = unique_ptr<ParsedSqlNode>($1);
    sql_result->add_sql_node(std::move(sql_node));
  }
  ;

command_wrapper:
    calcStmt
  | selectStmt
  | insertStmt
  | updateStmt
  | deleteStmt
  | createTableStmt
  | dropTableStmt
  | showTablesStmt
  | descTableStmt
  | createIndexStmt
  | dropIndexStmt
  | syncStmt
  | beginStmt
  | commitStmt
  | rollbackStmt
  | loadDataStmt
  | explainStmt
  | setVariableStmt
  | helpStmt
  | exitStmt
    ;

exitStmt:
    EXIT {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      $$ = new ParsedSqlNode(SqlCommandType::EXIT);
    };

helpStmt:
    HELP {
      $$ = new ParsedSqlNode(SqlCommandType::HELP);
    };

syncStmt:
    SYNC {
      $$ = new ParsedSqlNode(SCF_SYNC);
    }
    ;

beginStmt:
    BEGIN  {
      $$ = new ParsedSqlNode(SqlCommandType::BEGIN);
    }
    ;

commitStmt:
    COMMIT {
      $$ = new ParsedSqlNode(SqlCommandType::COMMIT);
    }
    ;

rollbackStmt:
    ROLLBACK  {
      $$ = new ParsedSqlNode(SqlCommandType::ROLLBACK);
    }
    ;

dropTableStmt:    /*drop table 语句的语法解析树*/
    DROP TABLE ID {
      $$ = new ParsedSqlNode(SqlCommandType::DROP_TABLE);
      $$->drop_table.relation_name = $3;
    };

showTablesStmt:
    SHOW TABLES {
      $$ = new ParsedSqlNode(SqlCommandType::SHOW_TABLES);
    }
    ;

createIndexStmt:    /*create index 语句的语法解析树*/
    CREATE INDEX ID ON ID LBRACE ID RBRACE
    {
      $$ = new ParsedSqlNode(SqlCommandType::CREATE_INDEX);
      CreateIndexSqlNode &create_index = $$->create_index;
      create_index.index_name = $3;
      create_index.relation_name = $5;
      create_index.attribute_name = $7;
    }
    ;

dropIndexStmt:      /*drop index 语句的语法解析树*/
    DROP INDEX ID ON ID
    {
      $$ = new ParsedSqlNode(SqlCommandType::DROP_INDEX);
      $$->drop_index.index_name = $3;
      $$->drop_index.relation_name = $5;
    }
    ;
createTableStmt:    /*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE fieldDef fieldDefList RBRACE storageFormat
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
fieldDefList:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA fieldDef fieldDefList
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
field:
    ID type LBRACE INT RBRACE
    {
      $$ = new FieldInfo;
      $$->type = (FieldType)$2;
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
type:
    TYPE_INT      { $$ = static_cast<int>(FieldType::INT); }
    | TYPE_STRING { $$ = static_cast<int>(FieldType::CHARS); }
    | TYPE_FLOAT  { $$ = static_cast<int>(FieldType::FLOAT); }
    | TYPE_VECTOR { $$ = static_cast<int>(FieldType::VECTOR); }
    ;
insertStmt:        /*insert   语句的语法解析树*/
    INSERT INTO ID VALUES LBRACE expression expressionList RBRACE
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

deleteStmt:    /*  delete 语句的语法解析树*/
    DELETE FROM ID whereStmt
    {
      $$ = new ParsedSqlNode(SqlCommandType::DELETE);
      $$->deletion.relation_name = $3;
      if ($4 != nullptr) {
        $$->deletion.conditionList.swap(*$4);
        delete $4;
      }
    }
    ;
updateStmt:      /*  update 语句的语法解析树*/
    UPDATE ID SET ID EQ value whereStmt
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
selectStmt:        /*  select 语句的语法解析树*/
    SELECT expressionList FROM fieldList whereStmt groupByStmt
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
calcStmt:
    CALC expressionList
    {
      $$ = new ParsedSqlNode(SqlCommandType::CALC);
      $$->calc.expressions.swap(*$2);
      delete $2;
    }
    ;


compareOperator:
    EQ { $$ = CompareOperator::EQUAL; }
    | LE { $$ = CompareOperator::LESS_EQUAL; }
    | NE { $$ = CompareOperator::NOT_EQUAL; }
    | LT { $$ = CompareOperator::LESS_THAN; }
    | GE { $$ = CompareOperator::GREAT_EQUAL; }
    | GT { $$ = CompareOperator::GREAT_THAN; }
    | IS { $$ = CompareOperator::IS; }
    | IS NOT { $$ = CompareOperator::IS_NOT; }
    | LIKE { $$ = CompareOperator::LIKE; }
    | NOT LIKE { $$ = CompareOperator::NOT_LIKE; }
    ;
expressionList:
    expression
    {
      $$ = new vector<unique_ptr<Expression>>;
      $$->emplace_back($1);
    }
    | expression COMMA expressionList
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
    | expression compareOperator expression{
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
whereStmt:
    conditionList{
     $$ = $1;   
    }
conditionList:
    expressionList{
        $$ = $1;
    }
    
fieldDef:
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
tableList:
    table {
      $$ = new vector<string>();
      $$->push_back($1);
    }
    | tableList COMMA table {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new vector<string>;
      }

      $$->insert($$->begin(), $1);
    }
    ;

setVariableStmt:
    SET ID EQ expression
    {
      $$ = new ParsedSqlNode(SqlCommandType::SET_VARIABLE);
      $$->set_variable.name  = $2;
      $$->set_variable.value = *$4;
      delete $4;
    }
    ;


%%
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