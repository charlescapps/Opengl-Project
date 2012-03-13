/*
 * Ground.h: Header file for a class that draws the ground.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef _TREE_H_
#define _TREE_H_

#include <Fl/gl.h>
#include <GL/glu.h>
#include "libtarga.h"
#include <stdio.h>
#include "math.h"
#include "CommonFunctions.h"


class TreeObj {
  private:
    GLubyte display_list;   // The display list that does all the work.
    static bool texturesLoaded; 
    static GLuint  wood_tex;    // texture for the base of the tree
	static GLuint  leaf_tex;    //texture for leaves/pines

    bool    initialized;    // Whether or not we have been initialised.

	const double PI; 

	double cylR, cylH; //Geometry of the tent
	int cylSlices, cylStacks;

	double coneR, coneH;
	int coneSlices, coneStacks;

	double cosPhi, sinPhi; 
	double triW, triL; 
	double deltaR, deltaH; 

	double dx, dy, dz; //Translate to?
	double degrees, xRotate, yRotate, zRotate; //Rotate?

	double ellipse; //Amount it's skewed on x-axis to look more elliptical. 
	double overlap; //amount to stretch leaves so they overlap. 
	//e.g. 0.5 means leaves span 50% more angle and height, but are rooted at the same place

	bool effectsOn; 

	GLUquadric* cylQuad; 

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.

    TreeObj(void):PI(4.0*atan(1.0)) { display_list = 0; initialized = false; 
		effectsOn = true; 
	
		cylR = 1.0; 
		cylH = 3.0; 
		coneR = 4.0; 
		coneH = 8.0;
		cylSlices = cylStacks = 25;
		coneSlices = coneStacks = 10; 

		cosPhi = coneH / sqrt(coneH*coneH+coneR*coneR);
		sinPhi = coneR / sqrt(coneH*coneH+coneR*coneR);

		triW = 2*PI*coneR/coneSlices; 
		triL = sqrt(coneH*coneH+coneR*coneR)/coneStacks; 

		deltaR = triL*sinPhi;
		deltaH = triL*cosPhi; 
	};

	TreeObj(double cylR, double cylH, double coneR, double coneH, 
		double dx, double dy, double dz, 
		double degrees, double xRotate, double yRotate, double zRotate, 
		int coneSlices, int coneStacks, 
		double ellipse, double overlap):PI(4.0*atan(1.0)) {
		effectsOn = true;
		display_list = 0; initialized = false; 
		this->cylR=cylR;
		this->cylH=cylH; 
		this->coneR=coneR;
		this->coneH=coneH;
		this->dx=dx;
		this->dy=dy; 
		this->dz=dz; 
		this->degrees=degrees; 
		this->xRotate=xRotate;
		this->yRotate=yRotate;
		this->zRotate=zRotate;
		cylSlices = cylStacks = 25; 
		this->coneSlices = coneSlices; 
		this->coneStacks = coneStacks; 

		cosPhi = coneH / sqrt(coneH*coneH+coneR*coneR);
		sinPhi = coneR / sqrt(coneH*coneH+coneR*coneR);

		triW = 2*PI*coneR/coneSlices; 
		triL = sqrt(coneH*coneH+coneR*coneR)/coneStacks; 

		deltaR = triL*sinPhi;
		deltaH = triL*cosPhi; 

		this -> ellipse = ellipse ;
		this -> overlap = overlap;
	}

	void toggleEffects() {effectsOn = !effectsOn; ReInit();}

	void modifyConeStacks(int delta) {
		int newStacks = MAX(6, coneStacks + delta);

		if (newStacks == coneStacks)
			return; 

		coneStacks = newStacks;

		//recompute these.
		triL = sqrt(coneH*coneH+coneR*coneR)/coneStacks; 

		deltaR = triL*sinPhi;
		deltaH = triL*cosPhi; 
	}

	void modifyConeSlices(int delta) {
		int newSlices = MAX (6, coneSlices+ delta);

		if (newSlices == coneSlices)
			return; 

		coneSlices = newSlices; 

		//recompute these.
		triW = 2*PI*coneR/coneSlices; 
	}

	void ReInit(void);

    // Destructor. Frees the display lists and texture object.
    ~TreeObj(void);

	void DrawTreeBase(); 
	void DrawTreeTop();
	void DrawTreeTopNoEffects();
	void DrawTreeTopNoEffectsTexes();

    // Initializer. Creates the display list.
    bool    Initialize(void);

    // Does the drawing.
    void    Draw(void);
};


#endif
