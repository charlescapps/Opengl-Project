/*
 * Robot.cpp: A class for drawing Tents.
 *
 * (c) Charles Capps 2012.
 */

#include "Robot.h"

Robot::Robot(): walkProgress(0.0){
    initDefaults();

    transLeftLeg[0] = -bodyDimDefault[0]/2.0 + legDimDefault[0]/2.0; 
    transLeftLeg[2] = legDimDefault[2]; 

    transRightLeg[0] = bodyDimDefault[0]/2.0 - legDimDefault[0]/2.0; 
    transRightLeg[2] = legDimDefault[2]; 

    transLeftArm[0]=-bodyDimDefault[0]/2.0-armDimDefault[0]/2.0; 
    transRightArm[0]=bodyDimDefault[0]/2.0+armDimDefault[0]/2.0; 

    transLeftArm[1] = transRightArm[1] = 0.0; 
    transLeftArm[2] = transRightArm[2] = bodyDimDefault[2] + legDimDefault[2];

    transBody[2] = legDimDefault[2]; 
    transHead[2] = bodyDimDefault[2] + legDimDefault[2]; 

    leftLeg = new RobotLeg(legDimDefault[0], legDimDefault[1], legDimDefault[2]);
    rightLeg = new RobotLeg(legDimDefault[0], legDimDefault[1], legDimDefault[2]);
    leftArm = new RobotArm(armDimDefault[0], armDimDefault[1], armDimDefault[2]);
    rightArm = new RobotArm(armDimDefault[0], armDimDefault[1], armDimDefault[2]);
    body = new RobotTorso(bodyDimDefault[0], bodyDimDefault[1], bodyDimDefault[2]);
    head = new RobotHead(headDimDefault[0], headDimDefault[1], headDimDefault[2]);
}

Robot::Robot(double* legDim, double* armDim, double* bodyDim, double* headDim): walkProgress(0.0) { 
    initDefaults();

    transLeftLeg[0] = -bodyDim[0]/2.0 + legDim[0]/2.0; 
    transLeftLeg[2] = legDim[2]; 

    transRightLeg[0] = bodyDim[0]/2.0 - legDim[0]/2.0; 
    transRightLeg[2] = legDim[2]; 

    transLeftArm[0]=-bodyDim[0]/2.0-armDim[0]/2.0; 
    transRightArm[0]=bodyDim[0]/2.0+armDim[0]/2.0; 

    transLeftArm[1] = transRightArm[1] = 0.0; 
    transLeftArm[2] = transRightArm[2] = bodyDim[2] + legDim[2];

    transBody[2] = legDim[2]; 

    transHead[2] = bodyDim[2] + legDim[2]; 

    leftLeg = new RobotLeg(legDim[0], legDim[1], legDim[2]);
    rightLeg = new RobotLeg(legDim[0], legDim[1], legDim[2]);
    leftArm = new RobotArm(armDim[0], armDim[1], armDim[2]);
    rightArm = new RobotArm(armDim[0], armDim[1], armDim[2]);
    body = new RobotTorso(bodyDim[0], bodyDim[1], bodyDim[2]);
    head = new RobotHead(headDim[0], headDim[1], headDim[2]); 
}

void Robot::initDefaults(void) {

    display_list = 0;
    initialized = false;

    headTheta = 0.0; 
    isJumping = isWalking = false; 
    jumpProgress = walkProgress = forwardSpeed = 0.0;
    dir = true; 

    frontDir = new double[3];
    frontDir[0] = frontDir[2] = 0.0; 
    frontDir[1] = 1.0; 

	rotateAll = new double[4];
	transAll = new double[3]; 

    rotateLeftLeg = new double[4];
    rotateRightLeg = new double[4];
    rotateLeftArm = new double[4];
    rotateRightArm = new double[4];
    rotateBody = new double[4];
    rotateHead = new double[4];

    transLeftLeg = new double[3];
    transRightLeg = new double[3];
    transLeftArm = new double[3];
    transRightArm = new double[3];
    transBody = new double[3];
    transHead = new double[3];

    rotateAll[0] = rotateAll[1] = rotateAll[2] = 0.0; 
    rotateAll[3] = 1.0; 

    transAll[0]=transAll[1]=transAll[2] = 0.0;

    rotateLeftLeg[0]=rotateRightLeg[0]=180.0;
    rotateLeftLeg[1]=rotateRightLeg[1]=1.0;
    rotateLeftLeg[2]=rotateLeftLeg[3]=rotateRightLeg[2]=rotateRightLeg[3]=0.0;

    transLeftLeg[1] = transRightLeg[1] = 0.0;

    rotateLeftArm[0]=rotateRightArm[0]=180.0; 
    rotateLeftArm[1]=rotateRightArm[1]=1.0;
    rotateLeftArm[2]=rotateRightArm[2]=0.0; 
    rotateLeftArm[3]=-0.3; 
    rotateRightArm[3]=0.3;

    rotateHead[0] = rotateHead[1] = rotateHead[2] = 0.0;
    rotateHead[3] = 1.0;
    transHead[0] = transHead[1] = 0.0; 

    normalize3f(rotateLeftArm+1); 
    normalize3f(rotateRightArm+1); 

    rotateBody[0] = rotateBody[1] = rotateBody[2] = 0.0;
    rotateBody[3] = 1.0; 
    transBody[0] = transBody[1] = 0.0; 
}

