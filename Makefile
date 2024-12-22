CXX = clang++
CXX_FLAGS = -O0 -std=c++23  -fno-exceptions  -fconstexpr-steps=268435456 -fbracket-depth=65535 -g #-Wall -Wfatal-errors -Wpedantic -Wextra -Wshadow -Wconversion

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
