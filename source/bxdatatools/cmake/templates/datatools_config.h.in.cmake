// -*- mode: c++; -*- 
/* datatools_config.h header file 
 */

#ifndef __datatools__datatools_config_h
#define __datatools__datatools_config_h 1

/*** datatools versioning information ***/

//  DATATOOLS_VERSION % 100        is the patch level
//  DATATOOLS_VERSION / 100 % 1000 is the minor version
//  DATATOOLS_VERSION / 100000     is the major version

#define DATATOOLS_VERSION_MAJOR @datatools_VERSION_MAJOR@
#define DATATOOLS_VERSION_MINOR @datatools_VERSION_MINOR@
#define DATATOOLS_PATCH_VERSION @datatools_PATCH_VERSION@

#define DATATOOLS_VERSION @datatools_VERSION_NUMERIC@

//  DATATOOLS_LIB_VERSION must be defined to be the same as DATATOOLS_VERSION
//  but as a *string* in the form "x.y[.z]" where x is the major version
//  number, y is the minor version number, and z is the patch level if not 0.

#define DATATOOLS_LIB_VERSION "@datatools_VERSION_STR@"

/***  datatools embedded Boost/Serialization based I/O ***/
#define DATATOOLS_WITH_BIO @DATATOOLS_WITH_BIO@

#endif // __datatools__datatools_config_h

// end of datatools_config.h
