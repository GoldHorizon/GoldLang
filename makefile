compiler=g++
flag=-std=c++11

all : types.o reporting.o clock.o lexer.o ast_nodes.o parser.o
	${compiler} ${flag} -pthread main.cpp types.o reporting.o clock.o lexer.o parser.o ast_nodes.o -o compiler

types.o : types.cpp types.h
	${compiler} ${flag} -c types.cpp

reporting.o : reporting.cpp reporting.h
	${compiler} ${flag} -c reporting.cpp
	
clock.o : clock.cpp clock.h
	${compiler} ${flag} -c clock.cpp

lexer.o : lexer.cpp lexer.h
	${compiler} ${flag} -c lexer.cpp

ast_nodes.o : ast_nodes.cpp ast_nodes.h
	${compiler} ${flag} -c ast_nodes.cpp

parser.o : parser.cpp parser.h
	${compiler} ${flag} -c parser.cpp

clean :
	rm *.o
