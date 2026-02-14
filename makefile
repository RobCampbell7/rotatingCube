MAIN = main

bin/$(MAIN): build/$(MAIN).o src/*.c
	cc build/$(MAIN).o -o bin/$(MAIN) -lm

build/$(MAIN).o: src/$(MAIN).c build/cubeOpers.o build/geometricOpers.o
	cc -c src/$(MAIN).c -o build/$(MAIN).o

build/cubeOpers.o: src/cubeOpers.c
	cc -c src/cubeOpers.c -o build/cubeOpers.o

build/geometricOpers.o: src/geometricOpers.c
	cc -c src/geometricOpers.c -o build/geometricOpers.o

clean:
	rm -f bin/* build/*
