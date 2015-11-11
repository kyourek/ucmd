CC=gcc
OUTDIR=../out/
PROJECTDIR=../../ucmd/
CFLAGS=-Wall -c
SOURCES=$(wildcard $(PROJECTDIR)*.c)
OBJECTS=$(SOURCES:.c=.o)
LIBRARY=libucmd.a
LIBRARYPATH=$(OUTDIR)$(LIBRARY)

.PHONY: all
all: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rcs $(LIBRARYPATH) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(LIBRARYPATH)
