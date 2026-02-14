#include <stdio.h>
#include <stdbool.h>
#include "cubeOpers.c"
#include "geometricOpers.c"

#define clear() printf("\033[H\033[J")

void visibleFaces(double cube[8][3], bool visibleFacesMask[6]);

double toRadians(double angleinDegrees);

double triangleArea(double a[2], double b[2], double c[2]);

double quadrilateralArea(double a[2], double b[2], double c[2], double d[2]);

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
    double alpha, beta, gamma;
    double r;
    int screenWidth = 20;
    int screenHeight = 10;
    bool visible[6];
    char faceChars[6] = {'0', '1', '2', '3', '4', '5'};

    translate(cube, 0.5, 0.5, 0.5);
    scale(cube, 2, 2, 2);
    
    visibleFaces(cube, visible);

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
double quadrilateralArea(double a[2], double b[2], double c[2], double d[2]){
    return triangleArea(a, b, c) + triangleArea(c, d, a);
}
