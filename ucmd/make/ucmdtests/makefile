CC=gcc
OUTDIR=../out/
UCMDDIR=../../ucmd/
PROJECTDIR=../../ucmdtests/
CFLAGS=-I$(PROJECTDIR)include -I$(UCMDDIR)include -I$(UCMDDIR)include_p -Wall -c
LDFLAGS=-L$(OUTDIR) -lucmd
SOURCES=$(wildcard $(PROJECTDIR)source/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ucmdtests
EXECUTABLEPATH=$(OUTDIR)$(EXECUTABLE)

.PHONY: all
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(MAKE) -C ../ucmd
	$(CC) $(OBJECTS) -o $(EXECUTABLEPATH) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLEPATH)
