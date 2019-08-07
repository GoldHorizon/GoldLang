#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "reporting.h"

int main (int argc, char** argv) {

    lexer l;
    l.read_file("start.g");

    parser p(l.tokens);
    p.build_tree();
    p.print_tree();

    if (warning_count > 0 || error_count > 0)
        report_message("Warnings: % | Errors: %\n", warning_count, error_count);
}
