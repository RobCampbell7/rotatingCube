#ifndef GEOMETRIC_OPERS_H
#define GEOMETRIC_OPERS_H

double dot(double a[3], double b[3]);
double norm(double v[3]);

void normalise(double v[3], double nv[3]);
void centroid(double face[4][3], double c[3]);
void cross(double a[3], double b[3], double c[3]);
void sub(double v1[3], double v2[3], double res[3]);

#endif
