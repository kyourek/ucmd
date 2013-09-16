CC=gcc
OUTDIR=../out/
CFLAGS=-I./include -I./include_p -I../cmd_line/include -I../cmd_line/include_p -Wall -c
LDFLAGS=-L$(OUTDIR) -lcmd_line
SOURCES=$(wildcard ./source/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=cmd_line.tests
EXECUTABLEPATH=$(OUTDIR)$(EXECUTABLE)

.PHONY: all
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLEPATH)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLEPATH)

