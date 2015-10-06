ARG = -lglut -lGL -lm -g -Wall

a.out: main.o graph.o
	g++ $(ARG) main.o graph.o

main.o: main.cpp graph.h	
	g++ $(ARG) -c main.cpp 

graph.o: graph.h
	g++ $(ARG) -c graph.cpp 

clean:
	rm -f *.out *.o
