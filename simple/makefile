CPP = g++ -m64
CPPFLAGS=-std=c++11 -fopenmp -Iobjs/ -Wall -Wextra -pedantic -g
ISPC=ispc
ISPCFLAGS=--arch=x86-64 --target=sse4
ISPCASM = --emit-asm

default: simple

.PHONY: dirs clean
.PRECIOUS: objs/simple.h

dirs:
	mkdir -p objs/
	mkdir -p bin/

clean:
	rm -rf objs *~ simple serial

simple: dirs  objs/simple.o objs/simple_ispc.o
	$(CPP) $(CPPFLAGS) -O0 -o $@ objs/simple.o objs/simple_ispc.o

objs/simple.o: simple.cpp objs/simple_ispc.h 
	$(CPP) $(CPPFLAGS) -O0 -c -o $@ $<

objs/%_ispc.h objs/%_ispc.o: %.ispc
	$(ISPC) $(ISPCFLAGS) $< -o objs/$*_ispc.o -h objs/$*_ispc.h

serial: simple_serial.cpp
	$(CPP) $(CPPFLAGS) $< -o $@ -O3