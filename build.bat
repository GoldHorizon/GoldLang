clang-cl -c globals.cpp
clang-cl -c types.cpp
clang-cl -c reporting.cpp
clang-cl -c interface.cpp
clang-cl -c clock.cpp
clang-cl -c lexer.cpp
clang-cl -c ast_nodes.cpp
clang-cl -c parser.cpp
clang-cl main.cpp globals.obj types.obj reporting.obj interface.obj clock.obj lexer.obj parser.obj ast_nodes.obj -o compiler.exe
