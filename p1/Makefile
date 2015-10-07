ARG = -lglut -lGL -lm -g -Wall -DDEBUG_PRINT

a.out: main.o graph.o polygon.o
	g++ $(ARG) main.o graph.o polygon.o

main.o: main.cpp graph.h	
	g++ $(ARG) -c main.cpp 

graph.o: graph.h
	g++ $(ARG) -c graph.cpp 
polygon.o: polygon.h graph.h
	g++ $(ARG) -c polygon.cpp

clean:
	rm -f *.out *.o
