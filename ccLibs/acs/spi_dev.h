#ifndef __SPI_DEVICE_H__
#define __SPI_DEVICE_H__

typedef struct spi_dev_struct {
    UI_16 spi_channel;    //Which SPI device/channel?
    UI_16 spi_chip_select; //which CS on SPI channel?
    FILE_DESC spi_fd;
    FILE_DESC gpio_fd;
    UI_8 *tx_buf;
    UI_8 *rx_buf;
    const char *gpio_number = "68";
    const char *spi_device = "/dev/spidev1.1";
    const char *gpio_device = "/sys/class/gpio/gpio68/";
    const char *gpio_direction = "direction";
    const char *gpio_value = "value";
    UI_32 mode = 0;
    UI_8 bits = 8;
    UI_8 words = 3;
    UI_32 sck_speed = 100000;
    UI_16 delay = 0;
} spi_dev_struct_t;

//Public API
void InitSPI(spi_dev_struct_t *device);
UI_8 spi_in (spi_dev_struct_t *device, register_addr_t addr);
void spi_out(spi_dev_struct_t *device, register_addr_t addr, UI_8 data);

#endif // __SPI_DEVICE_H__
