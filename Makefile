CC = gcc
FILES = server.cpp
OUT_EXE = server
FLAGS = -std=c++11

build: $(FILES)
	$(CC) $(FLAGS) -o $(OUT_EXE) $(FILES)

clean:
	rm -f *.o $(OUT_EXE)

rebuild: clean build
