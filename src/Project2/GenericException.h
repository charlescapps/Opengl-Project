
/*
 * GenericException.h: Header file for GenericException class.
 *
 * (c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison
 */

#ifndef _GENERICEXCEPTION_H_
#define _GENERICEXCEPTION_H_

class GenericException {
  private:
    char    *message;	// Just contains a message

  public:
    GenericException(const char *m = "");
    ~GenericException() { delete message; };

    // Copy operator
    GenericException&	operator=(const GenericException&);

    // Return the error message string associated with the exception.
    const char* Message(void) { return message; };
};


#endif
