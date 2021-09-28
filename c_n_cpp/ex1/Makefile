You can do that by compiling first the C files and then straight after the CPP files. This might work (at least worked in one of my projects):

CXX = g++
CC = gcc
CFLAGS = -Wall 
CXXFLAGS = -Wall -D__STDC_LIMIT_MACROS

OUTPUTDIR = ./bin/
MKDIR = mkdir -p $(OUTPUTDIR)
OBJECTCPP = cpp.o

CSOURCES = \
	main.c \

CXXSOURCES = \
	cpp.cpp 

all: demo

demo: 
	$(MKDIR)
	$(CXX) $(CXXSOURCES) $(CXXFLAGS) -c -o $(OUTPUTDIR)$(OBJECTCPP)
	$(CXX) $(CSOURCES) $(OUTPUTDIR)$(OBJECTCPP) $(CFLAGS) -o $(OUTPUTDIR)demo

.PHONY: clean
clean:
	$(RM) -rf $(OUTPUTDIR)
	$(RM) ./*.gc??
	$(RM) ./*.o