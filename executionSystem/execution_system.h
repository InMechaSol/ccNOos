#ifndef __EXECUTION_SYSTEM__
#define __EXECUTION_SYSTEM__

#include "version_config.h"

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

struct computeModuleStruct; // forward declaration of compute module data structure

struct executionSystemStruct // declaration of execution system data structure
{
	struct computeModuleStruct* ModuleListHead;
};

struct computeModuleStruct CreateExecutionSystemStruct();

int Execute(struct executionSystemStruct* exeStructIn);



#ifdef __cplusplus
}	// ! extern "C"

class computeModuleClass;  // forward declaration of compute module class

class executionSystemClass // declaration of execution system class
{

};


#endif // !__cplusplus
#endif // ! __EXECUTION_SYSTEM__