#include "ast_nodes.h"

using namespace ast;

// Node (base)
node::~node() {}

// Root
root::~root() {
    delete statement_list;
}

void root::print(int t) {
    
}

// Statement
statement::~statement() {
    delete lhs;
}

// Definition
definition::~definition() {}

// Expression
expression::~expression() {}

// Function definition
func_def::~func_def() {
    delete rhs_params;
    delete rhs_code;
}

void func_def::print(int t) {

}

// Function call
func_call::~func_call() {
    delete rhs;
}

void func_call::print(int t) {

}

// Return call
return_call::~return_call() {
    delete rhs;
}

void return_call::print(int t) {

}

// Parameters
parameters::~parameters() {
    for (auto it : identifier_list)
        delete it;
}

void parameters::print(int t) {

}

// Code
code::~code() {
    for (auto it : statement_list)
        delete it;
}

void code::print(int t) {

}

// Identifier
void identifier::print(int t) {

}

// Number
void number::print(int t) {

}