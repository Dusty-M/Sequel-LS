PROGNAME =	MSGdb
PROG =		$(BIN_DIR)/$(PROGNAME)

VPATH = 	$(ANTLR_OUT_DIR) $(LIB_DIR) $(SRC_DIR)

BIN_DIR =	bin
LIB_DIR =	lib
SRC_DIR =	src

ANTLR_LIB =	antlr4-runtime
ANTLR_LIB_DIR =	.
ANTLR_INC_DIR =	antlr_cpp_src/runtime/src
ANTLR_OUT_DIR =	antlr_output

ANTLR_SRCS =	SQLBaseVisitor.cpp SQLLexer.cpp SQLParser.cpp SQLVisitor.cpp
LIB_SRCS =	$(wildcard $(LIB_DIR)/*.cpp)
PROG_SRCS =	$(wildcard $(SRC_DIR)/*.cpp)

ANTLR_OBJS =	$(patsubst %.cpp, $(BIN_DIR)/%.o, $(ANTLR_SRCS))
LIB_OBJS =	$(patsubst $(LIB_DIR)/%.cpp, $(BIN_DIR)/%.o, $(LIB_SRCS))
PROG_OBJS =	$(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(PROG_SRCS))

OBJS =		$(ANTLR_OBJS) $(LIB_OBJS) $(PROG_OBJS)

CXX ?=		g++  # '?=' allows CXX to be redefined in shell prior to running makefile
DISABLE_WARNINGS = -Wno-unused-parameter -Wno-attributes -Wno-delete-non-virtual-dtor
CXXFLAGS =	-g -Wall -Wextra -pedantic-errors $(DISABLE_WARNINGS) -std=c++11
CXXSANFLAGS =	-fsanitize=address -fsanitize=undefined # These flags are causing linking errors at runtime
INCLUDES =	-Iinclude -I$(ANTLR_INC_DIR) -I$(ANTLR_OUT_DIR)

ANTLR_GEN :=	$(shell if ! [ "$$(ls $(ANTLR_OUT_DIR))" ] ; then \
			java -jar antlr-4.7.1-complete.jar -Dlanguage=Cpp \
			-no-listener -visitor -o $(ANTLR_OUT_DIR) SQL.g4 ; \
		fi)

all: $(PROG)

test: all
	@./test.sh test.txt

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJS) -L$(ANTLR_LIB_DIR) -l$(ANTLR_LIB)

$(BIN_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $^

clean:
	rm -f $(PROG) $(OBJS)

cleanall: clean
	rm -f antlr_output/* *.table

.PHONY: all test clean cleanall
