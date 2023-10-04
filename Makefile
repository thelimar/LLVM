.PHONY: life clean

life: life.o
	g++ life.o -o life -lsfml-graphics -lsfml-window -lsfml-system

life.o: life.cpp
	g++ -c life.cpp

clean:
	rm -f *.o