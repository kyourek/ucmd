CC=gcc
CFLAGS=-I./include -I./include_p -Wall -c
SOURCES=$(wildcard ./source/*.c)
OBJECTS=$(SOURCES:.c=.o)
LIBRARY=libcmd_line.a
LIBRARYPATH=../out/$(LIBRARY)

.PHONY: all
all: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rcs $(LIBRARYPATH) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(LIBRARYPATH)

