#ifndef ADRF6660_H
#define ADRF6660_H

#include "ADRF6650_Map.h"
#include "version_config.h"


struct ADRF6650DataStruct
{
    float DesiredCenterFreqMHz;
    float RequiredLOFreqMHz;
    UI_8 LockedOnRequiredLOFreq;
};

#define DVGA_MAX_ATTEN 43

typedef enum {
    DVGA_SPI_MODE,
    DVGA_UP_DOWN_MODE
}dvga_mode_t;

// public scoped functions
struct ADRF6650DataStruct CreateADRF6650Data(); // data initialization function - like a class constructor




#endif // ADRF6660_CLASS_H
