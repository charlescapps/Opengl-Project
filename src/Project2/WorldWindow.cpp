/*
 * CS559 Maze Project
 *
 * Class file for the WorldWindow class.
 *
 * (c) Stephen Chenney, University of Wisconsin at Madison, 2001-2002
 *
 */

#include "WorldWindow.h"
#include <Fl/math.h>
#include <Fl/gl.h>
#include <GL/glu.h>
#include <stdio.h>

const double WorldWindow::FOV_X = 45.0;

WorldWindow::WorldWindow(int x, int y, int width, int height, char *label)
	: Fl_Gl_Window(x, y, width, height, label), lookInFrontOfTrainBy(5.0), trainSeatHeight(0.5)
{
	cameraFollowingTrain = cameraBehindTrain = false; 
    button = -1;

    // Initial viewing parameters.
    phi = 45.0f;
    theta = 0.0f;
    dist = 100.0f;
    x_at = 0.0f;
    y_at = 0.0f;

	numTents = 2; 
	someTents = new TentObj*[numTents]; 
	someTents[0] = new TentObj(5,  7, 8, 30, 10, 0.1, 190, 0,0,1);
	someTents[1] = new TentObj(10, 4, 12, 25, 30, 0.1, 210, 0,0,1);

	numTrees = 5;
	someTrees = new TreeObj*[numTrees]; 
	someTrees[0] = new TreeObj(0.8, 4, 5, 12, -25, -20, 0, 0, 0, 0, 1, 20, 20, 1.2, 1.2); 
	someTrees[1] = new TreeObj(1.8, 6, 8, 20, 0, -25, 0, 0, 0, 0, 1, 60, 20, 1.7, 1.5); 
	someTrees[2] = new TreeObj(1.2, 2, 3, 10, -14, -15, 0, 0, 0, 0, 1, 12, 12, 0.8, 0.8); 
	someTrees[3] = new TreeObj(1.2, 2, 3, 10, 10, 25, 0, 0, 0, 0, 1, 40, 12, 0.8, 0.8); 
	someTrees[4] = new TreeObj(0.3, 5, 8, 25, 15, 35, 0, 0, 0, 0, 1, 25, 25, 1.4, 1.2); 

}


void
WorldWindow::draw(void)
{
    double  eye[3];
    float   color[4], dir[4];

    if ( ! valid() )
    {
		// Stuff in here doesn't change from frame to frame, and does not
		// rely on any coordinate system. It only has to be done if the
		// GL context is damaged.

		double	fov_y;

		// Sets the clear color to sky blue.
		glClearColor(0.53f, 0.81f, 0.92f, 1.0);

		// Turn on depth testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Turn on back face culling. Faces with normals away from the viewer
		// will not be drawn.
		glEnable(GL_CULL_FACE);

		// Enable lighting with one light.
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);

		// Ambient and diffuse lighting track the current color.
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);

		// Turn on normal vector normalization. You don't have to give unit
		// normal vector, and you can scale objects.
		glEnable(GL_NORMALIZE);

		// Set up the viewport.
		glViewport(0, 0, w(), h());

		// Set up the persepctive transformation.
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		fov_y = 360.0f / M_PI * atan(h() * tan(FOV_X * M_PI / 360.0) / w());
		gluPerspective(fov_y, w() / (float)h(), 1.0, 1000.0);

		// Do some light stuff. Diffuse color, and zero specular color
		// turns off specular lighting.
		color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f; color[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_SPECULAR, color);

		// Initialize all the objects.
		ground.Initialize();
		traintrack.Initialize();
		for (int i  = 0; i < numTents; i++) 
			someTents[i]->Initialize(); 

		for (int i  = 0; i < numTrees; i++) 
			someTrees[i]->Initialize(); 

    }



    // Stuff out here relies on a coordinate system or must be done on every
    // frame.

    // Clear the screen. Color and depth.
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Set up the viewing transformation. The viewer is at a distance
    // dist from (x_at, y_ay, 2.0) in the direction (theta, phi) defined
    // by two angles. They are looking at (x_at, y_ay, 2.0) and z is up.
    eye[0] = x_at + dist * cos(theta * M_PI / 180.0) * cos(phi * M_PI / 180.0);
    eye[1] = y_at + dist * sin(theta * M_PI / 180.0) * cos(phi * M_PI / 180.0);
    eye[2] = 2.0 + dist * sin(phi * M_PI / 180.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	if (cameraFollowingTrain) {
		float* lookAtParams = getTrainViewLookAt(); 
		gluLookAt(lookAtParams[0], lookAtParams[1], lookAtParams[2], lookAtParams[3], lookAtParams[4], lookAtParams[5], lookAtParams[6], lookAtParams[7], lookAtParams[8]);
		delete[] lookAtParams; 
	}	
	else if (cameraBehindTrain) {
		float* lookAtParams = getBehindTrainViewLookAt(); 
		gluLookAt(lookAtParams[0], lookAtParams[1], lookAtParams[2], lookAtParams[3], lookAtParams[4], lookAtParams[5], lookAtParams[6], lookAtParams[7], lookAtParams[8]);
		delete[] lookAtParams; 
	}
	else {
		gluLookAt(eye[0], eye[1], eye[2], x_at, y_at, 2.0, 0.0, 0.0, 1.0);

	}
    // Position the light source. This has to happen after the viewing
    // transformation is set up, so that the light stays fixed in world
    // space. This is a directional light - note the 0 in the w component.
    dir[0] = 1.0; dir[1] = 1.0; dir[2] = 1.0; dir[3] = 0.0;
    glLightfv(GL_LIGHT0, GL_POSITION, dir);

    // Draw stuff. Everything.
    ground.Draw();
    traintrack.Draw();

	for (int i = 0; i < numTents; i++)
		someTents[i] -> Draw(); 

	for (int i  = 0; i < numTrees; i++) 
		someTrees[i]->Draw();
}

