/** \file LTC2360.c
*   \brief part of ccNOos, Implementation for straight C, or Declarations for C++ wrappers 

    Copyright 2021 InMechaSol, Inc
    https://www.inmechasol.org/

    Licensed under the Apache License, Version 2.0(the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

Notes:
	(.c includes .h) - for straight C or
	(.cpp includes .c which includes .h) - for C++ wrapped straight C
	*Always compiled to a single compilation unit, either C or CPP, not both

*/

#include "LTC2360.h"


// Data initialization function
struct LTC2360DataStruct CreateLTC2360Data()
{
	struct LTC2360DataStruct outStruct;
	outStruct.BandwidthMHz = 0;
	outStruct.PowerMeasuredinBanddB = 0;
	outStruct.current_value = 0;
	outStruct.value_good = ui8FALSE;
	return outStruct;
}

//
////#include "spi_class.h"
//
//#include <stdio.h>
//#include <string.h>
//#include <math.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <sys/ioctl.h>
//#include <cstdlib>
//#include <linux/types.h>
//#include <linux/spi/spidev.h>
//#include <time.h>
//
//
//// TODO: Collect into SPI class
//static    int spi_fd, gpio_fd;
//static    const char *gpio_number = "49";
//static    const char *spi_device = "/dev/spidev1.1";
//static    const char *gpio_device = "/sys/class/gpio/gpio49/";
//static    const char *gpio_direction = "direction";
//static    const char *gpio_value = "value";
//static    uint32_t mode = 0;
//static    uint8_t bits = 12;
//static    uint32_t sck_speed = 100000;
//static    uint16_t delay = 0;
//
////LTC2360class::LTC2360class(struct LTC2360DataStruct* LTC2360DataPtrIn) : spi_class() 
//LTC2360class::LTC2360class(struct LTC2360DataStruct* LTC2360DataPtrIn)
//{
//    LTC2360DataPtr = LTC2360DataPtrIn;
//    if (LTC2360DataPtr)
//    {
//        LTC2360DataPtr->value_good = false; 
//    }
//}
//
//void LTC2360class::Setup()
//{
//    ;
//}
//
//void LTC2360class::InitSPI()
//{
//    ///////////////////////////////
//    ///////////////////////////////
//    //
//    // Set up SPI BUS if necessary
//    //  a) set up gpio chip select
//    //  b) open spi device
//    //
//    ///////////////////////////////
//    ///////////////////////////////
//
//    //
//    // If "/sys/class/gpio/gpio49/value" does not exist, export gpio49
//    if (-1 == access (gpio_device, F_OK ))
//    {
//        gpio_fd = open ("/sys/class/gpio/export", O_WRONLY);
//        if (gpio_fd < 0)
//        {
//            //ERROR - Do something intelligent
//            //printf ("Unable to open 'export' file\n");
//        }
//        write (gpio_fd, gpio_number, strlen(gpio_number));
//        close (gpio_fd);
//        //TODO: Do we need to pause a bit???
//    }
//
//    //
//    // If gpio is not set to 'output', change it
//    char *direction_file = (char *)malloc(strlen(gpio_device) + strlen(gpio_direction));
//    strcpy (direction_file, gpio_device);
//    strcat (direction_file, gpio_direction);
//    gpio_fd = open (direction_file, O_RDWR);
//    free (direction_file);
//
//    char direction[4];
//    read (gpio_fd, direction, sizeof(direction));
//    if (memcmp("out", direction, 3))
//    {
//        write (gpio_fd, "out", 3);
//    }
//    close (gpio_fd);
//
//    //
//    // If gpio chip select is not set to 'inactive', set it to 'inactive'.
//    char *value_file = (char *)malloc(strlen(gpio_device) + strlen(gpio_direction));
//    strcpy (value_file, gpio_device);
//    strcat (value_file, gpio_value);
//    gpio_fd = open (value_file, O_RDWR);
//    free (value_file);
//    char value = '1';
//    write (gpio_fd, &value, 1);
//    close (gpio_fd);
//
//
//    //
//    // Open SPI device and set properties
//    spi_fd = open(spi_device, O_RDWR);
//    if (spi_fd < 0)
//    {
//        // TODO:  ERROR- pabort("can't open spi device");
//    }
//
//    int ret = ioctl(spi_fd, SPI_IOC_WR_MODE32, &mode);
//    if (ret == -1)
//    {
//        //TODO:  ERROR- pabort("can't set spi mode");
//    }
//
//    ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
//    if (ret == -1)
//    {
//        //TODO:  ERROR- pabort("can't set bits per word");
//    }
//
//    ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &sck_speed);
//    if (ret == -1)
//    {
//        //TODO:  ERROR- pabort("can't set max speed hz");
//    }
//    close (spi_fd);
//} //End set up SPI if necessary
//
//uint16_t LTC2360class::ReadValue(int num_of_averages)
//{
//    int spi_fd, gpio_fd;
//    int average = 0;
//    uint16_t tx, rx;
//    int ret = 0;
//    struct timespec req;
//
//    LTC2360DataPtr->value_good = false;
//
//    InitSPI();
//
//    // First, open necessary files
//    //   a) gpio output
//    char *value_file = (char *)malloc(strlen(gpio_device) + strlen(gpio_direction));
//    strcpy (value_file, gpio_device);
//    strcat (value_file, gpio_value);
//    gpio_fd = open (value_file, O_RDWR);
//    free (value_file);
//
//    //  b) spi device
//    spi_fd = open(spi_device, O_RDWR);
//    if (spi_fd < 0)
//    {
//        // TODO:  ERROR- pabort("can't open spi device");
//    }
//
//    for (int i = 0; i < num_of_averages; i++)
//    {
//
//        // Start Conversion read cycle
//        char value = '0';
//        write (gpio_fd, &value, 1); // Enable chip select
//
//        rx = tx = 0;
//        struct spi_ioc_transfer tr = {
//            .tx_buf = (unsigned long)&tx,
//            .rx_buf = (unsigned long)&rx,
//            .len = 2,
//            .speed_hz = sck_speed,
//            .delay_usecs = delay,
//            .bits_per_word = bits,
//        };
//        ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
//        if (ret < 1)
//        {
//            //TODO:  ERROR- pabort("Error sending SPI message");
//        }
//
//        average += rx;
//
//        value = '1';
//        write (gpio_fd, &value, 1); // Deselect chip select
//
//        // LTC2360 conversion time = 8uS min.
//        req.tv_nsec = 8000;
//        req.tv_sec = 0;
//        clock_nanosleep(CLOCK_MONOTONIC, 0, &req, NULL);
//    }
//
//
//    close (spi_fd);
//    close (gpio_fd);
//
//    if (LTC2360DataPtr)
//    {
//        LTC2360DataPtr->current_value = average / num_of_averages; 
//        LTC2360DataPtr->value_good = true; 
//    }
//    return (rx);
//}
//
//
