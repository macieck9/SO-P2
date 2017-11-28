main: main.cpp
	g++ main.cpp -o main -Wall -Werror -lpthread

run: main.cpp
	./main 10 11 3 7
