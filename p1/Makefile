ARG = -lglut -lGL -lm -g -Wall #-DDEBUG_PRINT 

a.out: main.o graph.o polygon.o line.o userInput.o
	g++ $(ARG) main.o graph.o polygon.o line.o userInput.o

main.o: main.cpp graph.h	
	g++ $(ARG) -c main.cpp 

graph.o: graph.h
	g++ $(ARG) -c graph.cpp 
polygon.o: polygon.h graph.h
	g++ $(ARG) -c polygon.cpp

line.o: line.h graph.h
	g++ $(ARG) -c line.cpp

userInput.o: userInput.h
	g++ $(ARG) -c userInput.cpp

clean:
	rm -f *.out *.o
