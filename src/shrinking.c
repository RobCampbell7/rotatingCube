#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

#define clear() printf("\033[H\033[J")

const int XDIM = 60;
const int YDIM = 40;

void displayScreen(int screen[YDIM][XDIM]);

float map(float x, float x1, float x2, float y1, float y2);

bool inRadius(float x, float y, float r, float a, float b);

void sleepMS(int milliseconds);

int main()
{
    float xMin = -1;
    float xMax = 1;
    float yMin = -1;
    float yMax = 1;
    float r = 1;
    float dr = 0.05;
    float xCoord, yCoord;
    int board[YDIM][XDIM];
    int i, j;
    
    r = r + dr;
    while (r >= 0){
        r -= dr;
        for (j = 0; j < YDIM; j++){
            yCoord = map(j, 0, YDIM - 1, yMin, yMax);
            for (i = 0; i < XDIM; i++){
                xCoord = map(i, 0, XDIM - 1, xMin, xMax);
                // printf("(%d, %d) -> (%f, %f)\n", i, j, xCoord, yCoord);
                board[j][i] = inRadius(xCoord, yCoord, r, 0., 0.) ? 1 : 0;
            }
        }
        sleepMS(50);
        clear();
        displayScreen(board);
    }
    return 0;
}
/*
void displayScreen(int screen[XDIM][YDIM]){
    char output[YDIM * (XDIM + 1) + 1];
    for (int j = 0; j < YDIM; j++){
        for (int i = 0; i < XDIM; i++){
            output[j * (XDIM + 1)] = screen[j][i] ? '1' : '0';
        }
        output[j * XDIM + 2] = '\n';
    }
    output[YDIM * (XDIM + 1)] = '\0';
    clear();
    printf("%s", output);
}
*/
void displayScreen(int screen[YDIM][XDIM]){
    for (int j = 0; j < YDIM; j++){
        for (int i = 0; i < XDIM; i++){
            printf("%c", screen[j][i] ? '#' : '-');
        }
        printf("\n");
    }
}
float map(float x, float x0, float x1, float y0, float y1){
    return y0 + (y1 - y0) * (x - x0)/(x1 - x0);
}
bool inRadius(float x, float y, float r, float a, float b){
    if (pow((x - a), 2) + pow((y - b), 2) < r * r){
        //printf("(%.1f - %.1f)^2 + (%.1f - %.1f)^2 < %.1f^2\n", x, a, y, b, r);
        //printf("%f < %f\n", pow((x - a), 2) + pow((y - b), 2) , r*r); 
        return true;
    } else {
        return false;
    }
}
void sleepMS(int milliseconds){
    usleep(milliseconds * 1000);
}
