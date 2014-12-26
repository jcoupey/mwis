objects = main.o\
	undirectedgraph.o

mwis : $(objects)
	g++ -std=c++11 -o ../bin/mwis $(objects)

undirectedgraph.o : undirectedgraph.cpp undirectedgraph.h
	g++ -std=c++11 -c undirectedgraph.cpp

main.o : main.cpp undirectedgraph.h
	g++ -std=c++11 -c main.cpp

clean:
	rm ../bin/mwis $(objects)
