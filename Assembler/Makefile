assembler: assembler.o map.o
	gcc assembler.c map.o -o assembler
assembler.o: assembler.c
	gcc -c assembler.c
map.o: map.o
	gcc -c map.c

Clean:
	rm *.o
