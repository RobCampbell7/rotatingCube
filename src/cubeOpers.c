#include <stdio.h>
#include <math.h>
#include <stdbool.h>

void translate(double cube[8][3], double dx, double dy, double dz);

void rotate(double cube[8][3], double alpha, double beta, double gamma);

void scale(double cube[8][3], double fx, double fy, double fz);

void rotateInX(double cube[8][3], double theta);

void rotateInY(double cube[8][3], double theta);

void rotateInZ(double cube[8][3], double theta);

void projectInZ(double cube[8][3], double z1, double projectedCube[8][2]);

void midpoint(double cube[8][3], double *x, double *y, double *z);

void printCoords(double cube[8][3]);

int main(){
    /*
        5-------6
       /|      /|
      / |     / |
     1-------2  |
     |  |    |  |
     |  4----|--7
     | /     | /
     |/      |/
     0-------3
    KEEP THIS WINDING ORDER CONSISTENT OR IT ALL GOES TITS UP
    */
    double r = 2;
    double x, y, z;
    double cube[8][3] = {
        {0, 0, 0},
        {0, 1, 0},
        {1, 1, 0},
        {1, 0, 0},
        {0, 0, 1},
        {0, 1, 1},
        {1, 1, 1},
        {1, 0, 1},
    };
    translate(cube, -0.5, -0.5, -0.5);
    scale(cube, r, r, r);

    midpoint(cube, &x, &y, &z);
    printf("midpoint = (%f, %f, %f)\n\n", x, y, z);
    
    printCoords(cube);
    
    printf("\n");

    rotate(cube, M_PI/2, M_PI/8, 0);

    midpoint(cube, &x, &y, &z);
    printf("midpoint = (%f, %f, %f)\n\n", x, y, z);
    
    printCoords(cube);
    
    return 0;
}
void translate(double cube[8][3], double dx, double dy, double dz){
    for (int i=0; i<8; i++) {
        cube[i][0] += dx;
        cube[i][1] += dy;
        cube[i][2] += dz;
    }
}
void scale(double cube[8][3], double fx, double fy, double fz){
    for (int i=0; i<8; i++) {
        cube[i][0] *= fx;
        cube[i][1] *= fy;
        cube[i][2] *= fz;
    }
}
void rotate(double cube[8][3], double alpha, double beta, double gamma){
    /*
    alpha = rotation in z-axis
    beta = rotation in y-axis
    gamma = rotation in x-axis
    */
    rotateInX(cube, gamma);
    rotateInY(cube, beta);
    rotateInZ(cube, alpha);
}
void rotateInX(double cube[8][3], double theta){
    double x, y, z;
    for (int i=0; i<8; i++){
        x = cube[i][0];
        y = cube[i][1];
        z = cube[i][2];
        cube[i][0] = x;
        cube[i][1] = y * cos(theta) - z * sin(theta);
        cube[i][2] = y * sin(theta) + z * cos(theta);
    }
}
void rotateInY(double cube[8][3], double theta){
    double x, y, z;
    for (int i=0; i<8; i++){
        x = cube[i][0];
        y = cube[i][1];
        z = cube[i][2];
        cube[i][0] = x * cos(theta) + z * sin(theta);
        cube[i][1] = y;
        cube[i][2] = -x * sin(theta) + z * cos(theta);
    }
}
void rotateInZ(double cube[8][3], double theta){
    double x, y, z;
    for (int i=0; i<8; i++){
        x = cube[i][0];
        y = cube[i][1];
        z = cube[i][2];
        cube[i][0] = x * cos(theta) - y * sin(theta);
        cube[i][1] = x * sin(theta) + y * cos(theta);
        cube[i][2] = z;
    }
}
void projectInZ(double cube[8][3], double z1, double projectedCube[8][2]){
    // z1 > 0 <- IMPORTANT
    double x, y, z;
    for (int i=0; i<8; i++){
        x = cube[i][0];
        y = cube[i][1];
        z = cube[i][2];
        projectedCube[i][0] = x * z1 / z;
        projectedCube[i][1] = y * z1 / z;
   }
}
void midpoint(double cube[8][3], double *x, double *y, double *z){
    double xSum = 0;
    double ySum = 0;
    double zSum = 0;
    for (int i=0; i<8; i++){
        xSum += cube[i][0];
        ySum += cube[i][1];
        zSum += cube[i][2];
    }
    *x = xSum / 8;
    *y = ySum / 8;
    *z = zSum / 8;
}
void printCoords(double cube[8][3]){
    for (int i=0; i<8; i++){
        printf("(%.1f, %.1f, %.1f)", cube[i][0], cube[i][1], cube[i][2]);
        if (i < 7){
            printf(",\n");
        }
    }
    printf("\n");
}
