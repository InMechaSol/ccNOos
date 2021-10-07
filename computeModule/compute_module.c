#include "compute_module.h"
#include "../executionSystem/execution_system.h"

struct computeModuleStruct CreateComputeModuleStruct(int (*setup) (), int (*loop) ())
{
	struct computeModuleStruct outStruct;
	outStruct.setup = setup;
	outStruct.loop = loop;
	return outStruct;
}

