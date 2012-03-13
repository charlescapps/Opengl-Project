#include "CommonFunctions.h"

float randomFloat(float lower, float upper) {
	float range = upper - lower; 
	return lower + range * ((float)rand() / (float)RAND_MAX);
}

GLuint setupTexDefault(const char* filename, GLuint & texValue) {

	char* image_data; 
	int image_width, image_height; 

    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
  if ( ! ( image_data = (char*)tga_load(filename, &image_width,
					   &image_height, TGA_TRUECOLOR_24) ) )
    {
		fprintf(stderr, "setupTexDefault: Couldn't load %s\n", filename);
		return -1;
    }

    // This creates a texture object and binds it, so the next few operations
    // apply to this texture.
    glGenTextures(1, &texValue);
    glBindTexture(GL_TEXTURE_2D, texValue);

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

    delete[] image_data;

	return texValue; 
}

void crossProduct(float* v1, float* v2, float* output) {
   output[0] = v1[1]*v2[2] - v1[2]*v2[1];  
   output[1] = v1[2]*v2[0] - v1[0]*v2[2];  
   output[2] = v1[0]*v2[1] - v1[1]*v2[0];  
}
double degreesToRad(double deg) {
    return deg*PI/180.0; 
}

void normalize3f(double* v) {
    double mag = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]); 
    v[0] = v[0]/mag; 
    v[1] = v[1]/mag; 
    v[2] = v[2]/mag; 
}

void drawRectPrism(double w, double h, double l) {
    glBegin(GL_QUADS); 
        glNormal3f(0.0,0.0,-1.0); 
       glVertex3f(0.0-w/2.0, 0.0-h/2.0,0.0); 
       glVertex3f(0.0-w/2.0, 0.0+h/2.0,0.0); 
       glVertex3f(0.0+w/2.0, 0.0+h/2.0,0.0); 
       glVertex3f(0.0+w/2.0, 0.0-h/2.0,0.0); 

        glNormal3f(0.0,0.0,1.0); 
       glVertex3f(0.0+w/2.0, 0.0-h/2.0,l); 
       glVertex3f(0.0+w/2.0, 0.0+h/2.0,l); 
       glVertex3f(0.0-w/2.0, 0.0+h/2.0,l); 
       glVertex3f(0.0-w/2.0, 0.0-h/2.0,l); 

        glNormal3f(-1.0,0.0,0.0); 
       glVertex3f(0.0-w/2.0, 0.0+h/2.0,0.0); 
       glVertex3f(0.0-w/2.0, 0.0-h/2.0,0.0); 
       glVertex3f(0.0-w/2.0, 0.0-h/2.0,l); 
       glVertex3f(0.0-w/2.0, 0.0+h/2.0,l); 

        glNormal3f(1.0,0.0,0.0); 
       glVertex3f(0.0+w/2.0, 0.0+h/2.0,l); 
       glVertex3f(0.0+w/2.0, 0.0-h/2.0,l); 
       glVertex3f(0.0+w/2.0, 0.0-h/2.0,0.0); 
       glVertex3f(0.0+w/2.0, 0.0+h/2.0,0.0); 

        glNormal3f(0.0,-1.0,0.0); 
       glVertex3f(0.0-w/2.0, 0.0-h/2.0,0.0); 
       glVertex3f(0.0+w/2.0, 0.0-h/2.0,0.0); 
       glVertex3f(0.0+w/2.0, 0.0-h/2.0,l); 
       glVertex3f(0.0-w/2.0, 0.0-h/2.0,l); 

        glNormal3f(0.0,1.0,0.0); 
       glVertex3f(0.0-w/2.0, 0.0+h/2.0,l); 
       glVertex3f(0.0+w/2.0, 0.0+h/2.0,l); 
       glVertex3f(0.0+w/2.0, 0.0+h/2.0,0.0); 
       glVertex3f(0.0-w/2.0, 0.0+h/2.0,0.0); 
    glEnd();
}
