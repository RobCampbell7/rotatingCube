#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "cubeOpers.c"
#include "geometricOpers.c"

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
    double projectedFaces[6][4][2]; 
    double projectedCube[8][2];
    double alpha, beta, gamma;
    double r, x, y, z1;
    double xMin, yMin, xMax, yMax, xUnit, yUnit;
    double faceAreas[6];
    double areaSum;
    int screenWidth, screenHeight, fps, i, j, k, f, v;

    xMin = -1;
    xMax = 1;
    yMin = -1;
    yMax = 1;
    z1 = 6;
    r = 2;
    screenWidth = 60;
    screenHeight = 30;
    fps = 24;

    xUnit = (xMax - xMin) / screenWidth;
    yUnit = (yMax - yMin) / screenHeight;

    bool stop = false;
    bool emptySpace = false;
    time_t lastFrame, now;
    bool visible[6];

    char faceChars[6] = {'0', '1', '2', '3', '4', '5'};
    double timePerFrame = 1./((double)fps);
    char screen[screenWidth * screenHeight];
    
    translate(cube, -0.5, -0.5, -0.5);
    scale(cube, r, r, r);
    //rotate(cube, M_PI/8, 0, 0);
    while (!stop){
        translate(cube, 0, 0, z1);
        printCoords(cube);
        
        //rotate(cube, alpha, beta, gamma);
        projectInZ(cube, 1, projectedCube);
        //print2DCoords(projectedCube);
        visibleFaces(cube, visible);

        translate(cube, 0, 0, -z1);

        for (f = 0; f < 6; f++){
            for (v = 0; v < 4; v++){
                projectedFaces[f][v][0] = projectedCube[faceIndices[f][v]][0];
                projectedFaces[f][v][1] = projectedCube[faceIndices[f][v]][1];
            }
        } 

        for (f=0; f<6; f++){
            faceAreas[f] = faceArea(projectedFaces[f]);
            //faceAreas[f] = visible[f] ? faceArea(projectedFaces[f]) : 0;
            printf("%d ", visible[f]);
        }
        printf("\n");
        //printCoords(cube);
        for (int k=0; k<screenWidth*screenHeight; k++){
            i = k % screenWidth;
            j = k / screenWidth;

            coord[0] = xMin + i * xUnit;
            coord[1] = yMin + j * yUnit;
            //printf("(i, j) = (%d, %d) -> (x, y) = (%f, %f)\n", i, j, coord[0], coord[1]);
            emptySpace = true;
            for (f=0; f<6; f++){
                areaSum = 0;
                areaSum += triangleArea(projectedFaces[f][0], projectedFaces[f][1], coord);
                areaSum += triangleArea(projectedFaces[f][1], projectedFaces[f][2], coord);
                areaSum += triangleArea(projectedFaces[f][2], projectedFaces[f][3], coord);
                areaSum += triangleArea(projectedFaces[f][3], projectedFaces[f][0], coord);

                if (areaSum < faceAreas[f] + 1e-6){
                    screen[k] = faceChars[f];
                    //printf("\nareaSum  : %f\n", areaSum);
                    //printf("faceArea : %f\n", faceAreas[f]);
                    emptySpace = false;
                    //printf("FINALLY!!!");
                    break;
                }
            }
            if (emptySpace == true){
                screen[k] = '-';
            }
        }
        //clear();
        printf("----\n");
        for (k=0; k<screenWidth*screenHeight; k++){
            if (k % screenWidth == 0){
                printf("\n");
            }
            printf("%c", screen[k]);
        }
        printf("\n----\n");
        sleep(1);
        // Display time
    }
    return 0;
}
void visibleFaces(double cube[8][3], bool visibleFacesMask[6]){
    double v1[3], v2[3], cp[3], n[3], ce[3];

    for (int i = 0; i < 6; i++){
        double *a = cube[faceIndices[i][0]];
        double *b = cube[faceIndices[i][1]];
        double *c = cube[faceIndices[i][2]];
        double *d = cube[faceIndices[i][3]];

        sub(b, a, v1);
        sub(c, a, v2);
        cross(v1, v2, cp);
        normalise(cp, n);

        ce[0] = (a[0] + b[0] + c[0] + d[0]) / 4;
        ce[1] = (a[1] + b[1] + c[1] + d[1]) / 4;
        ce[2] = (a[2] + b[2] + c[2] + d[2]) / 4;

        visibleFacesMask[i] = (dot(n, ce) < 0);
    }
}
double toRadians(double angleInDegrees){
    return angleInDegrees * M_PI / 180;
}
double triangleArea(double a[2], double b[2], double c[2]){
    double A = a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) + c[0] * (a[1] - b[1]);
    //printf("a:(%f, %f, %f) ~ b:(%f, %f, %f) c:(%f, %f, %f)", a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2]);
    //printf("A = %f, fabs(A)/2 = %f\n", A, fabs(A) / 2);
    return fabs(A) / 2;
}
double faceArea(double face[4][2]){
    return quadrilateralArea(face[0], face[1], face[2], face[3]);
}
double quadrilateralArea(double a[2], double b[2], double c[2], double d[2]){
    return triangleArea(a, b, c) + triangleArea(c, d, a);
}
