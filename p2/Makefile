ARG = -lglut -lGL -lm -g -Wall -std=c++11 #-DDEBUG_PRINT 

C = g++

a.out: main.o graph.o user_io.o polyhedron.o
	$(C) main.o graph.o  user_io.o polyhedron.o $(ARG)

main.o: main.cpp graph.h	
	$(C) -c main.cpp  $(ARG)

graph.o: graph.h
	$(C) -c graph.cpp $(ARG)

user_io: user_io.h
	$(C) -c user_io.cpp $(ARG)

polyhedron.o: polyhedron.h graph.h
	$(C) -c polyhedron.cpp $(ARG)

clean:
	rm -f *.out *.o
