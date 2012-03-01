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
    DrawTreeTopNoEffectsTexes(); 
	

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

    double r, thetMin, thetMax, thet, thetNext, dThet, rStretch, rStretch2, ellipseLoc, zStretch, zStretch2; 
    double h1, h2; 

	for (double h = 0; h < coneH; h+=deltaH) {
		double r = (coneR/coneH)*(coneH-h); 
		int numTris = (int) (2*PI*r/triW);

		for (int i = 0; i <= numTris; i++) {
            h1 = h+cylH;
            h2 = (h < coneH - 3*deltaH ? h+cylH + deltaH : h+cylH + deltaH/2.0);
			thetMin = (triW*i-triW/2)/r; 
			thetMax = (triW*i+triW/2)/r; 
			thet = (h < coneH - 3*deltaH ? (triW*i)/r : triW*i/(2.0*r) ); 
			thetNext = (triW*(i+1))/r;
			dThet = randomFloat(triW*overlap/(r*10.0), triW*overlap/(r*5.0));//(h < coneH - 4*deltaH ? triW*overlap/(r*4.0) : 3.0*triW/r);
			rStretch = (h < 0.9*coneH ? randomFloat(coneR*overlap/10.0, coneR*overlap/4.0) : randomFloat(coneR*overlap/20.0, coneR*overlap/15.0) ) ;
            rStretch2 = (h < 0.9*coneH - 3*deltaH ? randomFloat(coneR*overlap/20.0, coneR*overlap/6.0): randomFloat(coneR*overlap/20.0, coneR*overlap/15.0) ) ;
			zStretch = (h < 0.9*coneH ? randomFloat((-overlap/12.0)*coneH, (-overlap/30.0)*coneH) : randomFloat((-overlap/15.0)*coneH, (-overlap/20.0)*coneH)) ; 
			zStretch2 = (h < 0.9*coneH ? randomFloat((-overlap/10.0)*coneH, (-overlap/30.0)*coneH) : randomFloat((-overlap/15.0)*coneH, (-overlap/30.0)*coneH)) ; 

			glNormal3f(deltaH*cos(thet), deltaH*sin(thet), deltaR);
			glTexCoord2d(texCoordsX[0], texCoordsY[0]);
			glVertex3f((rStretch+r)*ellipse*cos(thetMin-dThet), (rStretch+r)*sin(thetMin - dThet), zStretch +  h1); // +
			glTexCoord2d(texCoordsX[1], texCoordsY[1]);
			glVertex3f((rStretch+r)*ellipse*cos(thetMax+dThet), (rStretch+r)*sin(thetMax+dThet), zStretch + h1);
			glTexCoord2d(texCoordsX[2], texCoordsY[2]);
			glVertex3f((r-deltaR)*ellipse*cos(thet), (r-deltaR)*sin(thet), h2);

			if (h < 0.95*coneH) { //Avoid having a upside down triangle pointing up at the top of the tree.
				glTexCoord2d(texCoordsX[0], texCoordsY[0]);
				glVertex3f((r-deltaR - rStretch2)*ellipse*cos(thet+dThet), (r-deltaR - rStretch2)*sin(thet+dThet), h2);
				glTexCoord2d(texCoordsX[1], texCoordsY[1]);
				glVertex3f((r+rStretch2)*ellipse*cos(thetMax), (r+rStretch2)*sin(thetMax), zStretch2 + h1);
				glTexCoord2d(texCoordsX[2], texCoordsY[2]);
				glVertex3f((r-deltaR-rStretch2)*ellipse*cos(thetNext-dThet), (r-deltaR-rStretch2)*sin(thetNext-dThet), h2);
			}
		}
	}

    glDisable(GL_TEXTURE_2D); 

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


void TreeObj::DrawTreeTopNoEffectsTexes() {
	glColor3f(0.0,1.0,0.0); 
	float texW = triW/MAX(triL,triW); 
	float texH = triL/MAX(triL,triW);
	float texCoordsX[3] = {0.5 - (texW/2.0), 0.5 + (texW/2.0), 0.5 };
	float texCoordsY[3] = {0.0, 0.0, texH};

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, leaf_tex);

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
			glTexCoord2d(texCoordsX[0], texCoordsY[0]);
			glVertex3f(r*ellipse*cos(thetMin), r*sin(thetMin), h+cylH); // +
			glTexCoord2d(texCoordsX[1], texCoordsY[1]);
			glVertex3f(r*ellipse*cos(thetMax), r*sin(thetMax), h+cylH);
			glTexCoord2d(texCoordsX[2], texCoordsY[2]);
			glVertex3f((r-deltaR)*ellipse*cos(thet), (r-deltaR)*sin(thet), h+deltaH+cylH);

			if (h < coneH - deltaH) { //Avoid having a upside down triangle pointing up at the top of the tree.
                glNormal3f(deltaH*cos(thet), deltaH*sin(thet), deltaR);
                glTexCoord2d(texCoordsX[0], texCoordsY[0]);
				glVertex3f((r-deltaR)*ellipse*cos(thet), (r-deltaR)*sin(thet), h+deltaH+cylH);
                glTexCoord2d(texCoordsX[1], texCoordsY[1]);
				glVertex3f(r*ellipse*cos(thetMax), r*sin(thetMax), h+cylH);
                glTexCoord2d(texCoordsX[2], texCoordsY[2]);
				glVertex3f((r-deltaR)*ellipse*cos(thetNext), (r-deltaR)*sin(thetNext), h+deltaH+cylH);
			}
		}
	}

    glDisable(GL_TEXTURE_2D); 
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


