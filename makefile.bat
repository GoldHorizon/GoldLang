clang-cl -c reporting.cpp
clang-cl -c lexer.cpp
clang-cl -c ast_nodes.cpp
clang-cl -c parser.cpp
clang-cl main.cpp reporting.obj lexer.obj parser.obj ast_nodes.obj -o compiler.exe