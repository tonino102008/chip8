# For now, building only tests

# Points to the root of Google Test, relative to where this file is.
GTEST_DIR = ./test/googletest/googletest
# User files.
TEST_DIR = ./test/test-chip8
TEST_INCLUDE_DIR = $(TEST_DIR)
USER_DIR = ./src
USER_INCLUDE_DIR = ./include
# Build Directory
BUILD_DIR = ./build
# C Compiler
CC = gcc
# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include
CFLAGS = -Wall -Wextra
# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

# All tests produced by this Makefile.
TESTS = $(BUILD_DIR)/chip8tests
MAIN = $(BUILD_DIR)/main

# All Google Test headers.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h


all : dir $(TESTS) $(MAIN)

clean :
	@ if [ -d "$(BUILD_DIR)" ]; then \
		rm -r $(BUILD_DIR); \
	else \
		echo "The directory $(BUILD_DIR) doesn't exist."; \
	fi

dir :
	@ if [ -d "$(BUILD_DIR)" ]; then \
		echo "The directory $(BUILD_DIR) already exist."; \
	else \
		mkdir -p $(BUILD_DIR); \
	fi

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

$(BUILD_DIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

$(BUILD_DIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

$(BUILD_DIR)/gtest.a : $(BUILD_DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/gtest_main.a : $(BUILD_DIR)/gtest-all.o $(BUILD_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/memory.o : $(USER_DIR)/memory.c $(USER_INCLUDE_DIR)/types.h $(USER_INCLUDE_DIR)/memory.h
	$(CC) $(CFLAGS) -c $(USER_DIR)/memory.c
	@ mv memory.o $@

$(BUILD_DIR)/stack.o : $(USER_DIR)/stack.c $(USER_INCLUDE_DIR)/types.h $(USER_INCLUDE_DIR)/stack.h
	$(CC) $(CFLAGS) -c $(USER_DIR)/stack.c
	@ mv stack.o $@

$(BUILD_DIR)/system.o : $(USER_DIR)/system.c $(USER_INCLUDE_DIR)/types.h $(USER_INCLUDE_DIR)/system.h
	$(CC) $(CFLAGS) -c $(USER_DIR)/system.c
	@ mv system.o $@

$(BUILD_DIR)/register.o : $(USER_DIR)/register.c $(USER_INCLUDE_DIR)/types.h $(USER_INCLUDE_DIR)/register.h
	$(CC) $(CFLAGS) -c $(USER_DIR)/register.c
	@ mv register.o $@

$(BUILD_DIR)/display.o : $(USER_DIR)/display.c $(USER_INCLUDE_DIR)/types.h $(USER_INCLUDE_DIR)/display.h
	$(CC) $(CFLAGS) -c $(USER_DIR)/display.c
	@ mv display.o $@

$(BUILD_DIR)/chip8tests.o : $(TEST_DIR)/chip8tests.cc \
                    $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/chip8tests.cc -o $@

$(BUILD_DIR)/chip8tests : $(BUILD_DIR)/memory.o $(BUILD_DIR)/stack.o \
					$(BUILD_DIR)/system.o $(BUILD_DIR)/register.o $(BUILD_DIR)/display.o \
					$(BUILD_DIR)/chip8tests.o $(BUILD_DIR)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

$(BUILD_DIR)/main.o : $(USER_DIR)/main.c $(USER_INCLUDE_DIR)/types.h $(USER_INCLUDE_DIR)/display.h \
					$(USER_INCLUDE_DIR)/register.h $(USER_INCLUDE_DIR)/stack.h $(USER_INCLUDE_DIR)/memory.h \
					$(USER_INCLUDE_DIR)/system.h
	$(CC) $(CFLAGS) -c $(USER_DIR)/main.c
	@ mv main.o $@

$(BUILD_DIR)/main : $(BUILD_DIR)/memory.o $(BUILD_DIR)/stack.o \
					$(BUILD_DIR)/system.o $(BUILD_DIR)/register.o $(BUILD_DIR)/display.o \
					$(BUILD_DIR)/main.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@