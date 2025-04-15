/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Meiyi
//

#pragma once
#include "src/smart_ptr.h"
#include "src/global_def.h"
#include "../../../common/sql_command.h"
#include "../../../common/filed_type.h"


namespace wa{
    namespace db{
        class Expression;
        /**
         * @brief 描述一个字段
         * @ingroup SQLParser
         * @details 属性，或者说字段(column, field)
         * Rel -> Relation
         * Attr -> Attribute
         */
        struct FieldRefNode{
            UP<String>                              tableName_;
            UP<String>                              fieldName_;
        };

        /**
         * @brief 描述一个select语句
         * @ingroup SQLParser
         * @details 一个正常的select语句描述起来比这个要复杂很多，这里做了简化。
         * 一个select语句由三部分组成，分别是select, from, where。
         * select部分表示要查询的字段，from部分表示要查询的表，where部分表示查询的条件。
         * 比如 from 中可以是多个表，也可以是另一个查询语句，这里仅仅支持表，也就是 relations。
         * where 条件 conditions，这里表示使用AND串联起来多个条件。正常的SQL语句会有OR，NOT等，
         * 甚至可以包含复杂的表达式。
         */
        struct SelectSqlNode{
            Vector<UP<String>>                      tables_;                    ///< 查询的表
            Vector<UP<Expression>>                  expressions_;               ///< 查询的表达式
            Vector<UP<Expression>>                  conditions_;               ///< 查询条件，使用AND串联起来多个条件
            Vector<UP<Expression>>                  groupByConditions_;         ///< group by clause
        };

        /**
         * @brief 描述一个insert语句,目前仅支持单行插入
         * @ingroup SQLParser
         * @details 于Selects类似，也做了很多简化
         */
        struct InsertSqlNode{
            UP<String>                              tableName_;
            Vector<UP<Expression>>                  values_;
        };

        /**
         * @brief 描述一个delete语句
         * @ingroup SQLParser
         */
        struct DeleteSqlNode{
            UP<String>                              tableName_;
            Vector<UP<Expression>>                  conditions_;
        };

        /**
         * @brief 描述一个update语句
         * @ingroup SQLParser
         */
        struct UpdateSqlNode{
            UP<String>                              tableName_;
            Vector<UP<String>>                      filedNames_;
            Vector<UP<Expression>>                  values_;
            Vector<UP<Expression>>                  conditions_;
        };

        /**
         * @brief 描述一个属性,只有在建表时采用
         * @ingroup SQLParser
         * @details 属性，或者说字段(column, field)
         */
        struct FieldDefNode{
            FieldType                               type_;
            UP<String>                              name_;
            Size                                    length_;  ///< Length of attribute
        };

        /**
         * @brief 描述一个create table语句
         * @ingroup SQLParser
         * @details 这里也做了很多简化。
         */
        struct CreateTableSqlNode{
            UP<String>                              tableName_;
            Vector<UP<FieldInfoNode>>               fieldInfos_;
            UP<String>                              storageFormat_;
        };

        /**
         * @brief 描述一个drop table语句
         * @ingroup SQLParser
         */
        struct DropTableSqlNode{
            UP<String>                              tableName_;
            Bool                                    ifExist_;
        };

        /**
         * @brief 描述一个create index语句
         * @ingroup SQLParser
         * @details 创建索引时，需要指定索引名，表名，字段名。
         * 正常的SQL语句中，一个索引可能包含了多个字段，这里仅支持一个字段。
         */
        struct CreateIndexSqlNode{
            UP<String>                              indexName_;
            UP<String>                              tableName_;
            Vector<UP<String>>                      fieldNames_;
        };

        /**
         * @brief 描述一个drop index语句
         * @ingroup SQLParser
         */
        struct DropIndexSqlNode{
            UP<String>                              indexName_;
            UP<String>                              tableName_;
        };

        /**
         * @brief 描述一个desc table语句
         * @ingroup SQLParser
         * @details desc table 是查询表结构信息的语句
         */
        struct DescTableSqlNode{
            UP<String>                              tableName_;
        };

        /**
         * @brief 描述一个load data语句
         * @ingroup SQLParser
         * @details 从文件导入数据到表中。文件中的每一行就是一条数据，每行的数据类型、字段个数都与表保持一致
         */
        struct LoadDataSqlNode{
            UP<String>                              tableName_;
            UP<String>                              fileName_;
        };

        /**
         * @brief 设置变量的值
         * @ingroup SQLParser
         * @note 当前还没有查询变量
         */
        struct SetVariableSqlNode{
            UP<String>                              tableName_;
            UP<Expression>                          value_;
        };

        class ParsedSqlNode;

        /**
         * @brief 描述一个explain语句
         * @ingroup SQLParser
         * @details 会创建operator的语句，才能用explain输出执行计划。
         * 一个command就是一个语句，比如select语句，insert语句等。
         * 可能改成SqlCommand更合适。
         */
        struct ExplainSqlNode{
            UP<ParsedSqlNode>                       sqlNode_;
        };

        /**
         * @brief 解析SQL语句出现了错误
         * @ingroup SQLParser
         * @details 当前解析时并没有处理错误的行号和列号
         */
        struct ErrorSqlNode{
            Vector<String>                          error_msg;
            Int                                     line;
            Int                                     column;
        };
        /**
         * @brief 表示一个SQL语句
         * @ingroup SQLParser
         */
        class ParsedSqlNode{
        public:
            SqlCommandType                          flag;
            ErrorSqlNode                            error;
            SelectSqlNode                           selection;
            InsertSqlNode                           insertion;
            DeleteSqlNode                           deletion;
            UpdateSqlNode                           update;
            CreateTableSqlNode                      create_table;
            DropTableSqlNode                        drop_table;
            CreateIndexSqlNode                      create_index;
            DropIndexSqlNode                        drop_index;
            DescTableSqlNode                        desc_table;
            LoadDataSqlNode                         load_data;
            ExplainSqlNode                          explain;
            SetVariableSqlNode                      set_variable;

        public:
            ParsedSqlNode();
            explicit ParsedSqlNode(SqlCommandType flag);
        };

        /**
         * @brief 表示语法解析后的数据
         * @ingroup SQLParser
         */
        class ParsedSqlResult
        {
        public:
            void add_sql_node(UP<ParsedSqlNode> sql_node);

            Vector<UP<ParsedSqlNode>> &sql_nodes() { return sql_nodes_; }

        private:
            Vector<UP<ParsedSqlNode>> sql_nodes_;  ///< 这里记录SQL命令。虽然看起来支持多个，但是当前仅处理一个
        };

    }
}
