BINDIR=bin
OBJDIR=obj
SRCDIR=src
TESTDIR=tests

BIN=$(BINDIR)/cpps
OBJ=$(OBJDIR)/*.o
CFILES=$(SRCDIR)/*.c

CC=mingw32-gcc-9.2.0
CFLAGS=-g -pedantic -Wall

debug: build compile

release: CFLAGS=-O2 -Wall
release: build compile

compile: $(BIN)
$(BIN) : $(OBJ)
	$(CC) -o $(BIN) $(OBJ)

build: $(OBJ)
$(OBJ) : $(CFILES)
	$(CC) -c $(CFILES) $(CFLAGS)

	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@mv *.o $(OBJDIR)/

test: $(TESTDIR)/bin/test
$(TESTDIR)/bin/test: $(TESTDIR)/*.o
	$(CC) -o $(TESTDIR)/bin/test $(TESTDIR)/*.o

$(TESTDIR)/*.o: $(TESTDIR)/src/*.c $(SRCDIR)/functions.c $(SRCDIR)/functions.h
	$(CC) -c $(TESTDIR)/src/*.c $(SRCDIR)/functions.c $(SRCDIR)/functions.h $(CFLAGS)

	@mv *.o $(TESTDIR)/

clean:
	rm -f $(OBJ) $(BINDIR)/*

clean-test:
	rm -f $(TESTDIR)/bin/* $(TESTDIR)/*.o *.c

distclean: clean
	rmdir $(OBJDIR)/ $(BINDIR)
	rm -f C-Programming-Project-Setup-0.1.0-release.tar
	rm -f C-Programming-Project-Setup-0.1.0-debug.tar
	rm -f C-Programming-Project-Setup-0.1.0-source.tar

archive-source:
	tar -cf C-Programming-Project-Setup-0.1.0-source.tar src/*.c .gitignore Makefile README.md

archive-release:
	tar -cf C-Programming-Project-Setup-0.1.0-release.tar bin/* COPYING README.md

archive-debug:
	tar -cf C-Programming-Project-Setup-0.1.0-debug.tar bin/* COPYING README.md



