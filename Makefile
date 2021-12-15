allocate: allocate.c util.o process_cpu.o
	gcc -o allocate util.o process_cpu.o allocate.c -lm
util.o: util.c
	gcc -c util.c
process_cpu.o: process_cpu.c
	gcc -c process_cpu.c  
clean:
	del *.o allocate.exe
