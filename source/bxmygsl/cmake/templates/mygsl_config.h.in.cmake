// -*- mode: c++; -*- 
/* mygsl_config.h header file 
 */

#ifndef __mygsl__mygsl_config_h
#define __mygsl__mygsl_config_h 1

/*** mygsl versioning information ***/

//  MYGSL_VERSION % 100        is the patch level
//  MYGSL_VERSION / 100 % 1000 is the minor version
//  MYGSL_VERSION / 100000     is the major version

#define MYGSL_VERSION_MAJOR @mygsl_VERSION_MAJOR@
#define MYGSL_VERSION_MINOR @mygsl_VERSION_MINOR@
#define MYGSL_PATCH_VERSION @mygsl_PATCH_VERSION@

#define MYGSL_VERSION @mygsl_VERSION_NUMERIC@

//  MYGSL_LIB_VERSION must be defined to be the same as MYGSL_VERSION
//  but as a *string* in the form "x.y[.z]" where x is the major version
//  number, y is the minor version number, and z is the patch level if not 0.

#define MYGSL_LIB_VERSION "@mygsl_VERSION_STR@"

#endif // __mygsl__mygsl_config_h

// end of mygsl_config.h
