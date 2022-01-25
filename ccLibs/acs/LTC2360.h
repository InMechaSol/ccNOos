#ifndef LTC2360_H__
#define LTC2360_H__

#include "version_config.h"


struct LTC2360DataStruct
{
    I_16 current_value;
    UI_8 value_good;
};


void Setup(struct LTC2360DataStruct* LTC2360DataPtrIn);
UI_16 ReadValue(int num_of_averages, struct LTC2360DataStruct* LTC2360DataPtrIn);


#endif  // LTC2360_CLASS_H__
