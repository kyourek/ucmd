CC=gcc
CFLAGS=-I./include -I./include_p -Wall -c
SOURCES=$(wildcard ./source/*.c)
OBJECTS=$(SOURCES:.c=.o)
LIBRARY=../out/libcmd_line.a

all: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rcs $(LIBRARY) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(LIBRARY)

