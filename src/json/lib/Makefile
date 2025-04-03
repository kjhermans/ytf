CFLAGS=-g -O2 -Wall -Wextra -I../include $(DEBUG)
LIB=libjson.a

all: precompile compile

precompile: json_bytecode.h

json_bytecode.h: json.byc
	xxd -i json.byc json_bytecode.h

json.byc: json.asm
	gpega -i json.asm -o json.byc

json.asm: json.gpeg
	gpegc -i json.gpeg -o json.asm -M json_slotmap.h

compile: $(LIB)

$(LIB): json.o
	ar -rcs $(LIB) json.o

clean:
	rm -f json.o $(LIB)

superclean: clean
	rm -f json.asm json.byc json_bytecode.h json_slotmap.h
