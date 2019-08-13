#pragma once

#include <vector>
#include <string>

#include "reporting.h"

void read_arguments(std::vector<char*>& files, int argc, char** argv);
int read_option(char* option, char* value);

void print_help();
