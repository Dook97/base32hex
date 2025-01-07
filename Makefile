.PHONY: all clean

SOURCES ::= $(wildcard src/*.c)
HEADERS ::= $(wildcard include/*.h)
CFLAGS  ::= -I./include -g -O2 -std=c99 -Wall -Wextra -Wpedantic

all: base32hex

base32hex: $(SOURCES) $(HEADERS)
	cc $(CFLAGS) $(SOURCES) -o base32hex

clean:
	rm base32hex
