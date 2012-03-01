/*
 * TreeObj.cpp: A class for drawing Tents.
 *
 * (c) Charles Capps 2012.
 */

#include "Tree.h"


// Destructor
TreeObj::~TreeObj(void)
{
    if ( initialized )
    {
	glDeleteLists(display_list, 1);
	glDeleteTextures(1, &wood_tex);
	glDeleteTextures(1, &leaf_tex);
	
    }
}

// Initializer. Returns false if something went wrong, like not being able to
// load the texture.

void TreeObj::ReInit(void) {
	// Now do the geometry. Create the display list for drawing a tent.
   // display_list = glGenLists(1);
//    glNewList(display_list, GL_COMPILE);

	glNewList(display_list, GL_COMPILE);
	glTranslated(dx,dy,dz);
	glRotated(degrees,xRotate,yRotate,zRotate);

	if (effectsOn) {
		DrawTreeBase();
		DrawTreeTop();
	}
	else {
		DrawTreeBase(); 
		DrawTreeTopNoEffects();
	}
	

	glEndList();

}


bool
TreeObj::Initialize(void)
{
	cylQuad = gluNewQuadric(); 

    ubyte   *image_data;
    int	    image_height, image_width;

	/***************LOAD THE GROUND TEXTURE (WALNUT.TGA)****************/
    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
  if ( ! ( image_data = (ubyte*)tga_load("tex/walnut.tga", &image_width,
					   &image_height, TGA_TRUECOLOR_24) ) )
    {
	fprintf(stderr, "TreeObj::Initialize: Couldn't load walnut.tga\n");
	return false;
    }

    // This creates a texture object and binds it, so the next few operations
    // apply to this texture.
    glGenTextures(1, &wood_tex);
    glBindTexture(GL_TEXTURE_2D, wood_tex);

    // This sets a parameter for how the texture is loaded and interpreted.
    // basically, it says that the data is packed tightly in the image array.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // This sets up the texture with high quality filtering. First it builds
    // mipmaps from the image data, then it sets the filtering parameters
    // and the wrapping parameters. We want the grass to be repeated over the
    // TreeObj.
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


	setupTexDefault("tex/pine-needles.tga", leaf_tex);

	// Now do the geometry. Create the display list for drawing a tent.
    display_list = glGenLists(1);

	glNewList(display_list, GL_COMPILE);

	glTranslated(dx,dy,dz);
	glRotated(degrees,xRotate,yRotate,zRotate);

	DrawTreeBase();
	DrawTreeTop();
	

	glEndList();
 
    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}

void TreeObj::DrawTreeBase() {

	// Turn on texturing and bind the walnut texture for the ground.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wood_tex);

	glColor3f(1.0,1.0,1.0); //White since texture has colors

	gluCylinder(cylQuad, cylR, cylR, cylH, cylSlices, cylStacks); 

	glDisable(GL_TEXTURE_2D);
}

