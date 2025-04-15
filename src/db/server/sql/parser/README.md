Parser是服务器第一阶段处理的入口
## lex
我们使用`Flex`做词法分析,`lex.l`为词法分析器所依赖的文件

Flex 的语法规则主要分为三个部分：定义部分、规则部分和用户代码部分。下面是对这些部分的详细讲解。

### 1. 定义部分

定义部分位于文件的开头，使用 `%{` 和 `%}` 包围。这里可以包含 C 代码、宏定义、头文件等。常见的内容包括：

示例：
```c
%{
#include <stdio.h>
#define RETURN_TOKEN(token) return token
%}
```

### 2. 选项部分

选项部分使用 `%option` 指令来设置 Flex 的行为。常见的选项包括：

- **`noyywrap`**：不需要链接 `-lfl`，表示不需要处理输入结束。
- **`reentrant`**：使生成的词法分析器是可重入的。
- **`case-insensitive`**：使匹配不区分大小写。
- **`bison-bridge`**：与 Bison 兼容，允许在 Bison 中使用 Flex 生成的词法分析器。

示例：
```c
%option noyywrap
%option reentrant
%option case-insensitive
```

### 3. 规则部分

规则部分是 Flex 的核心，使用 `%%` 分隔。它定义了如何匹配输入文本中的模式。每条规则由一个正则表达式和一个动作组成。格式如下：

```
正则表达式    动作
```

- **正则表达式**：用于匹配输入文本的模式，可以是字符、字符类、重复、选择等。
- **动作**：当正则表达式匹配成功时执行的 C 代码。

示例：
```c
%%
[0-9]+          { printf("Number: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*  { printf("Identifier: %s\n", yytext); }
\n              { /* 忽略换行 */ }
.               { printf("Unknown character: %c\n", yytext[0]); }
%%
```

### 4. 用户代码部分

用户代码部分位于规则部分之后，使用 `%{` 和 `%}` 包围。这里可以包含额外的 C 代码，例如函数定义、全局变量等。通常用于定义辅助函数或全局变量。

示例：
```c
%{
void my_function() {
    // 自定义函数
}
%}
```

### 5. 特殊符号

- **`%%`**：分隔定义部分和规则部分。
- **`%{ ... %}`**：用户代码部分，可以包含 C 代码。
- **`{}`**：动作部分，包含在规则中，当正则表达式匹配时执行的代码。
- **`yytext`**：一个全局变量，包含当前匹配的文本。
- **`yyleng`**：一个全局变量，表示当前匹配文本的长度。

### 6. 例子

以下是一个简单的 Flex 文件示例，演示了如何使用上述语法规则：

```c
%{
#include <stdio.h>
%}

%option noyywrap

%%
[0-9]+          { printf("Number: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*  { printf("Identifier: %s\n", yytext); }
\n              { /* 忽略换行 */ }
.               { printf("Unknown character: %c\n", yytext[0]); }
%% 

int main(int argc, char **argv) {
    yylex(); // 调用词法分析器
    return 0;
}
```

### 总结

Flex 的语法规则通过定义模式和相应的动作，使得用户能够方便地创建词法分析器。通过正则表达式匹配输入文本中的模式，并在匹配成功时执行相应的 C 代码，Flex 可以有效地处理各种文本输入。

## yacc

Yacc（Yet Another Compiler Compiler）是一种用于生成解析器的工具，通常与 Lex（词法分析器生成器）一起使用。Yacc 使用一种特定的语法来定义文法规则和相应的动作。以下是 Yacc 语法的基本组成部分及其解释。

### Yacc 文件结构

Yacc 文件通常由三个部分组成：

1. **定义部分**：包含头文件、宏定义和全局变量的声明。
2. **规则部分**：定义文法规则和相应的动作。
3. **辅助代码部分**：包含 C 代码，通常用于实现辅助函数和主程序。

### 1. 定义部分

在定义部分，您可以包含头文件、定义常量、声明全局变量等。例如：

```yacc
%{
#include <stdio.h>
#include <stdlib.h>
%}

%token NUMBER
%token ADD SUB MUL DIV
%token LPAREN RPAREN
```

- `%{ ... %}`：C 代码块，包含在生成的解析器中。
- `%token`：声明词法分析器生成的令牌（token）。

### 2. 规则部分

规则部分定义了文法规则和相应的动作。每个规则的格式如下：

```
<非终结符> : <产生式> { <动作> }
```

例如，以下是一个简单的算术表达式文法：

```yacc
%%
expr: expr ADD term
    | expr SUB term
    | term
    ;

term: term MUL factor
    | term DIV factor
    | factor
    ;

factor: NUMBER
      | LPAREN expr RPAREN
      ;
%%
```

- `expr`、`term` 和 `factor` 是非终结符。
- `ADD`、`SUB`、`MUL`、`DIV`、`NUMBER`、`LPAREN` 和 `RPAREN` 是终结符。
- 每个规则的右侧是产生式，左侧是对应的非终结符。
- `{ <动作> }` 是在匹配到该规则时执行的 C 代码块。

### 3. 辅助代码部分

在辅助代码部分，您可以实现辅助函数和主程序。例如：

```yacc
int main(void) {
    return yyparse();
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
```

- `yyparse()` 是 Yacc 生成的解析函数。
- `yyerror()` 是错误处理函数，当解析出错时调用。

### 4. 语法优先级和结合性

Yacc 允许您定义操作符的优先级和结合性，以解决解析歧义。可以使用 `%left`、`%right` 和 `%nonassoc` 来定义：

```yacc
%left ADD SUB
%left MUL DIV
```

- `%left`：定义左结合的操作符。
- `%right`：定义右结合的操作符。
- `%nonassoc`：定义不结合的操作符。

### 完整示例

以下是一个完整的 Yacc 示例，结合了上述所有部分：

```yacc
%{
#include <stdio.h>
#include <stdlib.h>
%}

%token NUMBER
%token ADD SUB MUL DIV
%token LPAREN RPAREN

%left ADD SUB
%left MUL DIV

%%
expr: expr ADD term
    | expr SUB term
    | term
    ;

term: term MUL factor
    | term DIV factor
    | factor
    ;

factor: NUMBER
      | LPAREN expr RPAREN
      ;
%% 

int main(void) {
    return yyparse();
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
```

