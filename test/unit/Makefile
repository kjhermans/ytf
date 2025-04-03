CFILES:=$(shell ls *.c | sort -n)
CC=$(ARCH)gcc
AR=$(ARCH)ar
CFLAGS=-g -O2 -Wall -Wextra -I../../src/include -I../../src/json/include $(DEBUG)
LIB=libytf.a libjson.a -lgpege

all:
	for FILE in $(CFILES); do \
	  $(CC) $(CFLAGS) $$FILE $(LIB) -o test.exe && ./test.exe; \
	done

clean:
	rm -f test.exe
