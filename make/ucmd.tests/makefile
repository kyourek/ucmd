CC=gcc
OUTDIR=../out/
PROJECTDIR=../../ucmd.tests/
CMDLINEDIR=../../ucmd/
CFLAGS=-I$(PROJECTDIR)include -I$(PROJECTDIR)include_p -I$(CMDLINEDIR)include -I$(CMDLINEDIR)include_p -Wall -c
LDFLAGS=-L$(OUTDIR) -lucmd
SOURCES=$(wildcard $(PROJECTDIR)source/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ucmd.tests
EXECUTABLEPATH=$(OUTDIR)$(EXECUTABLE)

.PHONY: all
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(MAKE) -C ../ucmd
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLEPATH)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLEPATH)

