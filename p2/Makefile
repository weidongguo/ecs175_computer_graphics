ARG = -lglut -lGL -lm -g -Wall -std=c++11 #-DDEBUG_PRINT 

C = g++

a.out: main.o graph.o polygon.o line.o user_io.o polyhedron.o
	$(C) $(ARG) main.o graph.o polygon.o line.o user_io.o polyhedron.o

main.o: main.cpp graph.h	
	$(C) $(ARG) -c main.cpp 

graph.o: graph.h
	$(C) $(ARG) -c graph.cpp 
polygon.o: polygon.h graph.h
	$(C) $(ARG) -c polygon.cpp

line.o: line.h graph.h
	$(C) $(ARG) -c line.cpp

user_io: user_io.h
	$(C) $(ARG) -c user_io.cpp

polyhedron.o: polyhedron.h graph.h
	$(C) $(ARG) -c polyhedron.cpp	

clean:
	rm -f *.out *.o
