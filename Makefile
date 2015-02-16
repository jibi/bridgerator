CXX=clang++
CXXFLAGS=-Wall -pedantic -Ofast -I ./include --std=c++11
SOURCES=src/client.cpp  src/config.cpp  src/listener.cpp  src/main.cpp  src/server.cpp  src/socket_engine.cpp  src/socks5.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=bridgerator

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

all: $(SOURCES) $(EXECUTABLE)

clean:
	@rm $(EXECUTABLE) $(OBJECTS)
