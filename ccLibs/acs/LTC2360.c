#include "LTC2360.h"
#include "spi_dev.h"

static api_dev_struct_t ltc2360_spi_dev;


void Setup(struct LTC2360DataStruct* LTC2360DataPtrIn)
{
    LTC2360DataPtr = LTC2360DataPtrIn;
    LTC2360DataPtr->current_value = 0;
    LTC2360DataPtr->value_good = 0;

    //SPI specific requirements.
    adrf6650_spi_dev.spi_channel = 0;
    adrf6650_spi_dev.spi_chip_select = 1;
    adrf6650_spi_dev.mode = 0;
    adrf6650_spi_dev.bits = 12;
    adrf6650_spi_dev->words = 3;
    adrf6650_spi_dev.sck_speed = 100000;
    adrf6650_spi_dev.delay = 0;
    InitSPI(&ltc2360_spi_dev); //TODO: register spi device
}



UI_16 LTC2360class::ReadValue(int num_of_averages)
{
    UI_64 average = 0;
    UI_16 tx, rx;

    ltc2360_spi_dev->tx_buf = tx;
    ltc2360_spi_dev->rx_buf = rx;

    LTC2360DataPtr->value_good = 0;

    for (int i = 0; i < num_of_averages; i++)
    {
        spi_out (&ltc2360_spi_dev, 0, 0);

        average += rx;

//TODO: Need some sort of delay guarantee here...
//        // LTC2360 conversion time = 8uS min.
//        req.tv_nsec = 8000;
//        req.tv_sec = 0;
//        clock_nanosleep(CLOCK_MONOTONIC, 0, &req, NULL);
    }


    if (LTC2360DataPtr)
    {
        LTC2360DataPtr->current_value = average / num_of_averages; 
        LTC2360DataPtr->value_good = true; 
    }
    return (rx);
}
