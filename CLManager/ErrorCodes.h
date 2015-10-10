/*
 * File:   ErrorCodes.h
 * Author: olmozavala
 *
 * Created on September 29, 2011, 4:53 PM
 */

#ifndef ERRORCODES_H
#define	ERRORCODES_H

#include <CL/cl.hpp>

class ErrorCodes {
public:
    ErrorCodes();
    ErrorCodes(const ErrorCodes& orig);
    virtual ~ErrorCodes();
	char* print_cl_error(cl_int err);
private:

};

#endif	/* ERRORCODES_H */

