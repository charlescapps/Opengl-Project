/*
 * RobotArm.cpp: A class for drawing Tents.
 *
 * (c) Charles Capps 2012.
 */

#include "RobotArm.h"

RobotArm::RobotArm(double w, double h, double l) { 
    display_list = 0; 
    initialized = false;

	rotateArm = new double[4];
	transArm = new double[3]; 

    rotateArm[0] = rotateArm[2] = rotateArm[3] = 0.0; 
    rotateArm[1] = 1.0; 

    transArm[0]=transArm[1]=transArm[2] = 0.0;

    this -> w = w;
    this -> h = h;
    this -> l = l;
}

// Destructor
RobotArm::~RobotArm(void)
{
    if ( initialized )
    {
        glDeleteLists(display_list, 1);
    }
}

// Initializer. Returns false if something went wrong, like not being able to
// load the texture.

bool RobotArm::Initialize(void)
{
    if (!initialized) {
        display_list = glGenLists(1);
    }

	// Now do the geometry. Create the display list for drawing a robot
	glNewList(display_list, GL_COMPILE);

    drawRobotArm(); 

	glEndList();
 
    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}

void RobotArm::draw() {

    glPushMatrix(); 
        glTranslated(transArm[0], transArm[1], transArm[2]); 
        glRotated(rotateArm[0], rotateArm[1], rotateArm[2], rotateArm[3]); 
        glCallList(display_list); 
    glPopMatrix();
}

void RobotArm::drawRobotArm() {
	glColor3f(1.0,0.0,0.0); 

    drawRectPrism(w,h,l);
}

void RobotArm::rotateWalk(double degrees) {
    rotateArm[0] = rotateArm[2] = rotateArm[3] = 0.0; 
    rotateArm[1] = 1.0; 
    rotateArm[0]=degrees;   
}

void RobotArm::rotateJump(double degrees) {
    rotateArm[0] = rotateArm[1] = rotateArm[3] = 0.0; 
    rotateArm[2] = 1.0; 
    rotateArm[0]=degrees;   
}

