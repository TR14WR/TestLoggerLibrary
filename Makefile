export CXX = g++
export INCLUDE_DIR = $(CURDIR)/api
export BUILD_DIR = $(CURDIR)/build
export CXXFLAGS = -std=c++17 -Wall -MMD -I$(INCLUDE_DIR)
export UTIL = $(BUILD_DIR)/logger
export LIB = $(BUILD_DIR)/liblogger.so
export test = $(BUILD_DIR)/api/test
export LDFLAGS = -Wl,-rpath,$$origin

.PHONY: all clean test

all: $(BUILD_DIR) $(LIB) $(UTIL)

test:
	$(MAKE) -C api test

$(LIB):
	$(MAKE) -C api

$(UTIL):
	$(MAKE) -C logger

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)