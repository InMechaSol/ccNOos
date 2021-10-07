/** \file version_config.h
*	\brief Version and Compilation Configuration Settings 
*
* Intended as a common include for all base headers (packetbaseclass, iodevice_class, etc)	
* As a matter of principle and strategy, each of the including headers is expected to
* follow a coding methodology.  For example, Static C functions operating on a pointer to
* a common data structure type are declared and intended for straight C compilation.  C++ Wrapper
* classes are declared to wrap C functions and encapsulate instance data.  
* At this lowest of cross-platform levels, stdlib dependency must be limited to datatypes, datatype sizes, and 
* mathematical functions on those datatypes.  Typically these are in stdint.h, stdfloat.h, and stdmath.h.
*/
#ifndef __VERSIONCONFIG__
#define __VERSIONCONFIG__


#ifdef __cplusplus
							// bool is a type in c++
	#include <cstdint.h>	// standard int w/ fixed width integer since c99
#else
	#include <stdbool.h>	// standard bool available since c99
	#include <stdint.h>		// standard int w/ fixed width integer since c99
#endif // !__cplusplus


/** \def RETURN_SUCCESS
* \brief Function Return Value for Success
*/
#define RETURN_SUCCESS (0)

/** \def RETURN_ERROR
* \brief Function Return Value for ERROR
*/
#define RETURN_ERROR (-1)









#endif // !__VERSIONCONFIG__