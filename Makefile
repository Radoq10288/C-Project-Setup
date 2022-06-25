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
	rm -f $(TESTDIR)/bin/* $(TESTDIR)/*.o $(TESTDIR)/*.c
	rmdir $(TESTDIR)/testproject/bin $(TESTDIR)/testproject/src $(TESTDIR)/testproject
	rm -f $(TESTDIR)/Makefile

distclean: clean
	rmdir $(OBJDIR)/ $(BINDIR)
	rm -f C-Programming-Project-Setup-0.1.0-win-release.tar
	rm -f C-Programming-Project-Setup-0.1.0-win-debug.tar
	rm -f C-Programming-Project-Setup-0.1.0-win-source.tar

archive-source:
	tar -cf C-Programming-Project-Setup-0.1.0-win-source.tar include/sput-1.4.0/* src/*.c .gitignore COPYING install-cpps.cmd Makefile README.md

archive-release:
	tar -cf C-Programming-Project-Setup-0.1.0-win-release.tar bin/* COPYING README.md

archive-debug:
	tar -cf C-Programming-Project-Setup-0.1.0-win-debug.tar bin/* COPYING README.md

install:
	install -d /C/CPPS/bin
	install bin/cpps.exe /C/CPPS/bin
	install COPYING /C/CPPS
	install README.md /C/CPPS

uninstall:
	rm -fr /C/CPPS


