CXXFLAGS = -Wall -std=c++11

nqueens: nqueens.o
	g++ nqueens.o -g $(CXXFLAGS) -o nqueens

nqueens.o: nqueens.cpp
	g++ -c nqueens.cpp

clean:
	rm -f *.o nqueens
