#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "cubeOpers.h"
#include "geometricOpers.h"

#define clear() printf("\033[H\033[J")

static const int faceIndices[6][4] = {
    {0, 1, 2, 3},  // Front
    {7, 6, 5, 4},  // Back
    {4, 5, 1, 0},  // Top
    {3, 2, 6, 7},  // Bottom
    {4, 0, 3, 7},  // Left
    {1, 5, 6, 2},  // Right
};

void visibleFaces(double cube[8][3], bool visibleFacesMask[6]);

double toRadians(double angleinDegrees);

double triangleArea(double a[2], double b[2], double c[2]);

double quadrilateralArea(double a[2], double b[2], double c[2], double d[2]);

double faceArea(double face[4][2]);
/*
        e--------f
       /|       /|
      / |      / |
     a--------b  |
     |  |     |  |
     |  h-----|--g
     | /      | /
     |/       |/
     d--------c

    KEEP THIS WINDING ORDER CONSISTENT OR ELSE THINGS GO BAD
    Face order follows left hand rule.
*/
int main(){
    // Cube is initialised at simpler coordinates. Then it is translated and scaled to have a
    // centre at the origin and all side lengths equal to r
    double cube[8][3] = {
        {0, 1, 0},
        {1, 1, 0},
        {1, 0, 0},
        {0, 0, 0},
        {0, 1, 1},
        {1, 1, 1},
        {1, 0, 1},
        {0, 0, 1},
    };
    double coord[2], projectedFaces[6][4][2], projectedCube[8][2], faceAreas[6];
    double alpha, beta, gamma, r, x, y, z1, deltaZ, areaSum, targetFrametime, frametime;
    double xMin, yMin, xMax, yMax, xUnit, yUnit;
    int screenWidth, screenHeight, fps, i, j, k, f, v;
    bool emptySpace, visible[6], stop = false;
    struct timespec frameStart, frameEnd;

    // Characters for each face, in same order as faceIndices list
    char faceChars[6] = {'#', 'H', ';', '!', '+', '='};
    // Character for an empty space outside of cube
    char emptyChar = ' ';
    
    alpha = toRadians(1);
    beta = toRadians(1);
    gamma = toRadians(0.2);

    xMin = -1;
    xMax = 1;
    yMin = -1;
    yMax = 1;
    z1 = 1;
    deltaZ = 3;
    r = 2;
    screenWidth = 80;
    screenHeight = 40;
    fps = 24;

    xUnit = (xMax - xMin) / screenWidth;
    yUnit = (yMax - yMin) / screenHeight;
    targetFrametime = 1./fps;

    char screen[(screenWidth + 1) * screenHeight + 1]; 
    screen[(screenWidth + 1) * screenHeight] = '\0';
    translate(cube, -0.5, -0.5, -0.5);
    scale(cube, r, r, r);
 
    clock_gettime(CLOCK_MONOTONIC, &frameStart);
    clear();
    while (!stop){
        rotate(cube, alpha, beta, gamma);
        translate(cube, 0, 0, deltaZ);
       
        projectInZ(cube, z1, projectedCube);
        visibleFaces(cube, visible);

        translate(cube, 0, 0, -deltaZ);

        for (f = 0; f < 6; f++){
            for (v = 0; v < 4; v++){
                projectedFaces[f][v][0] = projectedCube[faceIndices[f][v]][0];
                projectedFaces[f][v][1] = projectedCube[faceIndices[f][v]][1];
            }
            faceAreas[f] = visible[f] ? faceArea(projectedFaces[f]) : 0;
        }
        for (int k=0; k<(screenWidth + 1)*screenHeight; k++){
            i = k % (screenWidth + 1);
            if (i == screenWidth){
                screen[k] = '\n';
            } else {
                j = k / (screenWidth + 1);

                coord[0] = xMin + i * xUnit;
                coord[1] = yMin + j * yUnit;
                emptySpace = true;
                for (f=0; f<6; f++){
                    if (visible[f] == false)
                        continue;
                   areaSum = 0;
                   areaSum += triangleArea(projectedFaces[f][0], projectedFaces[f][1], coord);
                   areaSum += triangleArea(projectedFaces[f][1], projectedFaces[f][2], coord);
                   areaSum += triangleArea(projectedFaces[f][2], projectedFaces[f][3], coord);
                    areaSum += triangleArea(projectedFaces[f][3], projectedFaces[f][0], coord);
    
                    if (areaSum < faceAreas[f] + 1e-6){
                        screen[k] = faceChars[f];
                        emptySpace = false;
                        break;
                    }
                }
                // If the point does not fall inside any visible faces it is given
                // the empty character
                if (emptySpace == true){
                    screen[k] = emptyChar;
                }
            }
        }
        // Gets the current time in seconds since boot.
        // if the difference in time between the start of the frame and the end is greater than 
        // the goal frame time then sleep for the difference before printing
 
        printf("\033[0;0H");
        clock_gettime(CLOCK_MONOTONIC, &frameEnd);
        frametime = (frameEnd.tv_sec - frameStart.tv_sec) + (frameEnd.tv_nsec - frameStart.tv_nsec)/1e9;
        if (frametime < targetFrametime){
            usleep((targetFrametime - frametime) * 1e6); // Works in microseconds
        }
        // Clearing then printing the screen to console
        printf("%s", screen);
        clock_gettime(CLOCK_MONOTONIC, &frameStart);
    }
    return 0;
}
void visibleFaces(double cube[8][3], bool visibleFacesMask[6]){
    /*
    This method works out which faces of the cube are visible from the origin.
    It modifies the visibleFacesMask list so if a face is visible its corresponding index is set
    to true.
    By using the faceIndices constant we ensure this is handling consistently to prevent issues
    */
    double v1[3], v2[3], cp[3], n[3], ce[3];

    for (int i = 0; i < 6; i++){
        // These are the vectors of corners of this face
        double *a = cube[faceIndices[i][0]];
        double *b = cube[faceIndices[i][1]];
        double *c = cube[faceIndices[i][2]];
        double *d = cube[faceIndices[i][3]];

        // We find v1 and v2, two vectors that follow this plane, compute the cross product, cp, 
        // and normalise it to give the vector n.
        sub(b, a, v1);
        sub(c, a, v2);
        cross(v1, v2, cp);
        normalise(cp, n);

        // Finding the centroid of the face
        ce[0] = (a[0] + b[0] + c[0] + d[0]) / 4;
        ce[1] = (a[1] + b[1] + c[1] + d[1]) / 4;
        ce[2] = (a[2] + b[2] + c[2] + d[2]) / 4;

        // If the dot product of n and ce is positive then this face is visible, this effectively
        // checks if the vector n from the centroid points towards the origin
        visibleFacesMask[i] = (dot(n, ce) < 0);
    }
}
double toRadians(double angleInDegrees){
    return angleInDegrees * M_PI / 180;
}
double triangleArea(double a[2], double b[2], double c[2]){
    double A = a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) + c[0] * (a[1] - b[1]);
    return fabs(A) / 2;
}
double faceArea(double face[4][2]){
    return quadrilateralArea(face[0], face[1], face[2], face[3]);
}
double quadrilateralArea(double a[2], double b[2], double c[2], double d[2]){
    return triangleArea(a, b, c) + triangleArea(c, d, a);
}
