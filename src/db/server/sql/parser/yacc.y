/* 头部分 ====================================================================================================================================== */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./sql_node.h"
#include "./lex.h"
#include "./yacc.hpp"
#include "../../../common/arithmetic_type.h"
#include "../../expression/expression.h"

using namespace std;
using namespace wa;
using namespace wa::db;
string token_name(const char *sql_string, YYLTYPE *llocp){
    return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
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
        IF
        EXISTS
%token  <int_>    INT
%token  <float_>    FLOAT
%token  <string_>   ID
%token  <string_>   STRING

/* union 中定义各种数据类型，真实生成的代码也是union类型，所以不能有非POD类型的数据*/
%union{
    wa::db::ParsedSqlNode *                             parsedSqlNode_;
    wa::db::Expression *                                expression_;
    Vector<wa::UP<wa::db::Expression>> *                    expressions_;
    wa::db::ArithmeticType                         arithmeticType_;
    
    wa::db::FieldNode*                               field_;
    Vector<wa::UP<wa::db::FieldNode>>*                   fields_;
    
    wa::db::FieldDefNode*                               fieldDef_;
    Vector<wa::UP<wa::db::FieldDefNode>>*                   fieldDefs_;

    wa::db::AssignmentNode*                             assignment_;
    Vector<wa::UP<wa::db::AssignmentNode>>*                 assignments_;

    F64                                               float_;
    Int                                                 int_;
    String*                                             string_;
    Vector<wa::UP<String>>*                                  strings_;   
    wa::db::FieldType                                   type_;
}



/*type union中的类型          规则名*/
%type   <type_>                         type
%type   <value_>                        value
%type   <arithmeticType_>               arithmeticType
%type   <expression_>                   expression
%type   <expressions_>                  expressions  
%type   <expressions_>                  nonEmptyExpressions
%type   <field_>                        field
%type   <fields_>                       fields

%type   <string_>                       table
%type   <strings_>                      tables

%type   <string_>                       storageStmt

%type   <fieldDef_>                     fieldDef
%type   <fieldDefs_>                    fieldDefs

%type   <fields_>                       groupByStmt
%type   <expressions_>                  whereStmt
%type   <assignment_>                   assignmentStmt
%type   <assignments_>                  assignmentStmts

%type   <parsedSqlNode_>                calcStmt
%type   <parsedSqlNode_>                selectStmt
%type   <parsedSqlNode_>                insertStmt
%type   <parsedSqlNode_>                updateStmt
%type   <parsedSqlNode_>                deleteStmt
%type   <parsedSqlNode_>                createTableStmt
%type   <parsedSqlNode_>                dropTableStmt
%type   <parsedSqlNode_>                showTablesStmt
 /*%type   <parsedSqlNode_>                descTableStmt*/
%type   <parsedSqlNode_>                createIndexStmt
%type   <parsedSqlNode_>                dropIndexStmt
%type   <parsedSqlNode_>                syncStmt
%type   <parsedSqlNode_>                beginStmt
%type   <parsedSqlNode_>                commitStmt
%type   <parsedSqlNode_>                rollbackStmt
 /*%type   <parsedSqlNode_>                loadDataStmt*/
 /*%type   <parsedSqlNode_>                explainStmt*/
%type   <parsedSqlNode_>                setVariableStmt
%type   <parsedSqlNode_>                helpStmt
%type   <parsedSqlNode_>                exitStmt
%type   <parsedSqlNode_>                commands
%type   <parsedSqlNode_>                commandWrapper

%left ADD SUB MUL DIV
%nonassoc UMINUS
%%
commands: commandWrapper SEMICOLON /*commands or sqls. parser starts here.*/
  {
    unique_ptr<ParsedSqlNode> sql_node = unique_ptr<ParsedSqlNode>($1);
    sql_result->add_sql_node(std::move(sql_node));
  }
  ;

commandWrapper:
    calcStmt
  | selectStmt
  | insertStmt
  | updateStmt
  | deleteStmt
  | createTableStmt
  | dropTableStmt
  | showTablesStmt
  /*| descTableStmt*/
  | createIndexStmt
  | dropIndexStmt
  | syncStmt
  | beginStmt
  | commitStmt
  | rollbackStmt
 /*| loadDataStmt*/
 /* | explainStmt*/
  | setVariableStmt
  | helpStmt
  | exitStmt
    ;
type:
    TYPE_INT      { $$ = FieldType::INT; }
    | TYPE_STRING { $$ = FieldType::CHARS; }
    | TYPE_FLOAT  { $$ = FieldType::FLOAT; }
    | TYPE_VECTOR { $$ = FieldType::VECTOR; }
    ;
    

/* stmt  ================================================================================= */
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
      $$ = new ParsedSqlNode(SqlCommandType::SCF_SYNC);
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
      $$ = new ParsedSqlNode(new DropTableSqlNode($3,FALSE)); 
    }
    | DROP TABLE IF EXISTS ID {
      $$ = new ParsedSqlNode(new DropTableSqlNode($5, TRUE)); 
    }
    ;
