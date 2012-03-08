/*
 * Ground.h: Header file for a class that draws the ground.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */
#ifndef _ROBOT_H_
#define _ROBOT_H_

#include <Fl/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <string.h>
#include "math.h"
#include "assert.h"

#include "../libtarga.h"
#include "../CommonFunctions.h"
#include "RobotTorso.h"
#include "RobotLeg.h"
#include "RobotArm.h"
#include "RobotHead.h"

const double legDimDefault[] = {2.0,2.0,6.0};
const double armDimDefault[] = {1.5,1.5,4.0};
const double bodyDimDefault[] = {6.0,5.0,8.0};
const double headDimDefault[] = {3.0,3.0,3.0};
const double legMaxAngle = 45.0; //Max angle of legs when walking
const double armMaxAngle = 60.0; //Max angle of arms when walking
const double walkMoveInc = 5.0; //Amount to move forward by in one walk cycle

class Robot {
  private:
    GLubyte display_list;   // The display list that does all the work.

    bool initialized;    // Whether or not we have been initialised.

    //Overall translation, rotation of entire Robot
	double* rotateAll;
	double* transAll; 

    //Rotations / translations to move parts from their body coordinate system
    //to the proper place on the body.
    double* rotateLeftLeg;
    double* rotateRightLeg;
    double* rotateLeftArm;
    double* rotateRightArm;
    double* rotateBody;
    double* rotateHead;

    double* transLeftLeg;
    double* transRightLeg;
    double* transLeftArm;
    double* transRightArm;
    double* transBody;
    double* transHead;

    //RobotHead head; 
    RobotArm* leftArm; 
    RobotArm* rightArm;
    RobotLeg* leftLeg; 
    RobotLeg* rightLeg; 
    RobotTorso* body;
    RobotHead* head;

    double* frontDir; 
    double headTheta; 

    //State of operations
    //0 <= walkProgress < 0.5 means legs going one way, 
    //0.5 <= walkProgress < 1.0 means legs going back the other way
    double walkProgress;

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.

    Robot();
    Robot(double* legDim, double* armDim, double* bodyDim, double* headDim);
    ~Robot(void);

    void initDefaults(void);

    bool Initialize(void);
	void draw();

    //A full walk takes place from 0.0 -> 1.0
    void walk(double inc = 0.05);
    void turn(double dTheta = 4.0); 
};


#endif
