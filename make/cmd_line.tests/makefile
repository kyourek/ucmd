CC=gcc
OUTDIR=../out/
PROJECTDIR=../../cmd_line.tests/
CMDLINEDIR=../../cmd_line/
CFLAGS=-I$(PROJECTDIR)include -I$(PROJECTDIR)include_p -I$(CMDLINEDIR)include -I$(CMDLINEDIR)include_p -Wall -c
LDFLAGS=-L$(OUTDIR) -lcmd_line
SOURCES=$(wildcard $(PROJECTDIR)source/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=cmd_line.tests
EXECUTABLEPATH=$(OUTDIR)$(EXECUTABLE)

.PHONY: all
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(MAKE) -C $(CMDLINEDIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLEPATH)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLEPATH)

