# Embroidermodder 2 Makefile

ifeq ($(OS),Windows_NT)
CFLAGS := -O2 -g -Wall -municode
LIBS := -lGdi32
else
CFLAGS := -O2 -g -Wall
LIBS := -lX11 -lm    
endif

CC := gcc
INCLUDE := -Isrc/libembroidery/src -Isrc -Isrc/stb

SRC := $(wildcard src/libembroidery/src/*.c src/*.c)
OBJ := $(patsubst %.c,%.o,$(SRC))

all: $(OBJ)
	$(CC) $(OBJ) -o embroidermodder $(LIBS)

%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

ios:
	echo "Assumes that we're building on a Mac OS system."

clean:
	rm -f $(OBJ)

test:
	timeout 10 ./embroidermodder --test &> test_result.txt

