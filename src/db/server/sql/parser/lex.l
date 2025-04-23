%top{
/*
这里的代码会被复制到lex_sql.cpp的最开始位置
定义yy_size_t的原因是因为flex生成的代码，会使用yy_size_t与其他类型的数字
做比较，导致编译报警
*/
#define YY_TYPEDEF_YY_SIZE_T
typedef int yy_size_t;

/* 参考生成的lex_sql.cpp代码，这个宏定义会放在每次运行yylex()最开始的地方 */
#define YY_USER_INIT                                         \
  yycolumn = 0;

/* 参考生成的lex_sql.cpp代码，这个宏定义会放在解析一个token之后，也可以在网上找到大量的参考资料 */
/* 我们在这里设置当前解析的token的位置信息，这样在yacc中就可以使用这些信息了 */
#define YY_USER_ACTION                                       \
do {                                                         \
  yylloc->first_line   = yylloc->last_line = yylineno;       \
  yylloc->first_column = yycolumn;                           \
  yylloc->last_column  = yylloc->first_column + yyleng - 1;  \
  yycolumn += yyleng;                                        \
}                                                            \
while (0);

}

/* 用户自定义代码段 ==================================================================================================================================== */
%{
#include<string.h>
#include<stdio.h>
#include <vector>
#include "sql/parser/parse_defs.h"
#include "common/log/log.h"
#include "yacc_sql.hpp"

#ifndef register
#define register
#endif // register

extern int atoi();
extern double atof();

#define RETURN_TOKEN(token) LOG_DEBUG("%s", #token);return token
%}

/* 选项部分 ==================================================================================================================================== */

%option noyywrap
%option nounput
%option noinput
%option bison-bridge
%option reentrant
%option case-insensitive                
%option bison-locations

/* 以下内容在定义正则
正则名称    正则值
*/
WHITE_SPACE      [\ \t\b\f]
DIGIT            [0-9]+

ID               [A-Za-z_]+[A-Za-z0-9_]*
DOT              \.
STRING           [\"([^"\\]*(\\.[^"\\]*)*)\" | '\''([^'\\]*(\\.[^'\\]*)*)'\'']

%x STR

%%

 /* 规则部分 ==================================================================================================================================== */

 /* 规则匹配的优先级：
    1. 匹配的规则长的优先
    2. 写在最前面的优先

    3. 规则的写法:
        规则的定义         匹配到该规则时,执行什么代码

    4. 规则的定义:
        {正则名称}              代表匹配正则表达式
        "符号"                代表匹配单个字符
        不包含任何特殊括号,代表正则表达式   例如 . 代表通配符, EXIT代表字符串EXIT本身
    其实 {} 可以视为取出前边定义的正则表达式, ""和不包含特殊符号的都可以视为正则本身的值,所以规则的定义本身就是正则表达式
 */

{WHITE_SPACE}                                        // ignore whitespace
\n                                                   ;

[\-]?{DIGIT}+                           yylval->int_=atoi(yytext); RETURN_TOKEN(INT);
[\-]?{DIGIT}+{DOT}{DIGIT}+              yylval->float_=(float)(atof(yytext)); RETURN_TOKEN(FLOAT);

";"                                     RETURN_TOKEN(SEMICOLON);
\.                                      RETURN_TOKEN(DOT);
EXIT                                    RETURN_TOKEN(EXIT);
HELP                                    RETURN_TOKEN(HELP);
DESC                                    RETURN_TOKEN(DESC);
CREATE                                  RETURN_TOKEN(CREATE);
DROP                                    RETURN_TOKEN(DROP);
TABLE                                   RETURN_TOKEN(TABLE);
TABLES                                  RETURN_TOKEN(TABLES);
INDEX                                   RETURN_TOKEN(INDEX);
ON                                      RETURN_TOKEN(ON);
SHOW                                    RETURN_TOKEN(SHOW);
SYNC                                    RETURN_TOKEN(SYNC);
SELECT                                  RETURN_TOKEN(SELECT);
CALC                                    RETURN_TOKEN(CALC);
FROM                                    RETURN_TOKEN(FROM);
WHERE                                   RETURN_TOKEN(WHERE);
AND                                     RETURN_TOKEN(AND);
INSERT                                  RETURN_TOKEN(INSERT);
INTO                                    RETURN_TOKEN(INTO);
VALUES                                  RETURN_TOKEN(VALUES);
DELETE                                  RETURN_TOKEN(DELETE);
UPDATE                                  RETURN_TOKEN(UPDATE);
SET                                     RETURN_TOKEN(SET);
BEGIN                                   RETURN_TOKEN(BEGIN);
START                                   RETURN_TOKEN(START);
TRANSACTION                             RETURN_TOKEN(TRANSACTION);
COMMIT                                  RETURN_TOKEN(COMMIT);
ROLLBACK                                RETURN_TOKEN(ROLLBACK);
INT                                     RETURN_TOKEN(TYPE_INT);
CHAR                                    RETURN_TOKEN(TYPE_STRING);
FLOAT                                   RETURN_TOKEN(TYPE_FLOAT);
VECTOR                                  RETURN_TOKEN(TYPE_VECTOR);
LOAD                                    RETURN_TOKEN(LOAD);
DATA                                    RETURN_TOKEN(DATA);
INFILE                                  RETURN_TOKEN(INFILE);
EXPLAIN                                 RETURN_TOKEN(EXPLAIN);
GROUP                                   RETURN_TOKEN(GROUP);
BY                                      RETURN_TOKEN(BY);
STORAGE                                 RETURN_TOKEN(STORAGE);
FORMAT                                  RETURN_TOKEN(FORMAT);
NULL                                    RETURN_TOKEN(NULL);
IS                                      RETURN_TOKEN(IS);
NOT                                     RETURN_TOKEN(NOT);
LIKE                                    RETURN_TOKEN(LIKE);
{ID}                                    yylval->string_=strdup(yytext); static_cast<std::vector<char*>*>(yyextra)->push_back(yylval->string_); RETURN_TOKEN(ID);
"("                                     RETURN_TOKEN(LBRACE);
")"                                     RETURN_TOKEN(RBRACE);

","                                     RETURN_TOKEN(COMMA);
"="                                     RETURN_TOKEN(EQ);
"<="                                    RETURN_TOKEN(LE);
"<>"                                    RETURN_TOKEN(NE);
"!="                                    RETURN_TOKEN(NE);
"<"                                     RETURN_TOKEN(LT);
">="                                    RETURN_TOKEN(GE);
">"                                     RETURN_TOKEN(GT);

"+"                                     RETURN_TOKEN(ADD);
"-"                                     RETURN_TOKEN(SUB);
"*"                                     RETURN_TOKEN(MUL);
"/"                                     RETURN_TOKEN(DIV);

{STRING}                                yylval->string_ = strdup(yytext); static_cast<std::vector<char*>*>(yyextra)->push_back(yylval->string_); RETURN_TOKEN(STRING);

.                                       LOG_DEBUG("Unknown character [%c]",yytext[0]); return yytext[0];
%%

void scan_string(const char *str, yyscan_t scanner) {
  yy_switch_to_buffer(yy_scan_string(str, scanner), scanner);
}