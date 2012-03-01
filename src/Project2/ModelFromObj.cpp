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
    glBegin(GL_QUADS);
    for (int i = 0; i < loadModel.faceCount; i++) {
        int num = loadModel.faceList[i] -> vertex_count; 
        for (int j = 0; j < num; j++) {
            int v = loadModel.faceList[i] -> vertex_index[j]; 
            int t = loadModel.faceList[i] -> texture_index[j]; 
            if (t < loadModel.textureCount && t >= 0 && loadModel.textureList[t] != NULL) {
                glTexCoord2d(loadModel.textureList[t]->e[0],loadModel.textureList[t]->e[1]);
                printf("Tex coord: (%lf, %lf)\n", loadModel.textureList[t]->e[0], loadModel.textureList[t]->e[1]);
            }
            glVertex3f(scaleFactor*(loadModel.vertexList[v]->e[0]),scaleFactor*(loadModel.vertexList[v]->e[1]),scaleFactor*(loadModel.vertexList[v]->e[2]));
        }
    }
    glEnd();

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
    glRotated(degrees, xRotate, yRotate, zRotate);
	glCallList(display_list); 
	glPopMatrix(); 

}
