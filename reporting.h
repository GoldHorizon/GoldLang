#pragma once

#include <string>
#include <iostream>

#include "globals.h"

#define allow_debug

extern uint16_t warning_count;
extern uint16_t error_count;

void report_tabs(int t);

// Messages
void report_message(const char* s);

template<typename T, typename... A>
void report_message(const char* s, T v, A... args) {
    if (s == nullptr) return;

    for (; s[0] != '\0'; ++s) {
        if (s[0] == '%') {
            std::cout << v;
            report_message(s + 1, args...);

            return;
        }

        std::cout << s[0];
    }
}

template<typename... A>
void report_warning(A... args) {
    ++warning_count;
    std::cout << "Warning: ";
    report_message(args...);
}

template<typename... A>
void report_error(A... args) {
    ++error_count;
    std::cout << "Error: ";
    report_message(args...);
}

#ifdef allow_debug
template<typename... A>
void report_debug(A... args) {
	if (debug_mode) {
		std::cout << "DEBUG: ";
		report_message(args...);
	}
}
#endif // allow_debug

//template<typename T, typename... A>
//void report_message(std::string s, T v, A... args);

//void report_warning(std::string s);
//void report_error(std::string s);
