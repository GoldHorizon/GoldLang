#include <fstream>
#include <iostream>

#include "lexer.h"
#include "reporting.h"

re_type::~re_type() {
    delete re;
}

lexer::lexer() {
    // C style regex
    // Comments
    auto re_comment = new std::regex ("^//.*");
    re_list.push_back(new re_type {re_comment, token_type::COMMENT});

    // Create keywords
    auto keywords = {"^return", 
                     "^print", 
                     "^assert", 
                     "^if", 
                     "^for", 
                     "^while", 
                     "^switch", 
                     "^case", 
                     "^break", 
                     "^continue",
                     "^struct",
                     "^int",
                     "^bool",
                     "^float",
                     "^string"};

    for (auto word : keywords) {
        re_list.push_back(new re_type {new std::regex (word), token_type::KEYWORD});
    }

    // Identifiers
    auto re_identifier = new std::regex ("^[[:alpha:]]\\w*");
    re_list.push_back(new re_type {re_identifier, token_type::IDENTIFIER});

    // Constants
    auto re_number     = new std::regex ("^[[:digit:]]+(\\.[[:digit:]]*)?");
    auto re_hex_number = new std::regex ("^0x[[:xdigit:]]+");
    re_list.push_back(new re_type {re_number, token_type::CONSTANT});
    re_list.push_back(new re_type {re_hex_number, token_type::CONSTANT});

    // Strings
    auto re_string = new std::regex ("^\"[^\"]*\"");
    re_list.push_back(new re_type {re_string, token_type::STRING});

    // Symbols
    auto re_symbols = new std::regex ("^[;{}()\\[\\]]");
    re_list.push_back(new re_type {re_symbols, token_type::SYMBOL});

    // Operators
    auto re_operators = new std::regex ("^(==)|[:=,+-/*%]");
    re_list.push_back(new re_type {re_operators, token_type::OPERATOR});

}

lexer::~lexer() {
    for (auto t : tokens)  if (t) delete t;
    for (auto r : re_list) if (r) delete r;
}

void lexer::read_file(std::string file_name) {
    report_message("Begin reading file '%'\n", file_name);
    std::ifstream file;
    std::string line;
    int line_num = 1;
    int column_num = 0;

    file.open(file_name);

    if (!file) {
        // @todo Make into error report
        report_error("Could not open file %\n", file_name);
        return;
    }

    // @todo: Make each line work concurrently on another thread
    std::getline(file, line);
    while (file) {
        // Read tokens from line
        if (line.length() > 0) {
            std::string current_line = line;

            while (current_line[0] == ' ' || current_line[0] == '\t') {
                // @cleanup: This is SUPER inefficient I feel. 
                // Maybe use pointer to beginning of string, increment to search?
                current_line = current_line.substr(1);
            }

            while (current_line != "") {
                bool matched = false;
                std::smatch re_match;

                for (auto re_t : re_list) {
                    if (std::regex_search(current_line, re_match, *(re_t->re))) {
                        if (re_t->type != token_type::NONE && re_t->type != token_type::COMMENT) {
                            report_message("Found: %\n", re_match.str());

                            push_token(new token(re_t->type, re_match.str(), line_num, column_num));
                        } else if (re_t->type != token_type::COMMENT) {
                            report_error("Invalid type token: %\n", re_match.str());
                        }

                        current_line = re_match.suffix();
                        matched = true;
                        break;
                    }
                }

                if (!matched) {
                    report_error("Error: Couldn't match on line %: %\n", line_num, current_line);
                    return;
                }

                while (current_line[0] == ' ' || current_line[0] == '\t') {
                    // @cleanup: Same as above
                    current_line = current_line.substr(1);
                }
            }
        }

        std::getline(file, line);
        line_num++;
    }

    file.close();
}

void lexer::push_token(token* t) {
    tokens.push_back(t);
}
