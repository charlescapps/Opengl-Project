#ifndef _COMMON_FUNCS
#define _COMMON_FUNCS

#include <Fl/gl.h>
#include <FL/glu.h>
#include <GL/glu.h>
#include <FL/gl2opengl.h>
#include "libtarga.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#define MAX(x,y) ((x < y) ? (y) : (x))

const double PI = (4.0*atan(1.0));

float randomFloat(float lower, float upper);

GLuint setupTexDefault(const char* filename, GLuint & texValue);

void crossProduct(float* v1, float* v2, float* output);
double degreesToRad(double deg); 
void normalize3f(double* v); 

void drawRectPrism(double w, double h, double l); 

#endif
