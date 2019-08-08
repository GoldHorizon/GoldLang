#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "reporting.h"

extern uint16_t warning_count;
extern uint16_t error_count;

int main (int argc, char** argv) {

    lexer l;
    l.read_file("start.g");

    if (warning_count > 0 || error_count > 0) {
        report_message("Warnings: % | Errors: %\n", warning_count, error_count);
        return 1;
    }

    parser p(l.tokens);
    p.build_tree();

    if (warning_count > 0 || error_count > 0) {
        report_message("Warnings: % | Errors: %\n", warning_count, error_count);
        return 1;
    }

    p.print_tree();

    if (warning_count > 0 || error_count > 0) {
        report_message("Warnings: % | Errors: %\n", warning_count, error_count);
        return 1;
    }
}