//Use derivative and train position to get train's point of view
float* WorldWindow::getTrainViewLookAt() { //Get the eye coordinates, center point, and up vector
	float posn = traintrack.getPosn(); 
	float* derivative = new float[3]; 
	float* trainPosn = new float[3]; 
	traintrack.getSpline()->Evaluate_Derivative(posn, derivative);
	traintrack.getSpline()->Evaluate_Point(posn,trainPosn); 
	Normalize_3(derivative); 

	float* lookAtParams = new float[9]; //eye coords, center coords, up vector 

	trainPosn[2] += trainSeatHeight; //Add some value to z so the eye is above train, not in center of it. 

	for (int i = 0; i < 3; i++) {
		lookAtParams[i] = trainPosn[i]; //The eye is on the train.
		lookAtParams[3+i] = trainPosn[i] + derivative[i]*lookInFrontOfTrainBy; //The center is in front of the train. 
	}

	lookAtParams[8] = 1.0; //Camera orientation is upwards. Would need more general function if the train could wind around the track. 
	lookAtParams[7] = lookAtParams[6] = 0.0;
	delete[] derivative;
	delete[] trainPosn; 
	return lookAtParams;
}

float* WorldWindow::getBehindTrainViewLookAt() { //Get the eye coordinates, center point, and up vector
	float posn = traintrack.getPosn(); 
	float* derivative = new float[3]; 
	float* trainPosn = new float[3]; 
	traintrack.getSpline()->Evaluate_Derivative(posn, derivative);
	traintrack.getSpline()->Evaluate_Point(posn,trainPosn); 
	Normalize_3(derivative); 

	float* lookAtParams = new float[9]; //eye coords, center coords, up vector 

	trainPosn[2] += trainSeatHeight; //Add some value to z so the eye is above train, not in center of it. 

	for (int i = 0; i < 3; i++) {
		lookAtParams[i] = trainPosn[i]; //The eye is on the train.
		lookAtParams[3+i] = trainPosn[i] - derivative[i]*lookInFrontOfTrainBy; //The center is behind the train. 
	}

	lookAtParams[8] = 1.0; //Camera orientation is upwards. Would need more general function if the train could wind around the track. 
	lookAtParams[7] = lookAtParams[6] = 0.0;
	delete[] derivative;
	delete[] trainPosn; 
	return lookAtParams;
}


