BINDIR=bin
OBJDIR=obj
SRCDIR=src

OS=$(shell uname -o)
ifeq ($(OS), Msys)
BINFILE=cps.exe
endif
ifeq ($(OS), GNU/Linux)
BINFILE=cps
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

clean:
	rm -f $(OBJ) $(BINDIR)/*

distclean: clean
	rmdir $(OBJDIR)
	rm CPS-0.1.1-alpha.3-release.tar
	rm CPS-0.1.1-alpha.3-debug.tar
	rm CPS-0.1.1-alpha.3-source.tar


build-test:
	cd tests && $(MAKE)

run-test:
ifeq ($(shell if test -d tests/bin/testproject; then echo "exist"; fi), exist)
	@rm -f tests/bin/testproject/src/*
	@rm -f tests/bin/testproject/Makefile
	@rmdir tests/bin/newdir
	@rmdir tests/bin/testproject/bin
	@rmdir tests/bin/testproject/src
	@rmdir tests/bin/testproject
endif
	cd tests/bin && ./test

clean-test:
	cd tests && $(MAKE) clean

distclean-test:
	cd tests && $(MAKE) distclean


tar-source:
	tar -cvf CPS-0.1.1-alpha.3-source.tar include/sput-1.4.0/* src/*.c src/*.h .gitignore COPYING Makefile README.md

tar-release:
	tar -cvf CPS-0.1.1-alpha.3-release.tar bin/* COPYING README.md

tar-debug:
	tar -cvf CPS-0.1.1-alpha.3-debug.tar bin/* COPYING README.md


install:
	install -d ~/local/bin
	install -d ~/local/share/cps
	install bin/$(BINFILE) ~/local/bin
	install COPYING ~/local/share/cps
	install README.md ~/local/share/cps

uninstall:
	rm ~/local/bin/$(BINFILE)
	rm ~/local/share/cps/*
	rmdir ~/local/share/cps


