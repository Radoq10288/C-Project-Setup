BINDIR=bin
OBJDIR=obj
SRCDIR=src
TESTDIR=tests

OS=$(shell uname -o)
ifeq ($(OS), Msys)
BINFILE=cpps.exe
endif
ifeq ($(OS), GNU/Linux)
BINFILE=cpps
endif

BIN=$(BINDIR)/$(BINFILE)
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
	rm $(TESTDIR)/bin/* $(TESTDIR)/*.o $(TESTDIR)/*.c $(TESTDIR)/Makefile
	rm $(TESTDIR)/testproject/src/* $(TESTDIR)/testproject/Makefile
	rmdir $(TESTDIR)/testproject/bin $(TESTDIR)/testproject/src
	rmdir $(TESTDIR)/testproject

distclean: clean
	rmdir $(OBJDIR) $(BINDIR)
	rm CPPS-0.1.1-alpha.3-release.tar
	rm CPPS-0.1.1-alpha.3-debug.tar
	rm CPPS-0.1.1-alpha.3-source.tar


tar-source:
	tar -cvf CPPS-0.1.1-alpha.3-source.tar include/sput-1.4.0/* src/*.c src/*.h .gitignore COPYING Makefile README.md

tar-release:
	tar -cvf CPPS-0.1.1-alpha.3-release.tar bin/* COPYING README.md

tar-debug:
	tar -cvf CPPS-0.1.1-alpha.3-debug.tar bin/* COPYING README.md


install:
	install -d ~/local/bin
	install -d ~/local/share/cpps
	install bin/$(BINFILE) ~/local/bin
	install COPYING ~/local/share/cpps
	install README.md ~/local/share/cpps

uninstall:
	rm ~/local/bin/$(BINFILE)
	rm ~/local/share/cpps/*
	rmdir ~/local/share/cpps


