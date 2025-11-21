MAIN=ytf
INCLUDE=-I../include
LIB=../lib/libytf.a ../util/src/lib/libutil.a -lgpege

all: $(MAIN)

$(MAIN): ytf.c
	$(CC) -o $(MAIN) $(INCLUDE) ytf.c $(LIB)

clean:
	rm -f $(MAIN)
