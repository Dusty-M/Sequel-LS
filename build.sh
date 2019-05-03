
ANTLR_HEADER_PATH=./antlr_cpp_src/runtime/src
ANTLR_OUTPUT_PATH=./antlr_output
ANTLR_LIB_PATH=.

EXECUTABLE_NAME=sqltest
g++ -std=c++17 -I${ANTLR_HEADER_PATH} -I${ANTLR_OUTPUT_PATH} -o $EXECUTABLE_NAME main.cpp ${ANTLR_OUTPUT_PATH}/*.cpp -L${ANTLR_LIB_PATH} -lantlr4-runtime 