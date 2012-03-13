/*
 * TentObj.cpp: A class for drawing Tents.
 *
 * (c) Charles Capps 2012.
 */

#include "TentObj.h"

bool TentObj::texturesLoaded = false;
GLuint TentObj::ground_tex = 1;
GLuint TentObj::tent_tex = 1;

// Destructor
TentObj::~TentObj(void)
{
    if ( initialized )
    {
        glDeleteLists(display_list, 1);
    }
}

// Initializer. Returns false if something went wrong, like not being able to
// load the texture.
bool
TentObj::Initialize(void)
{

    if (!TentObj::texturesLoaded) {
        setupTexDefault("tex/walnut.tga", TentObj::ground_tex);
        setupTexDefault("tex/leopard.tga", TentObj::tent_tex);
        TentObj::texturesLoaded = true; 
    }

	// Now do the geometry. Create the display list for drawing a tent.
    display_list = glGenLists(1);

    glNewList(display_list, GL_COMPILE);

	DrawTent();

	glEndList();
 
    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}

void TentObj::DrawTent() {

	// Turn on texturing and bind the walnut texture for the ground.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ground_tex);

	glColor3f(1.0,1.0,1.0); //White since texture has colors

	//Bottom of tent
	glBegin(GL_QUADS); 
		glTexCoord2d(0,0);glVertex3f(0.,0.,0.); 
		glTexCoord2d(1.0,0);glVertex3f(l,0.,0.);
		glTexCoord2d(1,1);glVertex3f(l,w,0.); 
		glTexCoord2d(0,1);glVertex3f(0.,w,0.); 
	glEnd(); 

	//Use leopard texture (tent_tex) for sides
	glBindTexture(GL_TEXTURE_2D, tent_tex);

	//Back triangle
	glBegin(GL_TRIANGLES); 
		glNormal3f(-1,0,0);
		glTexCoord2d(0,0); glVertex3f(0.,0.,0.); 
		glTexCoord2d(0.5,1.0);glVertex3f(0.,w/2,h); 
		glTexCoord2d(1.0,0);glVertex3f(0.,w,0.); 
	glEnd();

	//Front triangle, partially covers front
	glBegin(GL_TRIANGLES);
	    glNormal3f(1,0,0);
		glTexCoord2d(0.3,0);glVertex3f(l,w/3,0.);
		glTexCoord2d(0.5,1.0);glVertex3f(l,w/2,h); 
		glTexCoord2d(0,0.0);glVertex3f(l,0.,0.);  
	glEnd();

	//sides of tent
	glBegin(GL_QUADS); 
		glNormal3f(0,-h,w/2);
		glTexCoord2d(0,0); glVertex3f(0.,0.,0.); 
		glTexCoord2d(1,0); glVertex3f(l,0.,0.); 
		glTexCoord2d(1,1); glVertex3f(l,w/2.,h);
		glTexCoord2d(0,1); glVertex3f(0.,w/2,h); 
	glEnd();
	
	//otherside
	glBegin(GL_QUADS); 
		glNormal3f(0,h,w/2);
		glTexCoord2d(0,0); glVertex3f(0.,w,0); 
		glTexCoord2d(1,0); glVertex3f(0,w/2,h); 
		glTexCoord2d(1,1); glVertex3f(l,w/2,h); 
		glTexCoord2d(0,1); glVertex3f(l,w,0); 
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glColor3f(0.0,0.0,0.0);

	//Back triangle, inside
	glBegin(GL_TRIANGLES);
		glNormal3f(1,0,0);
		glVertex3f(0.,w,0.); 
		glVertex3f(0.,w/2,h); 
		glVertex3f(0.,0.,0.);
	glEnd();

	//inside of tent side
	glBegin(GL_QUADS); 
		glNormal3f(0,-h,-w/2);
		glVertex3f(l,w,0); 
		glVertex3f(l,w/2,h); 
		glVertex3f(0,w/2,h); 
		glVertex3f(0.,w,0); 
	glEnd();

	//inside of other tent side
	glBegin(GL_QUADS); 
		glNormal3f(0,h,-w/2);
		glVertex3f(0.,w/2,h); 
		glVertex3f(l,w/2.,h);
		glVertex3f(l,0.,0.); 
		glVertex3f(0.,0.,0.); 
	glEnd();
}


// Draw just calls the display list we set up earlier.
void
TentObj::Draw(void)
{

	glPushMatrix();
	glTranslated(dx,dy,dz);
	glRotated(degrees,xRotate,yRotate,zRotate);
	glCallList(display_list); 
	glPopMatrix(); 

}


