CC=g++

all: main.cpp 
	$(CC) -o timber main.cpp -lsfml-window -lsfml-graphics -lsfml-system

clean:
	rm timber

