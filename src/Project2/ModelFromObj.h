#ifndef _MARIO_H_
#define _MARIO_H_

#include <iostream>
#include <stdio.h>

#include <Fl/gl.h>
#include <GL/glu.h>
#include "libtarga.h"
#include "math.h"
#include "CommonFunctions.h"
#include "objLoader.h"

class ModelFromObj {
  private:
    GLubyte display_list;   // The display list that does all the work.
    GLuint  mario_tex;    // The object for the bottom of the tent
    bool    initialized;    // Whether or not we have been initialised.

	double dx, dy, dz; //Translate to?
	double degrees, xRotate, yRotate, zRotate; 
    double scaleFactor; 

    objLoader loadModel; 
    char* objFile; 
    char* tgaFile; 


	//not using now. may try later.

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.

    ModelFromObj(void):scaleFactor(5.0) { display_list = 0; initialized = false; 
        dx = dy = dz = degrees = xRotate = yRotate = zRotate = 0.0; 
        loadModel.load((char*)""); 
	}

	ModelFromObj(double dx, double dy, double dz, 
          double degrees, double xRotate, double yRotate, double zRotate, 
          char* objFile, char* tgaFile, double scaleFactor = 5.0){
		display_list = 0; initialized = false; 
		this->dx=dx;
		this->dy=dy; 
		this->dz=dz; 
		this->degrees=degrees; 
		this->xRotate=xRotate;
		this->yRotate=yRotate;
		this->zRotate=zRotate;
        this->objFile = objFile;
        this->tgaFile = tgaFile; 
        this->scaleFactor = scaleFactor;

        loadModel.load(objFile); 
	}

    // Destructor. Frees the display lists and texture object.
    ~ModelFromObj(void);


	void DrawModelFromObj(void); 

    // Initializer. Creates the display list.
    bool Initialize(void);

    // Does the drawing.
    void Draw(void);
};


#endif