void
WorldWindow::Drag(float dt)
{
    int	    dx = x_down - x_last;
    int     dy = y_down - y_last;

    switch ( button )
    {
      case FL_LEFT_MOUSE:
	// Left button changes the direction the viewer is looking from.
	theta = theta_down + 360.0f * dx / (float)w();
	while ( theta >= 360.0f )
	    theta -= 360.0f;
	while ( theta < 0.0f )
	    theta += 360.0f;
	phi = phi_down + 90.0f * dy / (float)h();
	if ( phi > 89.0f )
	    phi = 89.0f;
	if ( phi < -5.0f )
	    phi = -5.0f;
	break;
      case FL_MIDDLE_MOUSE:
	// Middle button moves the viewer in or out.
	dist = dist_down - ( 0.5f * dist_down * dy / (float)h() );
	if ( dist < 1.0f )
	    dist = 1.0f;
	break;
      case FL_RIGHT_MOUSE: {
	// Right mouse button moves the look-at point around, so the world
	// appears to move under the viewer.
	float	x_axis[2];
	float	y_axis[2];

	x_axis[0] = -(float)sin(theta * M_PI / 180.0);
	x_axis[1] = (float)cos(theta * M_PI / 180.0);
	y_axis[0] = x_axis[1];
	y_axis[1] = -x_axis[0];

	x_at = x_at_down + 100.0f * ( x_axis[0] * dx / (float)w()
				    + y_axis[0] * dy / (float)h() );
	y_at = y_at_down + 100.0f * ( x_axis[1] * dx / (float)w()
				    + y_axis[1] * dy / (float)h() );
	} break;
      default:;
    }
}


bool
WorldWindow::Update(float dt)
{
    // Update the view. This gets called once per frame before doing the
    // drawing.

    if ( button != -1 ) // Only do anything if the mouse button is down.
	Drag(dt);

    // Animate the train.
    traintrack.Update(dt);

    return true;
}


int
WorldWindow::handle(int event)
{
    // Event handling routine. Only looks at mouse events.
    // Stores a bunch of values when the mouse goes down and keeps track
    // of where the mouse is and what mouse button is down, if any.
    switch ( event )
    {
	  case FL_FOCUS:
      case FL_UNFOCUS:
         return 1;
      case FL_PUSH:
        button = Fl::event_button();
		x_last = x_down = Fl::event_x();
		y_last = y_down = Fl::event_y();
		phi_down = phi;
		theta_down = theta;
		dist_down = dist;
		x_at_down = x_at;
		y_at_down = y_at;
		return 1;
      case FL_DRAG:
		x_last = Fl::event_x();
		y_last = Fl::event_y();
		return 1;
      case FL_RELEASE:
        button = -1;
		return 1;
	  case FL_KEYBOARD:
        int key = Fl::event_key();
        switch (key)
         {
            case FL_Left:
               redraw();
               return 1;

            case FL_Right:
               redraw();
               return 1;
			case FL_Down:
				return 1;
			case FL_Up:
				return 1;
			case FL_Page_Up: 
				for (int i = 0; i < numTrees; i++) {
					someTrees[i] ->modifyConeSlices(1);
					someTrees[i] -> ReInit(); 
				}
				return 1; 
			case FL_Page_Down: 
				for (int i = 0; i < numTrees; i++) {
					someTrees[i] ->modifyConeSlices(-1);
					someTrees[i] -> ReInit(); 
				}
				return 1; 
			case (FL_F + 1): 
				cameraFollowingTrain = !cameraFollowingTrain; 
				if (cameraFollowingTrain)
					cameraBehindTrain = false;
				return 1; 
			case (FL_F + 2): 
				cameraBehindTrain = !cameraBehindTrain; 
				if (cameraBehindTrain)
					cameraFollowingTrain = false;
				return 1; 
			case FL_Home: 
				for (int i = 0; i < numTrees; i++)
					someTrees[i]->toggleEffects(); 
				return 1; 
         }
    }

    // Pass any other event types on the superclass.
    return Fl_Gl_Window::handle(event);
}


