/*
 * RobotTorso.h: Header file for a class that draws a robot's torso
 *
 * (c) 2012 Charles Capps.
 */


#ifndef _TORSO_H_
#define _TORSO_H_

#include <Fl/gl.h>
#include <stdio.h>
#include <GL/glu.h>
#include "math.h"
#include "../libtarga.h"
#include "../CommonFunctions.h"


class RobotTorso {
  private:
    GLubyte display_list;   // The display list that does all the work.

    bool initialized;    // Whether or not we have been initialised.

    double w, h, l; 

    //Overall translation, rotation of torso
	double* rotateRobotTorso;
	double* transRobotTorso; 

    void rotateHips(double degrees); 


  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.

    RobotTorso(double w, double h, double l);
    ~RobotTorso(void);

    // Initializer. Creates the display list.
    bool Initialize(void);
	void ReInit(void);

    void drawRobotTorso(); 
	void draw();

};


#endif
