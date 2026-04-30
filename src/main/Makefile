MAIN=ytf
INCLUDE=-I../include
LIB=../lib/libytf.a -lgpege ../util/src/lib/libutil.a
CFLAGS=-g -O2 -Wall -Wextra

all: $(MAIN)

$(MAIN): ytf.c
	$(CC) $(CFLAGS) -o $(MAIN) $(INCLUDE) ytf.c $(LIB)

clean:
	rm -f $(MAIN)
