#include <iostream>

#include "reporting.h"

void report_tabs(int t) {
    for (int i = 0; i < t; ++i)
        report_message("  ");
}

// Messages
void report_message(const char* s) {
    std::cout << s;
}