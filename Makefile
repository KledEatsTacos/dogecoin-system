all: build run

build:
	g++ -o ./bin/Program ./src/main.cpp

run:
	./bin/Program