#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "cubeOpers.c"
#include "geometricOpers.c"

#define clear() printf("\033[H\033[J")

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
*/
int main(){
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
    double coord[2];
    double projectedCube[8][2];
    double alpha, beta, gamma;
    double r, x, y, z1;
    double xMin, yMin, xMax, yMax, xUnit, yUnit;
    double faceAreas[6];
    double areaSum;
    int screenWidth, screenHeight, fps, i, j, k, f;

    xMin = -1;
    xMax = 1;
    yMin = -2;
    yMax = 2;
    z1 = 1;
    screenWidth = 20;
    screenHeight = 10;
    fps = 24;

    xUnit = (xMax - xMin) / screenWidth;
    yUnit = (yMax - yMin) / screenHeight;

    bool stop = false;
    bool emptySpace = false;
    time_t lastFrame, now;
    bool visible[6];

    char faceChars[6] = {'0', '1', '2', '3', '4', '5'};
    double timePerFrame = 1./((double)fps);
    char screen[(screenWidth + 1) * screenHeight];
    
    translate(cube, 0.5, 0.5, 0.5);
    scale(cube, r, r, r);

    while (!stop){;
        translate(cube, 0, 0, -10);
        rotate(cube, alpha, beta, gamma);

        projectInZ(cube, z1, projectedCube);
        visibleFaces(cube, visible);

        double projectedFaces[6][4][2] = {
            {*projectedCube[0], *projectedCube[1], *projectedCube[2], *projectedCube[3]},
            {*projectedCube[7], *projectedCube[6], *projectedCube[5], *projectedCube[4]},
            {*projectedCube[4], *projectedCube[5], *projectedCube[1], *projectedCube[0]},
            {*projectedCube[3], *projectedCube[2], *projectedCube[6], *projectedCube[7]},
            {*projectedCube[4], *projectedCube[0], *projectedCube[3], *projectedCube[7]},
            {*projectedCube[1], *projectedCube[5], *projectedCube[6], *projectedCube[2]},
        };

        translate(cube, 0, 0, 10);
        for (f=0; f<6; f++){
            faceAreas[f] = visible[f] ? faceArea(projectedFaces[f]) : 0;
        }

        for (int k=0; k<(screenWidth + 1)*screenHeight; k++){
            if (k != 0 && (k % (screenWidth + 1) == 0)){
                screen[k] = '\n';
                continue;
            }
            i = k % screenWidth;
            j = k / screenWidth;

            coord[0] = xMin + i * xUnit;
            coord[1] = yMin + j * yUnit;
            emptySpace = true;
            for (f=0; f<6; f++){
                areaSum = 0;
                areaSum += triangleArea(projectedFaces[f][0], projectedFaces[f][1], coord);
                areaSum += triangleArea(projectedFaces[f][1], projectedFaces[f][2], coord);
                areaSum += triangleArea(projectedFaces[f][2], projectedFaces[f][3], coord);
                areaSum += triangleArea(projectedFaces[f][3], projectedFaces[f][0], coord);
                if (areaSum < faceAreas[f] + 1e-6){
                    screen[k] = faceChars[f];
                    emptySpace = true;
                    break;
                }
            }
            if (emptySpace){
                screen[k] = ' ';
            }
        }
        // Display time
    }
    return 0;
}
void visibleFaces(double cube[8][3], bool visibleFacesMask[6]){
    double v1[3];
    double v2[3];
    double cp[3];
    double n[3];
    double c[3];
    double faces[6][4][3] = {
        {*cube[0], *cube[1], *cube[2], *cube[3]},
        {*cube[7], *cube[6], *cube[5], *cube[4]},
        {*cube[4], *cube[5], *cube[1], *cube[0]},
        {*cube[3], *cube[2], *cube[6], *cube[7]},
        {*cube[4], *cube[0], *cube[3], *cube[7]},
        {*cube[1], *cube[5], *cube[6], *cube[2]},
    };
    for (int i=0; i<6; i++){
        sub(faces[i][1], faces[i][0], v1);
        sub(faces[i][2], faces[i][0], v2);
        cross(v1, v2, cp);
 
        normalise(cp, n);
        centroid(faces[i], c);

        visibleFacesMask[i] = (dot(n, c) > 0) ? true : false;
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
