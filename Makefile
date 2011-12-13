# The structure and format of this Makefile was inspired by:
# http://scottmcpeak.com/autodepend/autodepend.html
# http://make.paulandlesley.org/autodep.html

# The following vars are specific to this program:

PROGRAM = Main

SRCS = Index.cpp MultiTrie.cpp Test.cpp

# Compiler flags:

CXX = g++
CXXFLAGS = -g -Wall -Werror

# The rest is boiler plate that sould be the same for most programs:

SRCDIR = src
INCLUDEDIR = include
BINDIR = bin

DEPDIR = .deps
df = $(DEPDIR)/$(*F)

OBJS = $(SRCS:%.cpp=$(BINDIR)/%.o)

all : $(BINDIR)/$(PROGRAM)

$(BINDIR) :
	mkdir -p $(BINDIR)

$(DEPDIR) :
	mkdir -p $(DEPDIR)

$(BINDIR)/$(PROGRAM) : $(BINDIR) $(DEPDIR) $(OBJS)
	$(CXX) $(OBJS) -o $(BINDIR)/$(PROGRAM)

$(BINDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) -MMD -MF $(df).d -I$(INCLUDEDIR) $(CXXFLAGS) -c -o $@ $<
	@cp $(df).d $(df).P
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
                -e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P
	@rm -f $(df).d

-include $(SRCS:%.cpp=$(DEPDIR)/%.P)

clean :
	rm -f $(BINDIR)/*
	mkdir -p $(DEPDIR)
	rm -f $(DEPDIR)/*
	rmdir $(DEPDIR)