// Destructor
Robot::~Robot(void)
{
    delete[] transAll; 
    delete[] rotateAll; 
    delete leftLeg;
    delete rightLeg; 
    delete leftArm;
    delete rightArm; 
    delete body;
    delete head; 

    delete[] rotateLeftLeg;
    delete[] transLeftLeg; 
    delete[] rotateRightLeg;
    delete[] transRightLeg; 
    delete[] rotateLeftArm; 
    delete[] transLeftArm; 
    delete[] rotateRightArm; 
    delete[] transRightArm; 
    delete[] rotateBody; 
    delete[] transBody; 
    delete[] rotateHead; 
    delete[] transHead; 

    delete[] frontDir; 
}

// Initializer. Returns false if something went wrong, like not being able to
// load the texture.

bool Robot::Initialize(void)
{

    leftLeg -> Initialize();
    rightLeg -> Initialize();
    leftArm -> Initialize();
    rightArm -> Initialize();
    body -> Initialize();
    head -> Initialize(); 

    initialized = true;

    return true;
}

void Robot::draw() {

	//glCallList(display_list); 
    glPushMatrix();

    //Rotate, then translate all body parts
	    glTranslated(transAll[0],transAll[1],transAll[2]);
	    glRotated(rotateAll[0],rotateAll[1],rotateAll[2],rotateAll[3]);

//Apply rotation, translation to place body parts with respect to center of robot
        glPushMatrix();
            glTranslated(transLeftLeg[0],transLeftLeg[1],transLeftLeg[2]);
            glRotated(rotateLeftLeg[0],rotateLeftLeg[1],rotateLeftLeg[2],rotateLeftLeg[3]);
            leftLeg->draw();
        glPopMatrix();

        glPushMatrix();
            glTranslated(transRightLeg[0],transRightLeg[1],transRightLeg[2]);
            glRotated(rotateRightLeg[0],rotateRightLeg[1],rotateRightLeg[2],rotateRightLeg[3]);
            rightLeg->draw();
        glPopMatrix();

        glPushMatrix();
            glTranslated(transLeftArm[0],transLeftArm[1],transLeftArm[2]);
            glRotated(rotateLeftArm[0],rotateLeftArm[1],rotateLeftArm[2],rotateLeftArm[3]);
            leftArm->draw();
        glPopMatrix();

        glPushMatrix();
            glTranslated(transRightArm[0],transRightArm[1],transRightArm[2]);
            glRotated(rotateRightArm[0],rotateRightArm[1],rotateRightArm[2],rotateRightArm[3]);
            rightArm->draw();
        glPopMatrix();

        glPushMatrix();
            glTranslated(transBody[0],transBody[1],transBody[2]);
            glRotated(rotateBody[0],rotateBody[1],rotateBody[2],rotateBody[3]);
            body->draw();
        glPopMatrix();

        glPushMatrix();
            glTranslated(transHead[0],transHead[1],transHead[2]);
            glRotated(rotateHead[0],rotateHead[1],rotateHead[2],rotateHead[3]);
            head->draw();
        glPopMatrix();

    glPopMatrix();

}

void Robot::turnHead(double dTheta) {
    rotateHead[0] += dTheta; 
    if (rotateHead[0] > headTurnMax)
        rotateHead[0] = headTurnMax; 
    else if (rotateHead[0] < -headTurnMax)
        rotateHead[0] = -headTurnMax; 
}

void Robot::turnBody(double dTheta) {
    rotateBody[0] += dTheta; 
    if (rotateBody[0] > bodyTurnMax) {
        rotateBody[0] = bodyTurnMax; 
    }
    else if (rotateBody[0] < -bodyTurnMax) {
        rotateBody[0] = -bodyTurnMax; 
    }
}

void Robot::setDir(bool newDir) {
    dir = newDir; 
}

void Robot::incSpeed(double inc) {
    forwardSpeed += inc; 

    if (forwardSpeed > maxForwardSpeed)
        forwardSpeed = maxForwardSpeed; 
    else if (forwardSpeed < -maxForwardSpeed)
        forwardSpeed = -maxForwardSpeed; 
}

void Robot::accelWalk(double dt) {
    incSpeed((dir==false ? -1.0 : 1.0)*dt*walkAccel); 
}

void Robot::setWalking(bool toWalk) {
    isWalking = toWalk;

    if (toWalk == false) {
        forwardSpeed = 0.0;
    }
}

void Robot::setJumping(bool toJump) {
    if (isJumping == false && toJump == true) {
        isJumping = true; 
        jumpProgress = 0.0; 
        jumpSpeed = initialJumpSpeed; 
    }
}

