MAIN=ytf
INCLUDE=-I../include
LIB=../lib/libytf.a ../util/src/lib/libutil.a -lgpege
CFLAGS=-g -O2 -Wall -Wextra

all: $(MAIN)

$(MAIN): ytf.c
	$(CC) $(CFLAGS) -o $(MAIN) $(INCLUDE) ytf.c $(LIB)

clean:
	rm -f $(MAIN)
