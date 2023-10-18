.PHONY: life clean

life:
	g++ sim.cpp life.cpp -lSDL2 -o life

clean:
	rm -f *.o