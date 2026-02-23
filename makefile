MAIN = main
CC = cc
CFLAGS = -Iinclude

bin/$(MAIN): build/$(MAIN).o src/*.c
	$(CC) build/$(MAIN).o build/cubeOpers.o build/geometricOpers.o -o bin/$(MAIN) -lm 

build/$(MAIN).o: src/$(MAIN).c build/cubeOpers.o build/geometricOpers.o
	$(CC) $(CFLAGS) -c src/$(MAIN).c -o build/$(MAIN).o

build/cubeOpers.o: src/cubeOpers.c
	$(CC) $(CFLAGS) -c src/cubeOpers.c -o build/cubeOpers.o

build/geometricOpers.o: src/geometricOpers.c
	$(CC) $(CFLAGS) -c src/geometricOpers.c -o build/geometricOpers.o

clean:
	rm -f bin/* build/*
