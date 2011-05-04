SRCDIR=src
BINDIR=bin
SOURCES=$(wildcard $(SRCDIR)/*.cpp)
HEADERS=$(wildcard $(SRCDIR)/*.hpp)
OBJS=$(SOURCES:.cpp=.o)
EXEC=jl

CC=g++
CFLAGS=

LD=ld
LFLAGS=

ifdef profile
CFLAGS+=-pg
LFLAGS+=-pg
endif

ifndef nowall
CFLAGS+=-Wextra -pedantic -Weffc++ -Wswitch-default -Wswitch-enum
CFLAGS+=-Wmissing-include-dirs -Winline -Wfloat-equal -Wundef
CFLAGS+=-Wcast-align -Wredundant-decls -Winit-self -Wshadow
endif

ifdef release
CFLAGS+=-O3 -s
else
CFLAGS+=-g
endif

all: config $(EXEC)

config:
	./mkversion.sh

$(EXEC): $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$(EXEC) $(LFLAGS) $?

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BINDIR)/$(EXEC)
	rm -f $(SRCDIR)/*.o

