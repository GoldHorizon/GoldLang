#include <iostream>

#include "reporting.h"

uint16_t warning_count;
uint16_t error_count;


void report_tabs(int t) {
    for (int i = 0; i < t; ++i)
        report_message("  ");
}

// Messages
void report_message(const char* s) {
    std::cout << s;
}
