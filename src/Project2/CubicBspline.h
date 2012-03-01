/*
 * CubicBspline.h: Header file for a uniform cubic B-spline class.
 *
 * (c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison
 */

#ifndef _CUBICBSPLINE_H_
#define _CUBICBSPLINE_H_

#include <stdio.h>


class CubicBspline {
  private:
    unsigned short  d;		/* The dimension of each point on the curve. */
    unsigned short  n;		/* The number of control points. */
    float   	    **c_pts;	/* The control points. */
    bool	    loop;	/* Whether the curve loops or not. */

  public:
    /* Initializes with the given dimension and no control points. */
    CubicBspline(const unsigned short dim = 3, const bool l = true)
	{ d = dim; n = 0; c_pts = NULL; loop = l; };

    /* Initializes with the given dimension and control points. */
    CubicBspline(const unsigned short, const unsigned short, float**,
		 const bool);

    /* Destructor. */
    ~CubicBspline(void);

    /* Copy operator. */
    CubicBspline& operator=(const CubicBspline&);

    /* Query the dimension. */
    unsigned short	D(void) { return d; };

    /* Query the number of control points. */
    unsigned short	N(void) { return n; };

    /* Query a control point, putting the value into the given array.
    ** Throws an exception if the index is out of range. */
    void		C(unsigned short, float*);

    /* Query whether the curve is a loop. */
    bool		Loop(void) { return loop; }

    /* Change a control point at the given position.
    ** Will throw an exception if the position is out of range. */
    void    Set_Control(const float*, const unsigned short);

    /* Add a control point at the end. */
    void    Append_Control(const float*);

    /* Add a control point at the given position. */
    /* Will throw an exception if the position is beyond the end of
    ** the existing set of control points. */
    void    Insert_Control(const float*, const unsigned short);

    /* Remove a control point at the given position. */
    /* Will throw an exception the position is out of range. */
    void    Delete_Control(const unsigned short);

    /* Evaluate the curve at a parameter value and copy the result into
    ** the given array. Throws an exception if the parameter is out of
    ** range, unless told to wrap. */
    void    Evaluate_Point(const float, float*);

    /* Evaluate the derivative at a parameter value and copy the result into
    ** the given array. Throws an exception if the parameter is out of
    ** range, unless told to wrap. */
    void    Evaluate_Derivative(const float, float*);

    /* Refine the curve one level, putting the result into the given curve. This
    ** will correctly account for cyclic curves. There is also a tolerance
    ** version that stops when each sub-section is locally flat enough. */
    void    Refine(CubicBspline&);
    void    Refine_Tolerance(CubicBspline&, const float);

  private:
    void    Copy_Controls(float**);
    void    Delete_Controls(void);
    bool    Within_Tolerance(const float);
};


#endif

