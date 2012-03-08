/*
 * RobotArm.h: Header file for a class that draws a robot's torso
 *
 * (c) 2012 Charles Capps.
 */


#ifndef _ROBOT_ARM_
#define _ROBOT_ARM_

#include <Fl/gl.h>
#include <stdio.h>
#include <GL/glu.h>
#include "math.h"
#include "../libtarga.h"
#include "../CommonFunctions.h"


class RobotArm {
  private:
    GLubyte display_list;   // The display list that does all the work.

    bool initialized;    // Whether or not we have been initialised.

    double w, h, l; 

    //Overall translation, rotation of torso
	double* rotateArm;
	double* transArm; 

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.

    RobotArm(double w, double h, double l);
    ~RobotArm(void);

    // Initializer. Creates the display list.
    bool Initialize(void);

    void drawRobotArm(); 
	void draw();

    void rotateWalk(double degrees); 
};


#endif
