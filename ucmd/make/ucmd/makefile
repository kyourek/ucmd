CC=gcc
OUTDIR=../out/
PROJECTDIR=../../ucmd/
CFLAGS=-I$(PROJECTDIR)include -I$(PROJECTDIR)include_p -Wall -c
SOURCES=$(wildcard $(PROJECTDIR)source/*.c)
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
