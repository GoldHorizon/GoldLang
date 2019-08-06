#include "ast_nodes.h"

using namespace ast;

node::~node() {}
root::~root() {
    delete statement_list;
}
statement::~statement() {
    delete lhs;
}
definition::~definition() {}
expression::~expression() {}
func_def::~func_def() {
    delete rhs_params;
    delete rhs_code;
}
func_call::~func_call() {
    delete rhs;
}
return_call::~return_call() {
    delete rhs;
}
parameters::~parameters() {
    for (auto it : identifier_list)
        delete it;
}
code::~code() {
    for (auto it : statement_list)
        delete it;
}
eval::~eval() {
    delete inside;
}