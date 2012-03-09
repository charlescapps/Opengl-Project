/*
 * Ground.h: Header file for a class that draws the ground.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef _TENT_H_
#define _TENT_H_

#include <Fl/gl.h>
#include <GL/glu.h>
#include "libtarga.h"
#include <stdio.h>

#include "CommonFunctions.h"

class TentObj {
  private:
    GLubyte display_list;   // The display list that does all the work.
    static bool texturesLoaded;
    static GLuint  ground_tex;    // The object for the bottom of the tent
	static GLuint  tent_tex; //texture for sides of tent
    bool    initialized;    // Whether or not we have been initialised.

	double w, h, l; //Geometry of the tent
	double dx, dy, dz; //Translate to?
	double degrees, xRotate, yRotate, zRotate; 

	//not using now. may try later.

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.

    TentObj(void) { display_list = 0; initialized = false; 
		w=5.0;
		h=8.0; 
		l=10.0; 
	};

	TentObj(double w, double h, double l, double dx, double dy, double dz, double degrees, double xRotate, double yRotate, double zRotate) {
		display_list = 0; initialized = false; 
		this->w=w;
		this->h=h; 
		this->l=l; 
		this->dx=dx;
		this->dy=dy; 
		this->dz=dz; 
		this->degrees=degrees; 
		this->xRotate=xRotate;
		this->yRotate=yRotate;
		this->zRotate=zRotate;
	};

    // Destructor. Frees the display lists and texture object.
    ~TentObj(void);

	void DrawTent(); 

    // Initializer. Creates the display list.
    bool    Initialize(void);

    // Does the drawing.
    void    Draw(void);
};


#endif
