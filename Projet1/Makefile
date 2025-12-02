main.o : projet.c projet.h
	gcc -c main.c

projet.o : projet.c projet.h
	gcc -c projet.c

main : projet.o main.o
	gcc main.o projet.o -o main


#main.o : main.c projet.h
#	gcc -c main.c

#projet.o : projet.c projet.h
#	gcc -c projet.c

#projet : projet.o main.o
#	gcc main.o projet.o -o projet