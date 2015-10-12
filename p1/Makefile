ARG = -lglut -lGL -lm -IAntTweakBar/include -g -Wall -DDEBUG_PRINT -Wl,-rpath '-Wl,$$ORIGIN/AntTweakBar/lib'

a.out: main.o graph.o polygon.o line.o 
	g++ $(ARG) main.o graph.o polygon.o line.o #AntTweakBar/lib/libAntTweakBar.so

main.o: main.cpp graph.h	
	g++ $(ARG) -c main.cpp 

graph.o: graph.h
	g++ $(ARG) -c graph.cpp 
polygon.o: polygon.h graph.h
	g++ $(ARG) -c polygon.cpp

line.o: line.h graph.h
	g++ $(ARG) -c line.cpp

clean:
	rm -f *.out *.o
