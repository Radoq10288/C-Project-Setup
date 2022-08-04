BINDIR=bin
OBJDIR=obj
SRCDIR=src
TESTDIR=tests

BIN=$(BINDIR)/cpps
OBJ=$(OBJDIR)/*.o
CFILES=$(SRCDIR)/*.c

CC=gcc
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
	rm -fr $(BINDIR)/* $(OBJDIR)/*
	rm -f CPPS-0.1.1-alpha.2-win32-release.tar
	rm -f CPPS-0.1.1-alpha.2-win32-debug.tar
	rm -f CPPS-0.1.1-alpha.2-win32-source.tar
	rmdir $(OBJDIR)/ $(BINDIR)


tar-source:
	tar -cvf CPPS-0.1.1-alpha.2-win32-source.tar include/sput-1.4.0/* src/*.c src/*.h .gitignore COPYING Makefile README.md

tar-release:
	tar -cvf CPPS-0.1.1-alpha.2-win32-release.tar bin/* COPYING README.md

tar-debug:
	tar -cvf CPPS-0.1.1-alpha.2-win32-debug.tar bin/* COPYING README.md


install:
	install -d ~/local/bin
	install -d ~/local/share/cpps
	install bin/cpps.exe ~/local/bin
	install COPYING ~/local/share/cpps
	install README.md ~/local/share/cpps

uninstall:
	rm -fr ~/local/bin/cpps.exe
	rm -fr ~/local/share/cpps/*
	rmdir ~/local/share/cpps


