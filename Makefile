CXX = clang++
CXX_FLAGS = -O0 -g -Wall -Wfatal-errors -Wpedantic -Wextra -Wshadow -Wconversion -std=c++23 -fconstexpr-steps=8388608

BINDIR = ./build
CPP = $(wildcard ./*.cpp)
BIN = $(CPP:%.cpp=$(BINDIR)/%)

$(BINDIR)/% : %.cpp %.hpp
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $< -o $@


.PHONY: clean all

all: $(BIN)
clean:
	-rm $(BIN)
