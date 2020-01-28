##############################################
# Makefile for CST320 labs
#
# Author: Philip Howard
# phil.howard@oit.edu
#

COPTS=-Wall -g -c -O0 -std=c++11
OBJS=main.o \
	 pascallex.o \
	 cSymbolTable.o \

all: pascal

clean:
	rm -f $(OBJS)
	rm -f pascallex.c
	rm -f pascal
	rm -f out
	rm -f *.xml

.c.o:
	g++ $(COPTS) $? -o $@

.cpp.o:
	g++ $(COPTS) $? -o $@

main.o: main.cpp pascallex.c
	g++ $(COPTS) main.cpp -o main.o

cSymbolTable.o: cSymbolTable.cpp
	g++ $(COPTS) cSymbolTable.cpp -o cSymbolTable.o

pascallex.c: pascal.l
	flex -o pascallex.c pascal.l

pascallex.o: pascallex.c
	g++ $(COPTS) -Wno-sign-compare $? -o $@
pascal: $(OBJS)
	g++ $(OBJS) -o pascal