void TreeObj::DrawTreeTop() {

	float texW = triW/MAX(triL,triW); 
	float texH = triL/MAX(triL,triW);
	float texCoordsX[3] = {0.5 - (texW/2.0), 0.5 + (texW/2.0), 0.5 };
	float texCoordsY[3] = {0.0, 0.0, texH};

	glColor3f(0.0,1.0,0.0); 

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, leaf_tex);
	glBegin(GL_TRIANGLES); 

	for (double h = 0; h < coneH ; h+=deltaH) {
		double r = (coneR/coneH)*(coneH-h); 
		int numTris = (int) (2*PI*r/triW);

		for (int i = 0; i <= numTris; i++) {
			double thetMin = (triW*i-triW/2)/r; 
			double thetMax = (triW*i+triW/2)/r; 
			double thet = (triW*i)/r; 
			double thetNext = (triW*(i+1))/r;
			double dThet = randomFloat(triW*overlap/(r*10.0), triW*overlap/(r*2.0));//(h < coneH - 4*deltaH ? triW*overlap/(r*4.0) : 3.0*triW/r);
			double rStretch = randomFloat(coneR*overlap/20.0, coneR*overlap/9.0);//(h < coneH-4*deltaH ? randomFloat(0.0, coneR*overlap/10.0) : 0.0 );
			double ellipseLoc = (h < coneH - 4*deltaH ? ellipse : 1.0); 
			double zStretch = (h < coneH - 4*deltaH ? randomFloat((-overlap/5.0)*coneH, (-overlap/10.0)*coneH) : -coneH/10.0); 
			glNormal3f(deltaH*cos(thet), deltaH*sin(thet), deltaR);
			glTexCoord2d(texCoordsX[0], texCoordsY[0]);
			glVertex3f((rStretch+r)*ellipseLoc*cos(thetMin-dThet), (rStretch+r)*sin(thetMin - dThet), zStretch +  h+cylH); // +
			glTexCoord2d(texCoordsX[1], texCoordsY[1]);
			glVertex3f((rStretch+r)*ellipseLoc*cos(thetMax+dThet), (rStretch+r)*sin(thetMax+dThet), zStretch + h+cylH);
			glTexCoord2d(texCoordsX[2], texCoordsY[2]);
			glVertex3f((r-deltaR)*ellipseLoc*cos(thet), (r-deltaR)*sin(thet), h+deltaH+cylH);

			if (h < coneH - 3.0*deltaH) { //Avoid having a upside down triangle pointing up at the top of the tree.
				glTexCoord2d(texCoordsX[0], texCoordsY[0]);
				glVertex3f((r-deltaR - rStretch)*ellipseLoc*cos(thet), (r-deltaR - rStretch)*sin(thet), h+deltaH+cylH);
				glTexCoord2d(texCoordsX[1], texCoordsY[1]);
				glVertex3f(r*ellipseLoc*cos(thetMax), r*sin(thetMax), h+cylH);
				glTexCoord2d(texCoordsX[2], texCoordsY[2]);
				glVertex3f((r-deltaR - rStretch)*ellipseLoc*cos(thetNext), (r-deltaR - rStretch)*sin(thetNext), h+deltaH+cylH);
			}
		}
	}

	glEnd();
}

void TreeObj::DrawTreeTopNoEffects() {
	glColor3f(0.0,1.0,0.0); 

	glBegin(GL_TRIANGLES); 

	for (double h = 0; h < coneH; h+=deltaH) {
		double r = (coneR/coneH)*(coneH-h); 
		int numTris = (int) (2.0*PI*r/triW);

		for (int i = 0; i <= numTris; i++) {
			double thetMin = (triW*i-triW/2.0)/r; 
			double thetMax = (triW*i+triW/2.0)/r; 
			double thet = (triW*i)/r; 
			double thetNext = (triW*(i+1))/r;
			glNormal3f(deltaH*cos(thet), deltaH*sin(thet), deltaR);
			glVertex3f(r*cos(thetMin), r*sin(thetMin), h+cylH); // +
			glVertex3f(r*cos(thetMax), r*sin(thetMax), h+cylH);
			glVertex3f((r-deltaR)*cos(thet), (r-deltaR)*sin(thet), h+deltaH+cylH);

			if (h < coneH - deltaH) { //Avoid having a upside down triangle pointing up at the top of the tree.
                glNormal3f(deltaH*cos(thet), deltaH*sin(thet), deltaR);
				glVertex3f((r-deltaR)*cos(thet), (r-deltaR)*sin(thet), h+deltaH+cylH);
				glVertex3f(r*cos(thetMax), r*sin(thetMax), h+cylH);
				glVertex3f((r-deltaR)*cos(thetNext), (r-deltaR)*sin(thetNext), h+deltaH+cylH);
			}
		}
	}

	glEnd();
}



// Draw just calls the display list we set up earlier.
void
TreeObj::Draw(void)
{

	glPushMatrix();

	glCallList(display_list); 
	glPopMatrix(); 

}


