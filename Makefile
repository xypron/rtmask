export DESTDIR ?= /usr/local

all:
	gcc rtmask.c -o rtmask
	chmod u+s rtmask

install:
	mkdir -p $(DESTDIR)/bin/
	cp rtmask $(DESTDIR)/bin/rtmask
