output=compiler
compiler=g++
c_flags=-std=c++11
l_flags=-pthread

all : globals.o types.o reporting.o interface.o clock.o lexer.o ast_nodes.o parser.o
	${compiler} ${c_flags} ${l_flags} main.cpp globals.o types.o reporting.o interface.o clock.o lexer.o parser.o ast_nodes.o -o ${output}

globals.o : globals.cpp globals.h
	${compiler} ${c_flags} -c globals.cpp

types.o : types.cpp types.h
	${compiler} ${c_flags} -c types.cpp

reporting.o : reporting.cpp reporting.h
	${compiler} ${c_flags} -c reporting.cpp
	
interface.o : interface.cpp interface.h
	${compiler} ${c_flags} -c interface.cpp

clock.o : clock.cpp clock.h
	${compiler} ${c_flags} -c clock.cpp

lexer.o : lexer.cpp lexer.h
	${compiler} ${c_flags} -c lexer.cpp

ast_nodes.o : ast_nodes.cpp ast_nodes.h
	${compiler} ${c_flags} -c ast_nodes.cpp

parser.o : parser.cpp parser.h
	${compiler} ${c_flags} -c parser.cpp

clean :
	@-rm -f *.o
	@-rm -f ${output}
