/*
 * Robot.h: Header file for class that controls a blocky robot
 *
 * Charles L. Capps 2012
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
const double headTurnMax = 45.0; //Max amount to turn head by
const double bodyTurnMax = 20.0;

const double walkAccel = 20.0; 
const double walkMoveInc = 10.0; //Amount to move forward by in one walk cycle
const double maxForwardSpeed = 48.0;

const double initialJumpSpeed = 30.0; 

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
    bool isWalking; 
    bool dir; 
    double walkProgress;
    double forwardSpeed; 

    bool isJumping; 
    double jumpProgress; 
    double jumpSpeed; 

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.

    Robot();
    ~Robot(void);
    Robot(double* legDim, double* armDim, double* bodyDim, double* headDim);

    void initDefaults(void);

    bool Initialize(void);
	void draw();

    void updateFrontDir(); 

    //A full walk takes place from 0.0 -> 1.0
    void turn(double dTheta = 4.0); 
    void turnHead(double dTheta = 4.0); 
    void turnBody(double dTheta = 4.0); 

    void incSpeed(double inc); 
    void accelWalk(double dt); 
    void setWalking(bool toWalk); 
    void setJumping(bool toJump); 
    void updateMotion(double dt); 
    void updateJump(double inc = 0.1); 
    void updateWalk(double inc = 0.05);

    void setDir(bool newDir); 
};


#endif
