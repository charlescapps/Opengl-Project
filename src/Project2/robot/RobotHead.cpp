/*
 * RobotHead.cpp: A class for drawing Tents.
 *
 * (c) Charles Capps 2012.
 */

#include "RobotHead.h"

RobotHead::RobotHead(double w, double h, double l) { 
    display_list = 0; 
    initialized = false;

	rotateHead = new double[4];
	transHead = new double[3]; 

    rotateHead[0] = rotateHead[1] = rotateHead[2] = 0.0; 
    rotateHead[3] = 1.0; 

    transHead[0]=transHead[1]=transHead[2] = 0.0;

    this -> w = w;
    this -> h = h;
    this -> l = l;
}

// Destructor
RobotHead::~RobotHead(void)
{
    if ( initialized )
    {
        glDeleteLists(display_list, 1);
    }
}

// Initializer. Returns false if something went wrong, like not being able to
// load the texture.

bool RobotHead::Initialize(void)
{
    if (!initialized) {
        display_list = glGenLists(1);
    }

	// Now do the geometry. Create the display list for drawing a robot
	glNewList(display_list, GL_COMPILE);

    drawRobotHead(); 

	glEndList();
 
    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}

void RobotHead::draw() {

    glPushMatrix(); 
        glTranslated(transHead[0], transHead[1], transHead[2]); 
        glRotated(rotateHead[0], rotateHead[1], rotateHead[2], rotateHead[3]); 
        glCallList(display_list); 
    glPopMatrix();
}

void RobotHead::drawRobotHead() {
	glColor3f(1.0,0.0,0.0); 

    drawRectPrism(w,h,l);
}

void RobotHead::rotateWalk(double degrees) {
   rotateHead[0]=degrees;   
}


