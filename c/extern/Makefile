CC = gcc
CXX = clang++

CC_SOURCE = extern_add.c
CC_OBJ = extern_add.o

CXX_SOURCE = extern.cpp
TARGET = ext

all:
	$(CC) -c $(CC_SOURCE)
	$(CXX) $(CXX_SOURCE) $(CC_OBJ) -std=c++2a -o $(TARGET)

clean:
	rm -f *.o $(TARGET)

