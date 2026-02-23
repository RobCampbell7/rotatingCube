---
title: README
...

# Rotating cube in console!

This program renders a rotating cube in the console.

There are a lot of parameters that can effect the visualisation of the cube. The background 
character and characters for each face of the cube can all be changed by editing values in 
`src/main.c`. Specifically:

- Changing the values in the initialisation of `faceChar` changes the characters each face uses
- Changing the value of `emptyChar` changes the background character

For the mathematics that affect the rotation animation there are more variables that can be altered.
- gamma - rotation in x-axis
- beta - rotation in y-axis
- alpha - rotation in z-axis
By default these variables are defined using the `toRadians` function to make the angles more 
human-readable since the angles per update are very small and using $\pi / 180$ instead of 1 degree
just makes it seem more complicated.

## Directory structure

