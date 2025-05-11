CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic -g -Iinclude

TARGET = cyclist_sim
TEST_TARGET = cyclist_sim_tests

SRC_DIR = src
TEST_DIR = tests
INCLUDE_DIR = include

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/simulation.cpp $(SRC_DIR)/hills.cpp
OBJS = $(SRCS:.cpp=.o)

TEST_SRCS = $(TEST_DIR)/simulation_test.cpp $(SRC_DIR)/simulation.cpp $(SRC_DIR)/hills.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -lgtest -lgtest_main -lpthread -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)