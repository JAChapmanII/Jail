SRCDIR=src
BINDIR=bin
OBJDIR=obj
#SOURCES=$(wildcard $(SRCDIR)/*.cpp)
#OBJECTS=$(SOURCES:.cpp=.o)
EXEC=jail

OBJECTS=
OBJECTS+=$(OBJDIR)/argparser.o $(OBJDIR)/fileio.o $(OBJDIR)/config.o
OBJECTS+=$(OBJDIR)/buffer.o $(OBJDIR)/view.o $(OBJDIR)/controller.o
OBJECTS+=$(OBJDIR)/window.o $(OBJDIR)/cursor.o

CXXFLAGS=-std=c++0x
LDFLAGS=-lncurses

ifdef profile
CXXFLAGS+=-pg
LDFLAGS+=-pg
endif

ifndef nowall
CXXFLAGS+=-Wall -Wextra -pedantic -Weffc++ -Wswitch-default -Wswitch-enum
CXXFLAGS+=-Wmissing-include-dirs -Winline -Wfloat-equal -Wundef
CXXFLAGS+=-Wcast-align -Wredundant-decls -Winit-self -Wshadow
endif

ifdef release
CXXFLAGS+=-O3 -s
else
CXXFLAGS+=-g
endif

all: dir $(BINDIR)/$(EXEC)
dir:
	mkdir -p $(OBJDIR) $(BINDIR)

$(BINDIR)/$(EXEC): $(OBJDIR)/$(EXEC).o $(OBJECTS)
	$(CXX) -o $(BINDIR)/$(EXEC) $(LDFLAGS) $^

$(OBJDIR)/argparser.o: $(SRCDIR)/argparser.cpp $(SRCDIR)/argparser.hpp
$(OBJDIR)/buffer.o: $(SRCDIR)/buffer.cpp $(SRCDIR)/buffer.hpp \
  $(SRCDIR)/fileio.hpp
$(OBJDIR)/config.o: $(SRCDIR)/config.cpp $(SRCDIR)/config.hpp \
  $(SRCDIR)/version.hpp
$(OBJDIR)/controller.o: $(SRCDIR)/controller.cpp $(SRCDIR)/controller.hpp \
  $(SRCDIR)/window.hpp $(SRCDIR)/view.hpp $(SRCDIR)/buffer.hpp \
  $(SRCDIR)/fileio.hpp $(SRCDIR)/cursor.hpp
$(OBJDIR)/cursor.o: $(SRCDIR)/cursor.cpp $(SRCDIR)/cursor.hpp \
  $(SRCDIR)/buffer.hpp $(SRCDIR)/fileio.hpp
$(OBJDIR)/fileio.o: $(SRCDIR)/fileio.cpp $(SRCDIR)/fileio.hpp
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(SRCDIR)/argparser.hpp \
  $(SRCDIR)/fileio.hpp $(SRCDIR)/config.hpp $(SRCDIR)/buffer.hpp \
  $(SRCDIR)/view.hpp $(SRCDIR)/window.hpp $(SRCDIR)/cursor.hpp \
  $(SRCDIR)/controller.hpp
$(OBJDIR)/view.o: $(SRCDIR)/view.cpp $(SRCDIR)/view.hpp $(SRCDIR)/buffer.hpp \
  $(SRCDIR)/fileio.hpp $(SRCDIR)/window.hpp $(SRCDIR)/cursor.hpp
$(OBJDIR)/window.o: $(SRCDIR)/window.cpp $(SRCDIR)/window.hpp

# TODO: will the above rules mesh with the following?

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJDIR)/version.hpp::
	./mkversion.sh

clean:
	rm -f $(BINDIR)/$(EXEC) $(OBJDIR)/*.o

