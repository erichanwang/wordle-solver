all: wordle_solver

wordle_solver: main.cpp
	g++ -std=c++17 main.cpp -o wordle_solver

clean:
	rm -f wordle_solver
