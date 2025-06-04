CC=gcc
CXX=g++
CFLAGS=-Wall -Wextra -Iinclude
LDFLAGS=-lncurses

SRC_C=$(shell find src -name '*.c')
SRC_CPP=$(shell find src -name '*.cpp')
OBJ_C=$(SRC_C:.c=.o)
OBJ_CPP=$(SRC_CPP:.cpp=.o)

all: synd3

synd3: $(OBJ_C) $(OBJ_CPP)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	find src -name '*.o' -delete
	rm -f synd3
