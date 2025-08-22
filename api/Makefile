SRC = src/logger.cpp
CXXFLAGS += -fPIC
LDFLAGS = -shared

$(LIB): $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

test: $(LIB)
	$(MAKE) -C test

