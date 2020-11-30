export DESTDIR ?= /usr/local

all:
	gcc rtmask.c -o rtmask

install:
	mkdir -p $(DESTDIR)/bin/
	cp rtmask $(DESTDIR)/bin/rtmask
	chmod u+s rtmask $(DESTDIR)/bin/rtmask
