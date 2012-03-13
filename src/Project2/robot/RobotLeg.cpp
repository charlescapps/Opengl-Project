/*
 * RobotLeg.cpp: A class for drawing Tents.
 *
 * (c) Charles Capps 2012.
 */

#include "RobotLeg.h"

RobotLeg::RobotLeg(double w, double h, double l) { 
    display_list = 0; 
    initialized = false;

	rotateLeg = new double[4];
	transLeg = new double[3]; 

    rotateLeg[0] = rotateLeg[2] = rotateLeg[3] = 0.0; 
    rotateLeg[1] = 1.0; 

    transLeg[0]=transLeg[1]=transLeg[2] = 0.0;

    this -> w = w;
    this -> h = h;
    this -> l = l;
}

// Destructor
RobotLeg::~RobotLeg(void)
{
    if ( initialized )
    {
        glDeleteLists(display_list, 1);
    }
}

// Initializer. Returns false if something went wrong, like not being able to
// load the texture.

bool RobotLeg::Initialize(void)
{
    if (!initialized) {
        display_list = glGenLists(1);
    }

	// Now do the geometry. Create the display list for drawing a robot
	glNewList(display_list, GL_COMPILE);

    drawRobotLeg(); 

	glEndList();
 
    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}

void RobotLeg::draw() {

    glPushMatrix(); 
        glTranslated(transLeg[0], transLeg[1], transLeg[2]); 
        glRotated(rotateLeg[0], rotateLeg[1], rotateLeg[2], rotateLeg[3]); 
        glCallList(display_list); 
    glPopMatrix();
}

void RobotLeg::drawRobotLeg() {
	glColor3f(1.0,0.0,0.0); 

    drawRectPrism(w,h,l);
}

void RobotLeg::rotateWalk(double degrees) {
    rotateLeg[0] = rotateLeg[2] = rotateLeg[3] = 0.0; 
    rotateLeg[1] = 1.0; 
    rotateLeg[0]=degrees;   
}

void RobotLeg::rotateJump(double degrees) {
    rotateLeg[0] = rotateLeg[1] = rotateLeg[3] = 0.0; 
    rotateLeg[2] = 1.0; 
    rotateLeg[0]=degrees;   
}


