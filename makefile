bin/rotatingCube: build/rotatingCube.o src/*.c
	cc build/rotatingCube.o -o bin/rotatingCube -lm

build/rotatingCube.o: src/rotatingCube.c build/cubeOpers.o
	cc -c src/rotatingCube.c -o build/rotatingCube.o

build/cubeOpers.o: src/cubeOpers.c
	cc -c src/cubeOpers.c -o build/cubeOpers.o

clean:
	rm -f bin/* build/*
