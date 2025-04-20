CC=gcc
CXX=g++
CFLAGS=-Wall -Wextra -Iinclude
LDFLAGS=-lncurses

SRC_C=$(wildcard src/*.c)
SRC_CPP=$(wildcard src/*.cpp)
OBJ_C=$(SRC_C:.c=.o)
OBJ_CPP=$(SRC_CPP:.cpp=.o)

all: glance

glance: $(OBJ_C) $(OBJ_CPP)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(LDFLAGS) -c $< -o $@

%.o: $.cpp
	$(CXX) $(LDFLAGS) -c $< -o $@

clean:
	rm -f src/*.o glance
