#ifndef _COMMON_FUNCS
#define _COMMON_FUNCS

#include <Fl/gl.h>
#include <FL/glu.h>
#include <GL/glu.h>
#include <FL/gl2opengl.h>
#include "libtarga.h"
#include <stdio.h>
#include <stdlib.h>
 
#define MAX(x,y) ((x < y) ? (y) : (x))

float randomFloat(float lower, float upper);

GLuint setupTexDefault(const char* filename, GLuint & texValue);

#endif
