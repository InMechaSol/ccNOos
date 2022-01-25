#ifndef ADRF6660_H
#define ADRF6660_H

#include "ADRF6650_Map.h"
#include "version_config.h"


struct ADRF6650DataStruct
{
    I_16 DetectorOutGood;
};

#define DVGA_MAX_ATTEN 43

#define FILE_DESC int

typedef enum {
    DVGA_SPI_MODE,
    DVGA_UP_DOWN_MODE
}dvga_mode_t;

// public scoped functions
struct ADRF6650DataStruct CreateADRF6650Data(); // data initialization function - like a class constructor

void Setup(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
UI_8 ResetADRF6550(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
UI_8 SetFrequency(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
UI_8 SetLOFreq(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
UI_8 is_PLL_Locked(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
UI_8 AdjustChannelSpacing(I_16 spacing, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
UI_8 SetReferenceFrequency(I_16 spacing, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
UI_8 SetRDividerValue(I_16 RDivider, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
UI_8 LockPLL(I_16 wait, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
UI_8 EnableLOBuffer(UI_8 enable, struct ADRF6650DataStruct* ADRF6650DataPtrIn); // Enable/disable LO buffer
UI_8 EnableMixerBuffer(UI_8 enable, struct ADRF6650DataStruct* ADRF6650DataPtrIn); // Enable/disable Mixer buffer


#endif // ADRF6660_CLASS_H
