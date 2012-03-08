#include "ModelFromObj.h"

ModelFromObj::~ModelFromObj(void)
{
    if ( initialized )
    {
        glDeleteLists(display_list, 1);
        glDeleteTextures(1, &mario_tex);
	
    }
}

void ModelFromObj::DrawModelFromObj(void) {

    glColor4f(1.0,1.0,1.0,1.0); 
    //glBegin(GL_QUADS);
    for (int i = 0; i < loadModel.faceCount; i++) {
        int num = loadModel.faceList[i] -> vertex_count; 
        glBegin(GL_POLYGON);
        for (int j = 0; j < num; j++) {
            int v = loadModel.faceList[i] -> vertex_index[j]; 
            int t = loadModel.faceList[i] -> texture_index[j]; 
            int n = loadModel.faceList[i] -> normal_index[j]; 
            if (n < loadModel.normalCount && n>= 0 && loadModel.normalList[n] != NULL) {
                printf("Found normal: %d: %lf, %lf, %lf\n", n, loadModel.normalList[n]->e[0], loadModel.normalList[n]->e[1],loadModel.normalList[n]->e[2]); 
                glNormal3f(loadModel.normalList[n]->e[0], loadModel.normalList[n]->e[1],loadModel.normalList[n]->e[2]); 
            }
            if (t < loadModel.textureCount && t >= 0 && loadModel.textureList[t] != NULL) {
                glTexCoord2d(loadModel.textureList[t]->e[0],loadModel.textureList[t]->e[1]);
            }
            glVertex3f(scaleFactor*(loadModel.vertexList[v]->e[0]),scaleFactor*(loadModel.vertexList[v]->e[1]),scaleFactor*(loadModel.vertexList[v]->e[2]));
        }
        glEnd(); 
    }
    //glEnd();

}

// Initializer. Creates the display list.
bool ModelFromObj::Initialize(void) {
	// Now do the geometry. Create the display list for drawing a tent.
    display_list = glGenLists(1);
    setupTexDefault((char*)tgaFile, mario_tex);

	glNewList(display_list, GL_COMPILE);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mario_tex);

    DrawModelFromObj();

    glDisable(GL_TEXTURE_2D); 

    glEndList(); 

    printf("Texture count: %d\n", loadModel.textureCount); 
    initialized = true; 
}

void ModelFromObj::Draw(void) {

	glPushMatrix();
    glTranslated(dx, dy, dz); 
    glRotated(degrees, xRotate, yRotate, zRotate);
	glCallList(display_list); 
	glPopMatrix(); 

}
