CFILES:=$(shell ls *.c | sort)
OBJECTS:=$(shell ls *.c | sort | sed -e 's/\.c$$/.o/')
CC=$(ARCH)gcc
AR=$(ARCH)ar
CFLAGS=-g -O2 -Wall -Wextra -I../include -I../json/include $(DEBUG)
TARGET=libytf.a

all: fnchdr $(TARGET)

preproc: fnchdr flatparser jsonparser huffmandict

fnchdr: ../include/ytf/ytf_functions.h

flatparser: flat_bytecode.h

flat_bytecode.h: flat.byc
	xxd -i flat.byc flat_bytecode.h

flat.byc: flat.asm
	gpega -i flat.asm -o flat.byc

flat.asm: flat.gpeg
	gpegc -C \
	  -i flat.gpeg \
	  -o flat.asm \
	  -M flat_slotmap.h \
	  -G flat_handler.c flat

jsonparser: json_bytecode.h

json_bytecode.h: json.byc
	xxd -i json.byc json_bytecode.h

json.byc: json.asm
	gpega -i json.asm -o json.byc

json.asm: json.gpeg
	gpegc -C \
	  -i json.gpeg \
	  -o json.asm \
	  -M json_slotmap.h \
	  -G json_handler.c json

huffmandict: ../include/ytf/ytf_dictionary.h

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

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f json.asm json.byc json_slotmap.h json_handler.c
	rm -f flat.asm flat.byc flat_slotmap.h flat_handler.c
