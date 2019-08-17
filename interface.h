#pragma once

#include <vector>
#include <string>

#include "reporting.h"

void read_arguments(std::vector<char*>& files, int argc, char** argv);
int read_option(char* option, char* value);

void option_print_help();
void option_thread_count(char* option, char* value);
void option_toggle_debug();
