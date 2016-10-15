CC=g++
FLAGS=-std=c++11

all: ./build/genreg ./build/binconv ./build/QMcC

./build/binconv: ./binconv/binconv.cpp
	$(CC) $(FLAGS) -o ./build/binconv ./binconv/binconv.cpp

./build/genreg: ./genreg/genreg.cpp
	$(CC) $(FLAGS) -o ./build/genreg ./genreg/genreg.cpp

./build/QMcC: ./QMcC/QMcC.cpp
	$(CC) $(FLAGS) -o ./build/QMcC ./QMcC/QMcC.cpp

debug: ./debug/genreg ./debug/binconv ./debug/QMcC

./debug/binconv: ./binconv/binconv.cpp
	$(CC) $(FLAGS) -ggdb -o ./debug/binconv ./binconv/binconv.cpp

./debug/genreg: ./genreg/genreg.cpp
	$(CC) $(FLAGS) -ggdb -o ./debug/genreg ./genreg/genreg.cpp

./debug/QMcC: ./QMcC/QMcC.cpp
	$(CC) $(FLAGS) -ggdb -o ./debug/QMcC ./QMcC/QMcC.cpp

clean: ./build/genreg ./build/binconv ./build/QMcC
	rm ./build/genreg ./build/binconv ./build/QMcC

.PHONY:all, clean, debug
