CC=gcc
OUTDIR=../out/
UCMDDIR=../../ucmd/
PROJECTDIR=../../ucmd.tests/
CFLAGS=-I$(PROJECTDIR)include -I$(PROJECTDIR)include_p -I$(UCMDDIR)include -I$(UCMDDIR)include_p -I$(UCMDDIR)config -I$(UCMDDIR)config/console -I$(UCMDDIR)config/console/default -Wall -c
LDFLAGS=-L$(OUTDIR) -lucmd
SOURCES=$(wildcard $(PROJECTDIR)source/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ucmd.tests
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

