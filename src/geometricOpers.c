#include <stdio.h>
#include <math.h>

double dot(double a[3], double b[3]){
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
double norm(double v[3]){
    return sqrt(dot(v, v));
}
void normalise(double v[3], double nv[3]){
    double n = norm(v);

    if (n == 0) {
        nv[0] = 0;
        nv[1] = 0;
        nv[2] = 0;
    } else {
        nv[0] = v[0] / n;
        nv[1] = v[1] / n;
        nv[2] = v[2] / n;
    }
}
void centroid(double face[4][3], double c[3]){
    c[0] = (face[0][0] + face[1][0] + face[2][0] + face[3][0]) / 4;
    c[1] = (face[0][1] + face[1][1] + face[2][1] + face[3][1]) / 4;
    c[2] = (face[0][2] + face[1][2] + face[2][2] + face[3][2]) / 4;
    c[3] = (face[0][3] + face[1][3] + face[2][3] + face[3][3]) / 4;
}
void cross(double a[3], double b[3], double c[3]){
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}
void sub(double v1[3], double v2[3], double res[3]){
    res[0] = v1[0] - v2[0];
    res[1] = v1[1] - v2[1];
    res[2] = v1[2] - v2[2];
}
