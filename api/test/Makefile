SRC = src/test_api.cpp

$(TEST): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ -L$(BUILD_DIR) -llogger -Wl,-rpath,$(BUILD_DIR)