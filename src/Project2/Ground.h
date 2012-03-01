/*
 * Ground.h: Header file for a class that draws the ground.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef _GROUND_H_
#define _GROUND_H_

#include <Fl/gl.h>

class Ground {
  private:
    GLubyte display_list;   // The display list that does all the work.
    GLuint  texture_obj;    // The object for the grass texture.
    bool    initialized;    // Whether or not we have been initialised.

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    Ground(void) { display_list = 0; initialized = false; };

    // Destructor. Frees the display lists and texture object.
    ~Ground(void);

    // Initializer. Creates the display list.
    bool    Initialize(void);

    // Does the drawing.
    void    Draw(void);
};


#endif
