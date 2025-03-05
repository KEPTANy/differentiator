TARGET_LIB   := differentiator.a
TARGET       := differentiator

BUILD_DIR    := build
INCLUDE_DIR  := include
LIB_DIR      := lib
SRC_DIR      := src
TEST_DIR     := test

SRCS         := $(filter-out $(SRC_DIR)/main.cpp,$(wildcard $(SRC_DIR)/*.cpp))
SRC_MAIN     := $(SRC_DIR)/main.cpp
OBJS         := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
OBJ_MAIN     := $(SRC_MAIN:%.cpp=$(BUILD_DIR)/%.o)
DEPS         := $(OBJS:.o=.d)

CXX          := clang++
CXXFLAGS     := -O2 -Wall -Wextra -std=c++20 -pedantic-errors
CPPFLAGS     := -MMD -MP -I$(INCLUDE_DIR)/
AR           := ar
ARFLAGS      := -r -c -s

TEST_SRCS    := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS    := $(TEST_SRCS:%.cpp=$(BUILD_DIR)/%.o)
TEST_FLAGS   := -I$(LIB_DIR)/doctest/doctest/ -DDIFFERENTIATOR_TEST_PRIVATE
TEST_TARGET  := $(TEST_DIR)/test_runner

RM           := rm -f 

.PHONY: all clean fclean re check-format test get-dependencies
.PRECIOUS: $(BUILD_DIR)/%.o $(BUILD_DIR)/%.d
.DEFAULT_GOAL: all

all: $(TARGET_LIB) $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -c -o $@

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp get-dependencies
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(TEST_FLAGS) $(CPPFLAGS) $< -c -o $@

$(TARGET_LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(TARGET): $(TARGET_LIB) $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@


$(TEST_TARGET): $(TEST_OBJS) $(TARGET_LIB)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	$(RM) -r $(BUILD_DIR)

fclean: clean
	$(RM) $(TARGET)
	$(RM) $(TARGET_LIB)
	$(RM) $(TEST_TARGET)

re:
	$(MAKE) fclean
	$(MAKE) all

get-dependencies:
	git submodule update --init --recursive

check-format:
	clang-format --dry-run --Werror $(shell find $(SRC_DIR) $(INCLUDE_DIR) \
			-name '*.cpp' -o -name '*.hpp' -o -name '*.h')
