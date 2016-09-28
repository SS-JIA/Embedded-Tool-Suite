CC=g++
FLAGS=-std=c++11

all: genreg

binconv: ./binconv/binconv.cpp
	$(CC) $(FLAGS) -o ./build/binconv ./binconv/binconv.cpp

genreg: ./genreg/genreg.cpp
	$(CC) $(FLAGS) -o ./build/genreg ./genreg/genreg.cpp

.PHONY:all
