/*
 * CubicBspline.cpp: Uniform cubic B-splines
 *
 * (c) 2002 Stephen Chenney, University of Wisconsin at Madison
 */

#include "CubicBspline.h"
#include "GenericException.h"
#include <math.h>


/* Initializes with the given dimension and control points. */
CubicBspline::CubicBspline(const unsigned short dim, const unsigned short num,
                           float **c_in, const bool l)
{
    d = dim;
    n = num;

    Copy_Controls(c_in);

    loop = l;
}


/* Destructor. */
CubicBspline::~CubicBspline(void)
{
    Delete_Controls();
}


/* Copy operator. */
CubicBspline&
CubicBspline::operator=(const CubicBspline &src)
{
    if ( this != &src )
    {
	Delete_Controls();
	d = src.d;
	n = src.n;
	Copy_Controls(src.c_pts);
	loop = src.loop;
    }

    return *this;
}


/* Query a control point, putting the value into the given array, pt.
** Throws an exception if the index is out of range. */
void
CubicBspline::C(unsigned short index, float *pt)
{
    int i;
    if ( index >= n )
	throw new GenericException("CubicBspline::C - Index out of range");

    for ( i = 0 ; i < d ; i++ )
	pt[i] = c_pts[index][i];
}


/* Change a control point at the given position.
** Will throw an exception if the position is out of range. */
void
CubicBspline::Set_Control(const float *pt, const unsigned short posn)
{
    int i;

    if ( posn >= n )
	throw new GenericException(
	    "CubicBspline::Set_Control - Posn out of range");

    for ( i = 0 ; i < d ; i++ )
	c_pts[posn][i] = pt[i];
}

 

/* Add a control point at the end. */
void
CubicBspline::Append_Control(const float *pt)
{
    float   **c_new = new float*[n + 1];
    int     i;

    // Copy the old points over.
    for ( i = 0 ; i < n ; i++ )
	c_new[i] = c_pts[i];

    // Add the new one.
    c_new[n] = new float[d];
    for ( i = 0 ; i < d ; i++ )
	c_new[n][i] = pt[i];

    // Swap old for new.
    delete[] c_pts;
    c_pts = c_new;

    // One more control pt.
    n++;
}


/* Add a control point at the given position. */
/* Will throw an exception if the position is beyond the end of
** the existing set of control points. */
void
CubicBspline::Insert_Control(const float *pt, const unsigned short posn)
{
    float   **c_new = new float*[n + 1];
    int     i;

    if ( posn > n )
	throw new GenericException(
	    "CubicBspline::Insert_Control - Posn out of range");

    // Copy some points over.
    for ( i = 0 ; i < posn ; i++ )
	c_new[i] = c_pts[i];

    // Add the new one.
    c_new[posn] = new float[d];
    for ( i = 0 ; i < d ; i++ )
	c_new[posn][i] = pt[i];

    // Copy the rest of the points over.
    for ( i = posn ; i < n ; i++ )
	c_new[i+1] = c_pts[i];

    // Swap old for new.
    delete[] c_pts;
    c_pts = c_new;

    // One more control pt.
    n++;
}


/* Remove a control point at the given position. */
/* Will throw an exception the position is out of range. */
void
CubicBspline::Delete_Control(const unsigned short posn)
{
    int i;

    if ( posn >= n )
	throw new GenericException(
	    "CubicBspline::Delete_Control - Posn out of range");

    // Get rid of the undesired control point
    delete[] c_pts[posn];

    // Copy the rest of the points down.
    for ( i = posn ; i < n - 1 ; i++ )
	c_pts[i] = c_pts[i+1];

    // One less control pt.
    n--;
}


/* Evaluate the curve at a parameter value and copy the result into
** the given array. Throws an exception if the parameter is out of
** range, unless the curve is a loop. */
void
CubicBspline::Evaluate_Point(const float t, float *pt)
{
    int     posn;
    float   u;
    float   u_sq;
    float   u_cube;
    float   basis[4];
    int     i, j;

    posn = (int)floor(t);

    if ( posn > n - 4 && ! loop )
    {
	throw new GenericException(
	    "CubicBspline::EvaluatePoint - Parameter value out of range");
    }

    u = t - posn;
    u_sq = u * u;
    u_cube = u * u_sq;

    /* This evaluates the blending functions at the parameter value. */
    basis[0] = -u_cube + 3.0f * u_sq - 3.0f * u + 1.0f;
    basis[1] = 3.0f * u_cube - 6.0f * u_sq + 4.0f;
    basis[2] = -3.0f * u_cube + 3.0f * u_sq + 3.0f * u + 1.0f;
    basis[3] = u_cube;

    /* Sum up the control points times the basis functions for each dimension.
    ** j loops over dimension, i loops over control point. */
    for ( j = 0 ; j < d ; j++ )
	pt[j] = 0.0f;
    for ( i = 0 ; i < 4 ; i++ )
    {
	int index = ( posn + i ) % n;
	for ( j = 0 ; j < d ; j++ )
	    pt[j] += c_pts[index][j] * basis[i];
    }
    /* Divide through the constant factor. */
    for ( j = 0 ; j < d ; j++ )
	pt[j] /= 6.0f;
}


