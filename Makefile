SRCDIR=src
LIBDIR=lib
OBJDIR=obj
BINDIR=bin

EXEC=jail

LIB_OBJECTS=$(OBJDIR)/datamap.o $(OBJDIR)/util.o

OBJECTS=$(LIB_OBJECTS)
OBJECTS+=$(OBJDIR)/argparser.o $(OBJDIR)/fileio.o $(OBJDIR)/config.o
OBJECTS+=$(OBJDIR)/buffer.o $(OBJDIR)/view.o $(OBJDIR)/controller.o
OBJECTS+=$(OBJDIR)/window.o $(OBJDIR)/cursor.o

CXXFLAGS=-std=c++0x -I$(LIBDIR)
LDFLAGS=-lncurses


TOOLS=$(BINDIR)/conf_compile $(BINDIR)/conf_dump
TOOLS_OBJECTS=$(LIB_OBJECTS) $(OBJDIR)/config.o
TOOLS_SRCDIR=tools

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

all: dir top $(BINDIR)/$(EXEC)
dir:
	mkdir -p $(OBJDIR) $(BINDIR)
top: $(SRCDIR)/version.hpp $(LIBDIR)/default_config.cpp

# main $(EXEC) binary
$(BINDIR)/$(EXEC): $(OBJDIR)/$(EXEC).o $(OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $^

# tools make targets
$(BINDIR)/conf_compile: $(OBJDIR)/conf_compile.o $(TOOLS_OBJECTS)
	$(CXX) -o $@ $^
$(BINDIR)/conf_dump: $(OBJDIR)/conf_dump.o $(TOOLS_OBJECTS)
	$(CXX) -o $@ $^

# version file target
$(SRCDIR)/version.hpp:
	./mkversion.sh

# default_config file target
$(LIBDIR)/default_config.cpp: jailrc.def
	./mkconfig.sh
$(SRCDIR)/default_keymap.cpp: keymap.def
	$(BINDIR)/conf_compile $< $@


# TODO: proper targets to get correct depenencies?

# targets to create object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
$(OBJDIR)/%.o: $(LIBDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
$(OBJDIR)/%.o: $(TOOLS_SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) -I$(SRCDIR)

clean:
	rm -f $(BINDIR)/$(EXEC) $(TOOLS) $(OBJDIR)/*.o
	rm -f $(LIBDIR)/default_config.cpp $(SRCDIR)/version.hpp

