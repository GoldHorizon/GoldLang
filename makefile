output=compiler
compiler=g++
c_flags=-std=c++11
l_flags=-pthread

all : types.o reporting.o clock.o lexer.o ast_nodes.o parser.o
	${compiler} ${c_flags} ${l_flags} main.cpp types.o reporting.o clock.o lexer.o parser.o ast_nodes.o -o ${output}

types.o : types.cpp types.h
	${compiler} ${c_flags} -c types.cpp

reporting.o : reporting.cpp reporting.h
	${compiler} ${c_flags} -c reporting.cpp
	
clock.o : clock.cpp clock.h
	${compiler} ${c_flags} -c clock.cpp

lexer.o : lexer.cpp lexer.h
	${compiler} ${c_flags} -c lexer.cpp

ast_nodes.o : ast_nodes.cpp ast_nodes.h
	${compiler} ${c_flags} -c ast_nodes.cpp

parser.o : parser.cpp parser.h
	${compiler} ${c_flags} -c parser.cpp

clean :
	rm *.o
	rm ${output}
