CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I./src -MMD -MP
#                   ^^^^^       ^^^^^^^
#  -MMD: generate a .d file with all the headers
#  -MP:  add phony targets for deleted headers (avoids errors)

SRC       := $(wildcard src/*.cpp)
OBJ       := $(patsubst src/%.cpp, build/%.o, $(SRC))
DEPFILES  := $(OBJ:.o=.d)
TARGET    := build/chess

all: $(TARGET)

# link
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@

# compile + dependency generation
build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build

.PHONY: all run clean

# include the .d files if they exist
-include $(DEPFILES)
