TARGET       := differentiator

BUILD_DIR    := build
INCLUDE_DIR  := include
EXAMPLES_DIR := examples
SRC_DIR      := src

SRCS         := $(wildcard $(SRC_DIR)/*.cpp)
OBJS         := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS         := $(OBJS:.o=.d)

CXX          := clang++
CXXFLAGS     := -O2 -Wall -Wextra -std=c++20 -pedantic-errors
CPPFLAGS     := -MMD -MP -I$(INCLUDE_DIR)/

RM           := rm -f 

.PHONY: all clean fclean re check-format
.PRECIOUS: $(BUILD_DIR)/%.o $(BUILD_DIR)/%.d

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -c -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@

all: $(TARGET)

clean:
	$(RM) -r $(BUILD_DIR)

fclean: clean
	$(RM) $(TARGET)

re:
	$(MAKE) fclean
	$(MAKE) all

check-format:
	clang-format --dry-run --Werror $(shell find $(SRC_DIR) $(INCLUDE_DIR) \
			-name '*.cpp' -o -name '*.hpp' -o -name '*.h')
