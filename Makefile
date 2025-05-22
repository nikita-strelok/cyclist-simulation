CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic -g -I$(INCLUDE_DIR) $(shell pkg-config --cflags sdl3)
LDFLAGS = $(shell pkg-config --libs sdl3)

TARGET = cyclist_sim
TEST_TARGET = cyclist_sim_tests

SRC_DIR = src
TEST_DIR = tests
INCLUDE_DIR = include
BUILD_DIR = .build
BUILD_APP = $(BUILD_DIR)/app
BUILD_TEST = $(BUILD_DIR)/test

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

APP_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_APP)/%.o,$(SRCS))

TEST_SRC_SRCS = $(filter-out $(SRC_DIR)/main.cpp,$(SRCS))
TEST_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_TEST)/%.o,$(TEST_SRC_SRCS)) \
            $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_TEST)/%.o,$(TEST_SRCS))

all: $(TARGET)

$(TARGET): $(APP_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_APP)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_APP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_TEST)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_TEST)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_TEST)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_TEST)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -lgtest -lgtest_main -lpthread -o $@

$(BUILD_APP) $(BUILD_TEST):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)
