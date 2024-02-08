.DEF: ui mbr scanner program clean

settings =  -std=c99

mbr: mbr.c
	gcc -c mbr.c -o mbr.o $(settings)

scanner: scanner.c
	gcc -c scanner.c -o scanner.o $(settings)

ui: ui.c
	gcc -c ui.c -o ui.o $(settings)
	
program: ui.o
	gcc ui.o mbr.o scanner.o -o scan.exe $(settings)

clean:
	-rm -f program.o
	-rm -f program