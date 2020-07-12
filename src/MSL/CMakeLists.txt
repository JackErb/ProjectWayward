add_library(MSLLib STATIC Msl.hpp MslInterpreter.hpp MslInterpreter.cpp MslInterpreter_visitor.cpp MslParser.hpp MslParser.cpp MslScanner.hpp
		MslScanner.cpp)

add_subdirectory(AST)
