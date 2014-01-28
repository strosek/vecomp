###############################################################################
# Makefile for building vecomp compiler.
###############################################################################

# Environment macros: Compiler to use, Compile Flags, Link Flags
CXX := clang++
CFLAGS := -Wall -Wextra -Weffc++ -Wswitch -Wshadow -Wc++11-compat --std=c++11 -c
LFLAGS := -Wall -Wextra -Weffc++ -Wswitch -Wshadow -Wc++11-compat --std=c++11

# Binary, Source, Include, Object Directories
BDIR := ../binaries
SDIR := source
IDIR := headers
ODIR := ../binaries/object

# Object files
_OBJ := main.o Parser.o SemanticChecker.o SymbolsTable.o SymbolData.o Scanner.o FileReader.o TokenLexeme.o ErrorReporter.o
OBJ := $(_OBJ:%.o=$(ODIR)/%.o)

# Binary file (executable)
_BIN := vecomp 
BIN := $(BDIR)/$(_BIN)

####################

# Special to compile all
.PHONY: all
all: $(BIN)

debug: CFLAGS += -D DEBUG -g
debug: LFLAGS += -D DEBUG -g
debug: $(BIN)
release: CFLAGS += -O2
release: LFLAGS += -O2
release: $(BIN)

# Special to clean everything
.PHONY: clean
clean:
	rm -f *~
	rm -f -r $(ODIR)
	rm -f -r $(BDIR)

####################

# Make Binary
$(BIN): $(OBJ)
	if [ ! -d $(BDIR) ]; then\
		mkdir $(BDIR);\
	fi
	$(CXX) $(LFLAGS) $^ -o $@
	touch $@

# Make Objects
$(ODIR)/%.o: $(SDIR)/%.cpp $(IDIR)/%.hpp
	if [ ! -d $(ODIR) ]; then\
		mkdir -p $(ODIR);\
	fi
	$(CXX) $(CFLAGS) $< -o $@
	touch $@

