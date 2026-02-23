#ifndef CUBE_OPERS_H
#define CUBE_OPERS_H

void translate(double cube[8][3], double dx, double dy, double dz);
void rotate(double cube[8][3], double alpha, double beta, double gamma);
void scale(double cube[8][3], double fx, double fy, double fz);

void midpoint(double cube[8][3], double *x, double *y, double *z);

void rotateInX(double cube[8][3], double theta);
void rotateInY(double cube[8][3], double theta);
void rotateInZ(double cube[8][3], double theta);

void projectInZ(double cube[8][3], double z1, double projectedCube[8][2]);
void projectPointInZ(double p[3], double z1, double newP[2]);

//void printCoords(double cube[8][3]);
//void print2DCoords(double cube[8][2]);

#endif
