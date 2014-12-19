HEADERS = daemon.hpp
SOURCES = main.cpp
OBJ = $(SOURCES:%.cpp=%.cpp.o)
BIN = run

all: $(BIN)

$(BIN) : $(OBJ)
	g++ -Wall -lmicrohttpd $^ -o $@

%.cpp.o: %.cpp $(HEADERS)
	g++ -Wall -c -std=c++11 $< -o $@