showTablesStmt:
    SHOW TABLES {
      $$ = new ParsedSqlNode(SqlCommandType::SHOW_TABLES);
    }
    ;

createIndexStmt:    /*create index 语句的语法解析树*/
    CREATE INDEX ID ON ID LBRACE ID RBRACE
    {
      $$ = new ParsedSqlNode(new CreateIndexSqlNode($3,$5,$7));
    }
    ;

dropIndexStmt:      /*drop index 语句的语法解析树*/
    DROP INDEX ID ON ID
    {
      $$ = new ParsedSqlNode(new DropIndexSqlNode($3,$5));
    }
    ;
createTableStmt:    /*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE fieldDef fieldDefs RBRACE storageStmt
    {
      $$ = new ParsedSqlNode(new CreateTableSqlNode($3,$5,$6,$8));
    }
    ;
insertStmt:  
    INSERT INTO ID VALUES LBRACE expressions RBRACE
    {
        $$ = new ParsedSqlNode(new InsertSqlNode($3,$6));
    };
storageStmt:
    /* empty */
    {
      $$ = NULLPTR;
    }
    | STORAGE FORMAT EQ ID
    {
      $$ = $4;
    }
    ;
deleteStmt:    /*  delete 语句的语法解析树*/
    DELETE FROM ID whereStmt
    {
      $$ = new ParsedSqlNode(new DeleteSqlNode($3,$4));
    }
    ;
updateStmt:      /*  update 语句的语法解析树*/
    UPDATE ID SET assignmentStmts whereStmt
    {
        $$ = new ParsedSqlNode(new UpdateSqlNode($2,$4,$5));
    };
selectStmt:        /*  select 语句的语法解析树*/
    SELECT nonEmptyExpressions FROM tables whereStmt groupByStmt
    {
      $$ = new ParsedSqlNode(new SelectSqlNode($4,$2,$5,$6));
    };
calcStmt:
    CALC expression
    {
      $$ = new ParsedSqlNode(new CalculateSqlNode($2));
    };
      
setVariableStmt:
    SET ID EQ expression
    {
        $$ = new ParsedSqlNode(new SetVariableSqlNode($2,$4));
    };      

/* substmt  ================================================================================= */
groupByStmt:
    /* empty */
    {
        $$ = NULLPTR;
    }|
    GROUP BY fields
    {
        $$=$3;
    };
    
whereStmt:
    /* empty */
    {
        $$ = NULLPTR;
    }|
    WHERE nonEmptyExpressions{
        $$ = $2;   
    };
assignmentStmts:
    assignmentStmt
    {
        $$ = new Vector<wa::UP<AssignmentNode>> ();
        $$->emplace_back($1);
    }|
    fieldDef COMMA fieldDefs
    {
        $$ = $3;
        $$->emplace_back($1);
    }
assignmentStmt:
    ID EQ expression
    {
        $$ = new AssignmentNode($1, $3);
    };
    
