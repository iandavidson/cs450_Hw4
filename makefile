p1.x: p1.cpp
	g++ p1.cpp -o p1.x

p2.x: p2.cpp
	g++ p2.cpp -o p2.x

clean:
	rm *~ *.x