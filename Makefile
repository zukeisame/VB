libVB.so: VB.o
	gcc VB.o -shared -o libVB.so

VB.o: VB.h VB.c
	gcc VB.c -c -fPIC -o VB.o