/* other  ================================================================================= */
field:
    ID {
      $$ = new FieldNode($1,NULLPTR);
    }
    | ID DOT ID {
      $$ = new FieldNode($1,$3);
    }
    ;

fields:
    /* empty */
    {
      $$ = NULLPTR;
    }
    | COMMA field fields
    {
      if ($3 != NULLPTR) {
        $$ = $3;
      } else {
        $$ = new Vector<UP<FieldNode>>;
      }
      $$->emplace_back($2);
      delete $2;
    }
    ;
fieldDef:
    ID type LBRACE INT RBRACE
    {
      $$ = new FieldDefNode($2,$1,$4);
    }
    | ID type
    {
      $$ = new FieldDefNode($2,$1,0);
    };
fieldDefs:
    /* empty */
    {
      $$ = NULLPTR;
    }
    | fieldDef COMMA fieldDefs
    {
        if ($3 != NULLPTR) {
            $$ = $3;
        } else {
            $$ = new Vector<UP<FieldDefNode>>;
        }
        $$->emplace_back($1);
    };


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

arithmeticType:
    /* 算术运算符 */
    ADD { $$ = ArithmeticType::ADD; }
    | SUB { $$ = ArithmeticType::SUB; }
    | MUL { $$ = ArithmeticType::MUL; }
    | DIV { $$ = ArithmeticType::DIV; }
    /* 比较运算符 */
    | EQ { $$ = ArithmeticType::EQUAL; }
    | LE { $$ = ArithmeticType::LESS_EQUAL; }
    | NE { $$ = ArithmeticType::NOT_EQUAL; }
    | LT { $$ = ArithmeticType::LESS_THAN; }
    | GE { $$ = ArithmeticType::GREAT_EQUAL; }
    | GT { $$ = ArithmeticType::GREAT_THAN; }
    /* IS 运算符 */
    | IS { $$ = ArithmeticType::IS; }
    | IS NOT { $$ = ArithmeticType::IS_NOT; }
    /* LIKE 运算符 */
    | LIKE { $$ = ArithmeticType::LIKE; }
    | NOT LIKE { $$ = ArithmeticType::NOT_LIKE; }
    ;
    
nonEmptyExpressions:
    expression
    {    
        $$ = new vector<UP<Expression>>;
        $$->emplace_back($1);
    }
    | nonEmptyExpressions COMMA expression
    {
        if ($1 != NULLPTR) {
            $$ = $1;
        } else {
            $$ = new Vector<UP<Expression>>;
        }
        $$->emplace_back($3);
    };
expressions:
    /* empty */
    {
        $$ = NULLPTR;
    }
    | expressions COMMA expression
    {
        if ($1 != NULLPTR) {
            $$ = $1;
        } else {
            $$ = new Vector<UP<Expression>>;
        }
        $$->emplace_back($3);
    };
    
expression:
    expression arithmeticType expression{
      $$ = ArithmeticExpression($1,$2,$3);
    }
    | LBRACE expression RBRACE {
      $$ = $2;
    }
    | SUB expression %prec UMINUS {
      $$ = new ArithmeticExpression(NULLPTR,ArighmeticType::NEGATIVE,$2);
    }
    | ID {
      $$ = new UnboundedExpression($1);
    }
    | MUL {
      $$ = new StarExpression();
    }
    ;

table:
    ID {
      $$ = $1;
    };
tables:
    table {
      $$ = new Vector<UP<String>>();
      $$->emplace_back($1);
    }
    | tables COMMA table {
      if ($1 != NULLPTR) {
        $$ = $1;
      } else {
        $$ = new Vector<UP<String>>;
      }
      
      $$->emplace_back($3);
    };



%%
extern void ScanString(const char *str, yyscan_t scanner);

int YyParseSql(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  std::vector<char *> allocated_strings;
  yylex_init_extra(static_cast<void*>(&allocated_strings),&scanner);
  ScanString(s, scanner);
  int result = yyparse(s, sql_result, scanner);

  for (char *ptr : allocated_strings) {
    free(ptr);
  }
  allocated_strings.clear();

  yylex_destroy(scanner);
  return result;
}
