#ifndef __COMPUTE_MODULE__
#define __COMPUTE_MODULE__

struct exeSystemStruct;  // forward declaration of the execution system structure

struct computeModuleStruct
{
	int (*setup) ();
	int (*loop) ();
	int (*handler) ();

	bool setupComplete = false;
	bool loopRunning = false;
	bool hasError = false;
};

struct computeModuleStruct CreateComputeModuleStruct(int (*setup) (), int (*loop) (), int (*handler)());



#endif // ! __COMPUTE_MODULE__