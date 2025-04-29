#pragma once

#include "../../../global_def.h"
#include "../../../smart_ptr.h"
#include "../sql/parser/parser.h"
#include "../../common/arithmetic_type.h"
namespace wa{
    namespace db{
        
        class Expression{};
        class UnboundedExpression : public Expression{
            UP<String> table_;
            UP<String> field_;
        public:
            UnboundedExpression(FieldNode* fieldNode): table_(fieldNode->tableName_), field_(fieldNode->fieldName_){}
            UnboundedExpression(String * fieldName): table_(NULLPTR),field_(fieldName){}
        };
        
        class ArithmeticExpression : public Expression{
            UP<Expression> left_;
            ArithmeticType op_;
            UP<Expression> right_;
        public:
            ArithmeticExpression(Expression* left, ArithmeticType op, Expression* right)
                    : left_(left), op_(op), right_(right) {}
            ArithmeticExpression() : left_(NULLPTR), op_(ArithmeticType::ADD), right_(NULLPTR) {}
        };
        
        class StarExpression : public Expression{
        public:
            StarExpression(){}
        };
    };
};