/*
 * RobotTorso.cpp: A class for drawing Tents.
 *
 * (c) Charles Capps 2012.
 */

#include "RobotTorso.h"

RobotTorso::RobotTorso(double w, double h, double l) { 
    display_list = 0; 
    initialized = false;

	rotateRobotTorso = new double[4];
	transRobotTorso = new double[3]; 

    this-> w = w;
    this-> h = h;
    this-> l = l;
}

// Destructor
RobotTorso::~RobotTorso(void)
{
    if ( initialized )
    {
        glDeleteLists(display_list, 1);
    }
}

// Initializer. Returns false if something went wrong, like not being able to
// load the texture.

bool RobotTorso::Initialize(void)
{
    if (!initialized) {
        display_list = glGenLists(1);
    }

	// Now do the geometry. Create the display list for drawing a robot
	glNewList(display_list, GL_COMPILE);

    drawRobotTorso(); 

	glEndList();
 
    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}

void RobotTorso::rotateHips(double degrees) {
    rotateRobotTorso[0] += degrees; 
    if (rotateRobotTorso[0] >= 360.0)
        rotateRobotTorso[0] -= 360.0; 
}

void RobotTorso::draw() {

    glPushMatrix(); 
        glTranslated(transRobotTorso[0], transRobotTorso[1], transRobotTorso[2]); 
        glRotated(rotateRobotTorso[0], rotateRobotTorso[1], rotateRobotTorso[2], rotateRobotTorso[3]); 
        glCallList(display_list); 
    glPopMatrix();

}

void RobotTorso::drawRobotTorso() {
	glColor3f(1.0,0.0,0.0); 

    drawRectPrism(w,h,l);

}



