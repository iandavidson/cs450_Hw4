

p1.x: p1.cpp
	g++ -std=c++11 p1.cpp -D PROB1 -o  p1.x

p2.x: p2.cpp
	g++ -std=c++11 p1.cpp -o p2.x

clean:
	rm *~ *.x
