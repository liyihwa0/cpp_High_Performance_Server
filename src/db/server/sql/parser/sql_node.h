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
         */
        struct FieldNode{
            UP<String>                              tableName_;
            UP<String>                              fieldName_;
            FieldNode(String* tableName,String* fieldName) : tableName_(tableName), fieldName_(fieldName){}
        };

        /**
       * @brief 描述一个字段的定义
       * @ingroup SQLParser
       * @details 属性，或者说字段(column, field)
       */
        struct FieldDefNode{            
            FieldType                               fieldType_;
            UP<String>                              fieldName_;
            Int                                     optionalArg_;
            FieldDefNode(FieldType fieldType, String* fieldName, Int optionalArg)
                    : fieldType_(fieldType), fieldName_(fieldName), optionalArg_(optionalArg) {}
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
            UP<Vector<UP<String>>>                      tables_;                    ///< 查询的表
            UP<Vector<UP<Expression>>>                  expressions_;               ///< 查询的表达式
            UP<Vector<UP<Expression>>>                  conditions_;               ///< 查询条件，使用AND串联起来多个条件
            UP<Vector<UP<Expression>>>                  groupByConditions_;         ///< group by clause

            SelectSqlNode(Vector<UP<String>>* tables, Vector<UP<Expression>>* expressions, Vector<UP<Expression>>* conditions, Vector<UP<Expression>>* groupByConditions)
                    : tables_(tables), expressions_(expressions), conditions_(conditions), groupByConditions_(groupByConditions) {}
        };

        /**
         * @brief 描述一个insert语句,目前仅支持单行插入
         * @ingroup SQLParser
         * @details 于Selects类似，也做了很多简化
         */
        struct InsertSqlNode{
            UP<String>                              tableName_;
            UP<Vector<UP<Expression>>>                  values_;
            InsertSqlNode(String* tableName,Vector<UP<Expression>>* values):tableName_(tableName), values_(values){}
        };

        /**
         * @brief 描述一个delete语句
         * @ingroup SQLParser
         */
        struct DeleteSqlNode{
            UP<String>                              tableName_;
            UP<Vector<UP<Expression>>>              conditions_;
            DeleteSqlNode(String* tableName, Vector<UP<Expression>>* conditions)
                    : tableName_(tableName), conditions_(conditions) {}
                    
        };
        
        struct AssignmentNode {
            UP<String>                                  fieldName_;
            UP<Expression>                                  expression_;

            AssignmentNode(String* fieldName,Expression* expression): fieldName_(fieldName),expression_(expression){}
        };

        /**
         * @brief 描述一个update语句
         * @ingroup SQLParser
         * @details 这里也做了很多简化。
         */
        struct UpdateSqlNode {
            UP<String>                                  tableName_;
            UP<Vector<UP<AssignmentNode>>>              assignments_;
            UP<Vector<UP<Expression>>>                  conditions_;

            UpdateSqlNode(String* tableName,
                          Vector<UP<AssignmentNode>>* assignments,
                          Vector<UP<Expression>>* conditions)
                    : tableName_(tableName),
                      assignments_(assignments),
                      conditions_(conditions) {}
        };

        
        /**
         * @brief 描述一个create table语句
         * @ingroup SQLParser
         * @details 这里也做了很多简化。
         */
        struct CreateTableSqlNode{
            UP<String>                              tableName_;
            UP<Vector<UP<FieldDefNode>>>             fieldDefs_;
            UP<String>                              storageFormat_;
            CreateTableSqlNode(String* tableName, FieldDefNode* fieldDef, Vector<UP<FieldDefNode>>* fieldDefs, String* storageFormat)
                    : tableName_(tableName), fieldDefs_(fieldDefs), storageFormat_(storageFormat) {
                fieldDefs_.emplace_back(fieldDef);
                reverse(fieldDefs_.begin(),fieldDefs_.end());
            }
        };

        /**
         * @brief 描述一个drop table语句
         * @ingroup SQLParser
         */
        struct DropTableSqlNode{
            UP<String>                              tableName_;
            Bool                                    ifExist_;
            DropTableSqlNode(String* tableName, Bool ifExist)
                    : tableName_(tableName), ifExist_(ifExist) {}
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
            CreateIndexSqlNode(String* indexName, String* tableName, String * fieldName)
                    : indexName_(indexName), tableName_(tableName) {
                fieldNames_.emplace_back(fieldName);
            }
            CreateIndexSqlNode(String* indexName, String* tableName, Vector<UP<String>> fieldNames)
                    : indexName_(indexName), tableName_(tableName), fieldNames_(std::move(fieldNames)) {}
        };

        /**
         * @brief 描述一个drop index语句
         * @ingroup SQLParser
         */
        struct DropIndexSqlNode{
            UP<String>                              indexName_;
            UP<String>                              tableName_;
            DropIndexSqlNode(String* indexName, String* tableName)
                    : indexName_(indexName), tableName_(tableName) {}
        };

        /**
         * @brief 描述一个desc table语句
         * @ingroup SQLParser
         * @details desc table 是查询表结构信息的语句
         */
        struct DescTableSqlNode{
            UP<String>                              tableName_;
            DescTableSqlNode(String* tableName)
                    : tableName_(tableName) {}
        };

        /**
         * @brief 描述一个load data语句
         * @ingroup SQLParser
         * @details 从文件导入数据到表中。文件中的每一行就是一条数据，每行的数据类型、字段个数都与表保持一致
         */
        struct LoadDataSqlNode{
            UP<String>                              tableName_;
            UP<String>                              fileName_;
            LoadDataSqlNode(String* tableName, String* fileName)
                    : tableName_(tableName), fileName_(fileName) {}
        };

        /**
         * @brief 设置变量的值
         * @ingroup SQLParser
         * @note 当前还没有查询变量
         */
        struct SetVariableSqlNode{
            UP<String>                              tableName_;
            UP<Expression>                          value_;
            SetVariableSqlNode(String* tableName, Expression* value)
                    : tableName_(tableName), value_(value) {}
        };

        /**
         * @brief 计算值的SQL语句
         * @ingroup SQLParser
         */
        struct CalculateSqlNode{
            UP<Expression>                              expression_;
            CalculateSqlNode(Expression* expression)
                    : expression_(expression) {}
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
            ExplainSqlNode(ParsedSqlNode* sqlNode)
                    : sqlNode_(sqlNode) {}
        };

        /**
         * @brief 解析SQL语句出现了错误
         * @ingroup SQLParser
         * @details 当前解析时并没有处理错误的行号和列号
         */
        struct ErrorSqlNode{
            Vector<String>                          errorMsg_;
            Int                                     line_;
            Int                                     column_;
            ErrorSqlNode(Vector<String> errorMsg, Int line, Int column)
                    : errorMsg_(std::move(errorMsg)), line_(line), column_(column) {}
        };
        /**
         * @brief 表示一个SQL语句
         * @ingroup SQLParser
         */
        class ParsedSqlNode{
        public:
            union SqlNode {
                ErrorSqlNode* error_;
                SelectSqlNode* select_;
                InsertSqlNode* insert_;
                DeleteSqlNode* delete_;
                UpdateSqlNode* update_;
                CreateTableSqlNode* createTable_;
                DropTableSqlNode* dropTable_;
                CreateIndexSqlNode* createIndex_;
                DropIndexSqlNode* dropIndex_;
                DescTableSqlNode* descTable_;
                LoadDataSqlNode* loadData_;
                ExplainSqlNode* explain_;
                SetVariableSqlNode* setVariable_;
                CalculateSqlNode* calculate_;
            };            
            enum SqlCommandType                         type_;
            SqlNode                                     sqlNode_;

        public:
            explicit ParsedSqlNode() : type_(SqlCommandType::ERROR) {
                sqlNode_.error_ = nullptr; 
            }


            explicit ParsedSqlNode(SqlCommandType type) : type_(type) {
                sqlNode_.error_ = nullptr;
            }


            // 构造函数，接受 ErrorSqlNode
            explicit ParsedSqlNode(ErrorSqlNode* error) : type_(SqlCommandType::ERROR) {
                sqlNode_.error_ = error;
            }

            // 构造函数，接受 SelectSqlNode
            explicit ParsedSqlNode(SelectSqlNode* selection) : type_(SqlCommandType::SELECT) {
                sqlNode_.select_ = selection;
            }

            // 构造函数，接受 InsertSqlNode
            explicit ParsedSqlNode(InsertSqlNode* insertion) : type_(SqlCommandType::INSERT) {
                sqlNode_.insert_ = insertion;
            }

            // 构造函数，接受 DeleteSqlNode
            explicit ParsedSqlNode(DeleteSqlNode* deletion) : type_(SqlCommandType::DELETE) {
                sqlNode_.delete_ = deletion;
            }

            // 构造函数，接受 UpdateSqlNode
            explicit ParsedSqlNode(UpdateSqlNode* update) : type_(SqlCommandType::UPDATE) {
                sqlNode_.update_ = update;
            }

            // 构造函数，接受 CreateTableSqlNode
            explicit ParsedSqlNode(CreateTableSqlNode* createTable) : type_(SqlCommandType::CREATE_TABLE) {
                sqlNode_.createTable_ = createTable;
            }

            // 构造函数，接受 DropTableSqlNode
            explicit ParsedSqlNode(DropTableSqlNode* dropTable) : type_(SqlCommandType::DROP_TABLE) {
                sqlNode_.dropTable_ = dropTable;
            }

            // 构造函数，接受 CreateIndexSqlNode
            explicit ParsedSqlNode(CreateIndexSqlNode* createIndex) : type_(SqlCommandType::CREATE_INDEX) {
                sqlNode_.createIndex_ = createIndex;
            }

            // 构造函数，接受 DropIndexSqlNode
            explicit ParsedSqlNode(DropIndexSqlNode* dropIndex) : type_(SqlCommandType::DROP_INDEX) {
                sqlNode_.dropIndex_ = dropIndex;
            }

            // 构造函数，接受 DescTableSqlNode
            explicit ParsedSqlNode(DescTableSqlNode* descTable) : type_(SqlCommandType::DESC_TABLE) {
                sqlNode_.descTable_ = descTable;
            }

            // 构造函数，接受 LoadDataSqlNode
            explicit ParsedSqlNode(LoadDataSqlNode* loadData) : type_(SqlCommandType::LOAD_DATA) {
                sqlNode_.loadData_ = loadData;
            }

            // 构造函数，接受 ExplainSqlNode
            explicit ParsedSqlNode(ExplainSqlNode* explain) : type_(SqlCommandType::EXPLAIN) {
                sqlNode_.explain_ = explain;
            }

            // 构造函数，接受 SetVariableSqlNode
            explicit ParsedSqlNode(SetVariableSqlNode* setVariable) : type_(SqlCommandType::SET_VARIABLE) {
                sqlNode_.setVariable_ = setVariable;
            }
            
            explicit ParsedSqlNode(CalculateSqlNode* calculateSqlNode):type_(SqlCommandType::CALC){
                sqlNode_.calculate_=calculateSqlNode;
            }

            // 析构函数
            ~ParsedSqlNode() {
                // 根据 type_ 释放相应的内存
                switch (type_) {
                    case ERROR: delete sqlNode_.error_; break;
                    case SELECT: delete sqlNode_.select_; break;
                    case INSERT: delete sqlNode_.insert_; break;
                    case DELETE: delete sqlNode_.delete_; break;
                    case UPDATE: delete sqlNode_.update_; break;
                    case CREATE_TABLE: delete sqlNode_.createTable_; break;
                    case DROP_TABLE: delete sqlNode_.dropTable_; break;
                    case CREATE_INDEX: delete sqlNode_.createIndex_; break;
                    case DROP_INDEX: delete sqlNode_.dropIndex_; break;
                    case DESC_TABLE: delete sqlNode_.descTable_; break;
                    case LOAD_DATA: delete sqlNode_.loadData_; break;
                    case EXPLAIN: delete sqlNode_.explain_; break;
                    case SET_VARIABLE: delete sqlNode_.setVariable_; break;
                    default: break;
                }
            }
        };
    }
}
