/*
 * GenericException.cpp: Class file for a generic exception class.
 *
 * (c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison
 */

#include "GenericException.h"
#include <string.h>


// Constructor stores the message.
GenericException::GenericException(const char *m)
{
    message = new char[strlen(m) + 4];
    strcpy(message, m);
}


// Copy operator. Allocate new space and copy the message.
GenericException&
GenericException::operator=(const GenericException& e)
{
    if ( this != &e )
    {
	delete message;
	message = new char[strlen(e.message)+4];
	strcpy(message, e.message);
    }

    return *this;
}


