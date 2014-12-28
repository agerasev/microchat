SRCDIR = source
OBJDIR = obj
HEADERS = \
	daemon.hpp \
	database.hpp \
	chatdaemon.hpp \
	chatdatabase.hpp
SOURCES = \
	main.cpp \
	daemon.cpp \
	database.cpp \
	chatdaemon.cpp \
	chatdatabase.cpp

OBJ = $(SOURCES:%.cpp=$(OBJDIR)/%.cpp.o)
SRC = $(SOURCES:%=$(SRCDIR)/%)
HDR = $(HEADERS:%=$(SRCDIR)/%)
BIN = run

all: $(OBJDIR) $(BIN)

$(BIN): $(OBJ)
	g++ -Wall -lmicrohttpd -lmysqlcppconn $^ -o $@

$(OBJDIR)/%.cpp.o: $(SRCDIR)/%.cpp $(HDR)
	g++ -Wall -c -std=c++11 $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)