void Robot::updateJump(double dt) {

    if (!isJumping)
        return;

    jumpSpeed -= G*dt;   
    
    if (transAll[2] < 0.0) {
        transAll[2] = 0.0; 
        isJumping = false; 
        jumpSpeed = 0.0; 
        jumpProgress = 0.0; 
        updateWalk(0.0); 
        return; 
    }

    jumpProgress += dt*3.0; 
    
    if (jumpProgress >=1.0) {
        jumpProgress = 0.0;
    }
    else if (jumpProgress < 0.0) {
        jumpProgress = 1.0; 
    }

    if (0 <= jumpProgress && jumpProgress < 0.5) {
      leftArm->rotateJump(-armMaxAngle*jumpProgress*2.0); 
      rightArm->rotateJump(armMaxAngle*jumpProgress*2.0); 
      leftLeg->rotateJump(-armMaxAngle*jumpProgress*2.0); 
      rightLeg->rotateJump(armMaxAngle*jumpProgress*2.0); 
    }
    else if (0.5 <= jumpProgress && jumpProgress < 1.0) {
        leftArm -> rotateJump(-armMaxAngle*(1.0-jumpProgress)*2.0);
        rightArm -> rotateJump(armMaxAngle*(1.0-jumpProgress)*2.0);
        leftLeg -> rotateJump(-armMaxAngle*(1.0-jumpProgress)*2.0);
        rightLeg -> rotateJump(armMaxAngle*(1.0-jumpProgress)*2.0);
    }

}

void Robot::updateWalk(double inc) {
    walkProgress += inc; 

    if (walkProgress >=1.0) {
        walkProgress = 0.0;
    }
    else if (walkProgress < 0.0) {
        walkProgress = 1.0; 
    }

    if (0 <= walkProgress && walkProgress < 0.25) {
      leftLeg->rotateWalk(legMaxAngle*walkProgress*4.0); 
      rightLeg->rotateWalk(-legMaxAngle*walkProgress*4.0); 
      leftArm->rotateWalk(-armMaxAngle*walkProgress*4.0); 
      rightArm->rotateWalk(armMaxAngle*walkProgress*4.0); 
    }
    else if (0.25 <= walkProgress && walkProgress < 0.5) {
        leftLeg -> rotateWalk(legMaxAngle*(0.5-walkProgress)*4.0);
        rightLeg -> rotateWalk(-legMaxAngle*(0.5-walkProgress)*4.0);
        leftArm -> rotateWalk(-armMaxAngle*(0.5-walkProgress)*4.0);
        rightArm -> rotateWalk(armMaxAngle*(0.5-walkProgress)*4.0);
    }
    else if (0.5 <= walkProgress && walkProgress < 0.75) {
        leftLeg -> rotateWalk(-legMaxAngle*(walkProgress-0.5)*4.0);
        rightLeg -> rotateWalk(legMaxAngle*(walkProgress-0.5)*4.0); 
        leftArm -> rotateWalk(armMaxAngle*(walkProgress-0.5)*4.0);
        rightArm -> rotateWalk(-armMaxAngle*(walkProgress-0.5)*4.0); 
    }
    else if (0.75 <= walkProgress && walkProgress < 1.0) {
        leftLeg -> rotateWalk(-legMaxAngle*(1.0-walkProgress)*4.0);
        rightLeg -> rotateWalk(legMaxAngle*(1.0-walkProgress)*4.0); 
        leftArm -> rotateWalk(armMaxAngle*(1.0-walkProgress)*4.0);
        rightArm -> rotateWalk(-armMaxAngle*(1.0-walkProgress)*4.0); 
    }

/*    transAll[0] += frontDir[0]*walkMoveInc*inc; 
    transAll[1] += frontDir[1]*walkMoveInc*inc; 
    transAll[2] += frontDir[2]*walkMoveInc*inc; */

}

void Robot::turn(double dTheta) {
    rotateAll[0] += dTheta;
    updateFrontDir(); 
}

void Robot::updateMotion(double dt) {

    transAll[0] += frontDir[0]*forwardSpeed*dt; 
    transAll[1] += frontDir[1]*forwardSpeed*dt; 
    transAll[2] += frontDir[2]*forwardSpeed*dt; 

    transAll[2] += jumpSpeed*dt; 

    double timeForWalkCycle = walkMoveInc/forwardSpeed; 

    if (isWalking && !isJumping) {
        updateWalk(dt/timeForWalkCycle); 
        accelWalk(dt); 
    }
    if (isJumping) {
        updateJump(dt); 
    }
}

void Robot::updateFrontDir() {
    double x = rotateAll[1]; 
    double y = rotateAll[2]; 
    double z = rotateAll[3];
    double c = cos(degreesToRad(rotateAll[0]));
    double s = sin(degreesToRad(rotateAll[0]));
    frontDir[0] = x*y*(1-c)-z*s; 
    frontDir[1] = y*y*(1-c)+c; 
    frontDir[2] = y*z*(1-c)+x*s; 
    normalize3f(frontDir); 
}
