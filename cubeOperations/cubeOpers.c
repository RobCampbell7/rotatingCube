#include <stdio.h>
#include <math.h>
#include <stdbool.h>

void translate(double cube[8][3], double dx, double dy, double dz);

void rotate(double cube[8][3], double alpha, double beta, double gamma);

void scale(double cube[8][3], double factor);

void project(double cube[8][3], double z1);

void rotateInX(double cube[8][3], double theta);

void rotateInY(double cube[8][3], double theta);

void rotateInZ(double cube[8][3], double theta);

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
    return 0;
}
