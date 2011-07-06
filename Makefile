SRCDIR=src
BINDIR=bin
SOURCES=$(wildcard $(SRCDIR)/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXEC=jl

LDFLAGS=-lncurses

ifdef profile
CXXFLAGS+=-pg
LDFLAGS+=-pg
endif

ifndef nowall
CXXFLAGS+=-Wextra -pedantic -Weffc++ -Wswitch-default -Wswitch-enum
CXXFLAGS+=-Wmissing-include-dirs -Winline -Wfloat-equal -Wundef
CXXFLAGS+=-Wcast-align -Wredundant-decls -Winit-self -Wshadow
endif

ifdef release
CXXFLAGS+=-O3 -s
else
CXXFLAGS+=-g
endif

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CXX) -o $(BINDIR)/$(EXEC) $(LDFLAGS) $^

$(SRCDIR)/argparser.o: $(SRCDIR)/argparser.cpp $(SRCDIR)/argparser.hpp
$(SRCDIR)/buffer.o: $(SRCDIR)/buffer.cpp $(SRCDIR)/buffer.hpp \
  $(SRCDIR)/fileio.hpp
$(SRCDIR)/config.o: $(SRCDIR)/config.cpp $(SRCDIR)/config.hpp \
  $(SRCDIR)/version.hpp
$(SRCDIR)/controller.o: $(SRCDIR)/controller.cpp $(SRCDIR)/controller.hpp \
  $(SRCDIR)/window.hpp $(SRCDIR)/view.hpp $(SRCDIR)/buffer.hpp \
  $(SRCDIR)/fileio.hpp $(SRCDIR)/cursor.hpp
$(SRCDIR)/cursor.o: $(SRCDIR)/cursor.cpp $(SRCDIR)/cursor.hpp \
  $(SRCDIR)/buffer.hpp $(SRCDIR)/fileio.hpp
$(SRCDIR)/fileio.o: $(SRCDIR)/fileio.cpp $(SRCDIR)/fileio.hpp
$(SRCDIR)/main.o: $(SRCDIR)/main.cpp $(SRCDIR)/argparser.hpp \
  $(SRCDIR)/fileio.hpp $(SRCDIR)/config.hpp $(SRCDIR)/buffer.hpp \
  $(SRCDIR)/view.hpp $(SRCDIR)/window.hpp $(SRCDIR)/cursor.hpp \
  $(SRCDIR)/controller.hpp
$(SRCDIR)/view.o: $(SRCDIR)/view.cpp $(SRCDIR)/view.hpp $(SRCDIR)/buffer.hpp \
  $(SRCDIR)/fileio.hpp $(SRCDIR)/window.hpp $(SRCDIR)/cursor.hpp
$(SRCDIR)/window.o: $(SRCDIR)/window.cpp $(SRCDIR)/window.hpp

$(SRCDIR)/version.hpp::
	./mkversion.sh

clean:
	rm -f $(BINDIR)/$(EXEC) $(OBJECTS)

