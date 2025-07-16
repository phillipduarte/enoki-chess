CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I./src -MMD -MP -g
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

# Add valgrind memory check target
valgrind-memcheck: $(TARGET)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) $(ARGS)

.PHONY: all run clean valgrind-memcheck

# include the .d files if they exist
-include $(DEPFILES)
