/*
 * TentObj.cpp: A class for drawing Tents.
 *
 * (c) Charles Capps 2012.
 */

#include "TentObj.h"


// Destructor
TentObj::~TentObj(void)
{
    if ( initialized )
    {
	glDeleteLists(display_list, 1);
	glDeleteTextures(1, &ground_tex);
	glDeleteTextures(1, &tent_tex);
    }
}

// Initializer. Returns false if something went wrong, like not being able to
// load the texture.
bool
TentObj::Initialize(void)
{
		/*printf("OPENGL VERSION: '%s'\n", glGetString(GL_VERSION));
		printf("OPENGL RENDERER: '%s'\n", glGetString(GL_RENDERER));
		printf("OPENGL VENDOR: '%s'\n", glGetString(GL_VENDOR));
		printf("OPENGL EXTENSIONS: '%s'\n", glGetString(GL_EXTENSIONS));*/

    ubyte   *image_data;
    int	    image_height, image_width;

	/***************LOAD THE GROUND TEXTURE (WALNUT.TGA)****************/
    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
  if ( ! ( image_data = (ubyte*)tga_load("tex/walnut.tga", &image_width,
					   &image_height, TGA_TRUECOLOR_24) ) )
    {
	fprintf(stderr, "TentObj::Initialize: Couldn't load grass.tga\n");
	return false;
    }

    // This creates a texture object and binds it, so the next few operations
    // apply to this texture.
    glGenTextures(1, &ground_tex);
    glBindTexture(GL_TEXTURE_2D, ground_tex);

    // This sets a parameter for how the texture is loaded and interpreted.
    // basically, it says that the data is packed tightly in the image array.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // This sets up the texture with high quality filtering. First it builds
    // mipmaps from the image data, then it sets the filtering parameters
    // and the wrapping parameters. We want the grass to be repeated over the
    // TentObj.
    gluBuild2DMipmaps(GL_TEXTURE_2D,3, image_width, image_height, 
		      GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_NEAREST_MIPMAP_LINEAR);

    // This says what to do with the texture. Modulate will multiply the
    // texture by the underlying color.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

	//********************LOAD THE TENT TEXTURE*************************
	    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
  if ( ! ( image_data = (ubyte*)tga_load("tex/leopard.tga", &image_width,
					   &image_height, TGA_TRUECOLOR_24) ) )
    {
	fprintf(stderr, "TentObj::Initialize: Couldn't load grass.tga\n");
	return false;
    }

    // This creates a texture object and binds it, so the next few operations
    // apply to this texture.
    glGenTextures(1, &tent_tex);
    glBindTexture(GL_TEXTURE_2D, tent_tex);

    // This sets a parameter for how the texture is loaded and interpreted.
    // basically, it says that the data is packed tightly in the image array.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // This sets up the texture with high quality filtering. First it builds
    // mipmaps from the image data, then it sets the filtering parameters
    // and the wrapping parameters. We want the grass to be repeated over the
    // TentObj.
    gluBuild2DMipmaps(GL_TEXTURE_2D,3, image_width, image_height, 
		      GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_NEAREST_MIPMAP_LINEAR);

    // This says what to do with the texture. Modulate will multiply the
    // texture by the underlying color.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

	// Now do the geometry. Create the display list for drawing a tent.
    display_list = glGenLists(2);
    glNewList(display_list, GL_COMPILE);

	glNewList(display_list, GL_COMPILE);

	DrawTent();

	glEndList();
 
	//doFog(); 

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

	glColor3f(0.2,0.2,0.8);

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


