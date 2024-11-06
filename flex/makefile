# Settings
# Set to 0 to enable C mode
CPP_MODE := 1
ifeq ($(CPP_MODE), 0)
FB_EXT := .c
else
FB_EXT := .cpp
endif

# Flags
CFLAGS := -Wall -std=c11
CXXFLAGS := -Wall -Wno-register -std=c++17
FFLAGS :=
BFLAGS := -d
LDFLAGS :=

# Debug flags
DEBUG ?= 1
ifeq ($(DEBUG), 0)
CFLAGS += -O2
CXXFLAGS += -O2
else
CFLAGS += -g -O0
CXXFLAGS += -g -O0
endif

# Compilers
CC := clang
CXX := clang++
FLEX := flex
BISON := bison

# Directories
TOP_DIR := $(shell pwd)
TARGET_EXEC := compiler
SRC_DIR := $(TOP_DIR)/src
BUILD_DIR ?= $(TOP_DIR)/build

# Source files & target files
SRCS := $(shell find $(SRC_DIR) -name "*.c" -or -name "*.cpp" -or -name "*.cc")
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.c.o, $(SRCS))
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.cpp.o, $(OBJS))
OBJS := $(patsubst $(SRC_DIR)/%.cc, $(BUILD_DIR)/%.cc.o, $(OBJS))
OBJS := $(patsubst $(BUILD_DIR)/%.c, $(BUILD_DIR)/%.c.o, $(OBJS))
OBJS := $(patsubst $(BUILD_DIR)/%.cpp, $(BUILD_DIR)/%.cpp.o, $(OBJS))
OBJS := $(patsubst $(BUILD_DIR)/%.cc, $(BUILD_DIR)/%.cc.o, $(OBJS))

# Header directories & dependencies
INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_DIRS += $(INC_DIRS:$(SRC_DIR)%=$(BUILD_DIR)%)
INC_FLAGS := $(addprefix -I, $(INC_DIRS))
DEPS := $(OBJS:.o=.d)
CPPFLAGS = $(INC_FLAGS) -MMD -MP


# Main target
$(BUILD_DIR)/$(TARGET_EXEC):  $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -lpthread -ldl -o $@

# C source
define c_recipe
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
endef
$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c; $(c_recipe)
$(BUILD_DIR)/%.c.o: $(BUILD_DIR)/%.c; $(c_recipe)

# C++ source
define cxx_recipe
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
endef
$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp; $(cxx_recipe)
$(BUILD_DIR)/%.cpp.o: $(BUILD_DIR)/%.cpp; $(cxx_recipe)
$(BUILD_DIR)/%.cc.o: $(SRC_DIR)/%.cc; $(cxx_recipe)

.PHONY: clean

clean:
	-rm -rf $(BUILD_DIR)

-include $(DEPS)