ARG = -lglut -lGL -lm -g -Wall -DDEBUG_PRINT # -Wl,-rpath '-Wl,$$ORIGIN/AntTweakBar/lib'

a.out: main.o graph.o polygon.o line.o userInput.o
	g++ $(ARG) main.o graph.o polygon.o line.o userInput.o #AntTweakBar/lib/libAntTweakBar.so

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
