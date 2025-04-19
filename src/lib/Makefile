CFILES:=$(shell ls *.c | sort)
OBJECTS:=$(shell ls *.c | sort | sed -e 's/\.c$$/.o/')
CC=$(ARCH)gcc
AR=$(ARCH)ar
CFLAGS=-g -O2 -Wall -Wextra -I../include -I../json/include $(DEBUG)
TARGET=libytf.a

all: fnchdr $(TARGET)

preproc: fnchdr huffmandict peg

fnchdr: ../include/ytf/ytf_functions.h

huffmandict: ../include/ytf/ytf_dictionary.h

peg: ytf_bytecode.h

ytf_bytecode.h: ytf.byc
	xxd -i ytf.byc ytf_bytecode.h

ytf.byc: ytf.asm
	gpega -i ytf.asm -o ytf.byc

ytf.asm: ytf.gpeg
	gpegc -i ytf.gpeg -o ytf.asm -M ytf_slotmap.h

../include/ytf/ytf_dictionary.h:
	../../bin/huffmandict.pl ../res/*.txt > ../include/ytf/ytf_dictionary.h

../include/ytf/ytf_functions.h: $(CFILES)
	../../bin/genfnchdr -i YTF_FUNCTIONS \
	  > ../include/ytf/ytf_functions.h

$(TARGET): $(OBJECTS)
	$(AR) -rcs $(TARGET) $(OBJECTS)

superclean: clean
	rm -f ../include/ytf/ytf_functions.h
	rm -f ../include/ytf/ytf_dictionary.h
	rm -f ytf_bytecode.h ytf.byc ytf.asm ytf_slotmap.h

clean:
	rm -f $(OBJECTS) $(TARGET)
