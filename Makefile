PROGNAME =	MSGdb
PROG =		$(BIN_DIR)/$(PROGNAME)

BIN_DIR =	bin
LIB_DIR =	lib
SRC_DIR =	src

VPATH =		$(LIB_DIR) $(SRC_DIR) $(BIN_DIR)

PROG_OBJS =	$(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))

LIB_OBJS =	$(patsubst $(LIB_DIR)/%.cpp, $(BIN_DIR)/%.o, $(wildcard $(LIB_DIR)/*.cpp))
 
OBJS =		$(LIB_OBJS)

CXX ?=		g++  # '?=' allows CXX to be redefined in shell prior to running makefile
DISABLE_WARNINGS = -Wno-unused-parameter -Wno-attributes -Wno-delete-non-virtual-dtor
CXX_FLAGS =	-g -Wall $(DISABLE_WARNINGS) -Wextra -pedantic-errors -std=c++14
CXX_SANFLAGS =	-fsanitize=address -fsanitize=undefined # These flags are causing linking errors at runtime
INCLUDES =	-Iinclude


# ANTLR STUFF - FROM build.sh PROVIDED BY BILL
################################################
ANTLR_HEADER_PATH = -Iantlr_cpp_src/runtime/src -Iantlr_output
ANTLR_OUTPUT_PATH=antlr_output
ANTLR_LIB_PATH=./ 
#####################END OF ANTLR STUFF ########

all: $(PROG)

test: all
	@./test.sh test.txt

$(PROG): $(notdir $(OBJS)) $(SRC_DIR)/*.cpp include/*.h
	$(CXX) $(CXX_FLAGS) -Iantlr_cpp_src/runtime/src $(INCLUDES) -I$(ANTLR_OUTPUT_PATH) -o $@ $(OBJS) ./src/main.cpp $(ANTLR_OUTPUT_PATH)/*.cpp -L$(ANTLR_LIB_PATH) -lantlr4-runtime

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $(ANTLR_HEADER_PATH) -c -o $(BIN_DIR)/$@ $^ 


clean:
	rm -f $(PROG) $(OBJS)

.PHONY: all test clean
