all:
	gcc rtmask.c -o rtmask
	chmod u+s rtmask

install:
	cp rtmask /usr/local/bin/rtmask