/* Evaluate the derivative at a parameter value and copy the result into
** the given array. Throws an exception if the parameter is out of
** range, unless the curve is a loop. */
void
CubicBspline::Evaluate_Derivative(const float t, float *deriv)
{
    int     posn;
    float   u;
    float   u_sq;
    float   basis[4];
    int     i, j;

    posn = (int)floor(t);

    if ( posn > n - 4 && ! loop )
    {
	throw new GenericException(
	    "CubicBspline::EvaluatePoint - Parameter value out of range");
    }

    u = t - posn;
    u_sq = u * u;

    /* Evaluate the derivatives of the blending functions at the parameter
    ** value. */
    basis[0] = -3.0f * u_sq + 6.0f * u - 3.0f;
    basis[1] = 9.0f * u_sq - 12.0f * u;
    basis[2] = -9.0f * u_sq + 6.0f * u + 3.0f;
    basis[3] = 3.0f * u_sq;

    /* Now it's just like evaluating a point. */
    for ( j = 0 ; j < d ; j++ )
	deriv[j] = 0.0f;
    for ( i = 0 ; i < 4 ; i++ )
    {
	int index = ( posn + i ) % n;
	for ( j = 0 ; j < d ; j++ )
	    deriv[j] += c_pts[index][j] * basis[i];
    }
    for ( j = 0 ; j < d ; j++ )
	deriv[j] /= 6.0f;
}


/* Refine the curve, putting the result into the given curve. This
** will correctly account for looped curves. */
void
CubicBspline::Refine(CubicBspline &result)
{
    int	    new_n;
    float   **new_c;
    int     i, j, k;

    /* Figure out how many new vertices. */
    if ( loop )
	new_n = n * 2;
    else
	new_n = n * 2 - 3;

    /* This creates two new controls for each existing one, except for the
    ** first and last control points (unless it loops.) */
    new_c = new float*[new_n];
    for ( i = 0, k = 0 ; i < new_n ; i+=2, k++ )
    {
	/* This figures out which control points to average for the new pts. */
	int p0 = k % n;
	int p1 = ( k + 1 ) % n;
	int p2 = ( k + 2 ) % n;

	/* Allocate space for the new points. */
	new_c[i] = new float[d];
	if ( i + 1 < new_n )
	    new_c[i+1] = new float[d];

	/* Compute the new points using the refinement rules. */
	for ( j = 0 ; j < d ; j++ )
	{
	    new_c[i][j] = 0.5f * ( c_pts[p0][j] + c_pts[p1][j] );
	    if ( i + 1 < new_n )
		new_c[i+1][j] = 0.125f * ( c_pts[p0][j] + 6.0f * c_pts[p1][j]
					 + c_pts[p2][j]);
	}
    }

    /* Get rid of any old control points in the result. */
    result.Delete_Controls();

    /* Copy everything over. */
    result.d = d;
    result.n = new_n;
    result.c_pts = new_c;
    result.loop = loop;
}


/* This function returns true if the curve is locally flat, to within
** tolerance. What it actually does is look at every set of three control
** points in turn, and checks the distance of the middle point from the
** line joining the other two. If the middle point is too far from the line,
** the curve is outside the tolerence. The code is messy because it computes
** certain vectors required for the test, and doesn't abstract them into a
** vector class. Basically all it is doing is a bunch of vector subtractions
** and dot products. */
bool
CubicBspline::Within_Tolerance(const float tolerance)
{
    float   *p;
    float   *x2_x1;
    float   *x3_x1;
    float   l_13, l_2p, dot;
    int     i, j;
    int     m;
    int     i1, i2, i3;

    p = new float[d];
    x2_x1 = new float[d];
    x3_x1 = new float[d];

    m = loop ? n : n - 2;

    for ( i = 0 ; i < m ; i++ )
    {
	i1 = i % n;
	i2 = ( i + 1 ) % n;
	i3 = ( i + 2 ) % n;

	dot = 0.0f;
	l_13 = 0.0f;
	for ( j = 0 ; j < d ; j++ )
	{
	    x2_x1[j] = c_pts[i2][j] - c_pts[i1][j];
	    x3_x1[j] = c_pts[i3][j] - c_pts[i1][j];
	    dot += ( x2_x1[j] * x3_x1[j] );
	    l_13 += ( x3_x1[j] * x3_x1[j] );
	}
	if ( l_13 == 0.0f )
	    continue;
	l_2p = 0.0f;
	for ( j = 0 ; j < d ; j++ )
	{
	    p[j] = c_pts[i1][j] + dot * x3_x1[j] / l_13;
	    l_2p += ( c_pts[i2][j] - p[j] ) * ( c_pts[i2][j] - p[j] );
	}
	if ( l_2p > tolerance * tolerance )
	    return false;
    }

    delete[] p;
    delete[] x2_x1;
    delete[] x3_x1;

    return true;
}


/* Refine a curve until it can be approximated with straight lines to within
** the given tolerance. Always does at least one refinement, even if the
** original curve is inside tolerance. */
void
CubicBspline::Refine_Tolerance(CubicBspline &result, const float tolerance)
{
    Refine(result);
    while ( ! result.Within_Tolerance(tolerance) )
	result.Refine(result);
}


/* Copy a set fo control points */
void
CubicBspline::Copy_Controls(float **c_in)
{
    int i, j;

    if ( c_pts )
	Delete_Controls();

    c_pts = new float*[n];
    for ( i = 0 ; i < n ; i++ )
    {
	c_pts[n] = new float[d];
	for ( j = 0 ; j < d ; j++ )
	    c_pts[n][d] = c_in[n][d];
    }
}


/* Delete a set of control points */
void
CubicBspline::Delete_Controls(void)
{
    int i;

    if ( ! c_pts )
	return;

    for ( i = 0 ; i < n ; i++ )
	delete[] c_pts[i];
    delete[] c_pts;

    c_pts = NULL;
}


