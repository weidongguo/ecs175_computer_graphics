ARG = -lglut -lGL -lm -g -Wall #-DDEBUG_PRINT 

a.out: main.o graph.o polygon.o line.o user_io.o
	g++ $(ARG) main.o graph.o polygon.o line.o user_io.o

main.o: main.cpp graph.h	
	g++ $(ARG) -c main.cpp 

graph.o: graph.h
	g++ $(ARG) -c graph.cpp 
polygon.o: polygon.h graph.h
	g++ $(ARG) -c polygon.cpp

line.o: line.h graph.h
	g++ $(ARG) -c line.cpp

user_io: user_io.h
	g++ $(ARG) -c user_io.cpp

clean:
	rm -f *.out *.o
