// 
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

static    FILE_DESC current_spi_dev; //if current_fd == calling_fd, no resetup required


void InitSPI(spi_dev_struct_t *device)
{
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
//    char value = '0';
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
//    int ret;
//    ret = ioctl(spi_fd, SPI_IOC_WR_MODE32, &mode);
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
//        //TODO:  ERROR- pabort("can't set max sck_speed hz");
//    }
//    close (spi_fd);
}



uint8_t spi_in (spi_dev_struct_t *device, register_addr_t addr)
{
//    uint8_t addr1, addr0;
//    uint8_t tx[3+1]; //handle terminating 0 in format string
//    uint8_t rx[3];
//
//    InitSPI();
//
//
//    addr0 = (uint8_t)(addr & 0x00ff);
//    addr1 = (uint8_t)(addr >> 8);
//    addr1 |= 0x80;  //Set read bit
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
//    // Start read cycle
//    char value = '0';
//    write (gpio_fd, &value, 1); // Enable chip select
//
//    sprintf ((char *)tx, "%c%c%c", addr1, addr0, 0xff);
//
//    struct spi_ioc_transfer tr = {
//        .tx_buf = (unsigned long)&tx,
//        .rx_buf = (unsigned long)&rx,
//        .len = 3,
//        .speed_hz = sck_speed,
//        .delay_usecs = delay,
//        .bits_per_word = bits,
//    };
//    int ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
//    if (ret < 1)
//    {
//        //TODO:  ERROR- pabort("Error sending SPI message");
//    }
//    close (spi_fd);
//
//    // End read cycle
//    value = '1';
//    write (gpio_fd, &value, 1); // Enable chip select
//
//    return rx[2];
//
}

void spi_out(spi_dev_struct_t *device, register_addr_t addr, uint8_t data)
{
//    uint8_t addr1, addr0;
//    uint8_t tx[3+1]; //handle terminating 0 in format string
//    uint8_t rx[3];
//
//    InitSPI();
//
//    addr0 = (uint8_t)(addr & 0x00ff);
//    addr1 = (uint8_t)(addr >> 8);
//
//
//    //system ("echo \"0\" > /sys/class/gpio/gpio68/value");
//    // First, open necessary files
//    //   a) open gpio output
//    char *value_file = (char *)malloc(strlen(gpio_device) + strlen(gpio_direction));
//    strcpy (value_file, gpio_device);
//    strcat (value_file, gpio_value);
//    gpio_fd = open (value_file, O_RDWR);
//    free (value_file);
//
//    //  b) open spi device
//    spi_fd = open(spi_device, O_RDWR);
//    if (spi_fd < 0)
//    {
//        // TODO:  ERROR- pabort("can't open spi device");
//    }
//
//    // Start read cycle
//    char value = '0';
//    write (gpio_fd, &value, 1); // Enable chip select
//
//    sprintf ((char *)tx, "%c%c%c", addr1, addr0, data);
//
//    struct spi_ioc_transfer tr = {
//        .tx_buf = (unsigned long)&tx,
//        .rx_buf = (unsigned long)&rx,
//        .len = 3,
//        .speed_hz = sck_speed,
//        .delay_usecs = delay,
//        .bits_per_word = bits,
//    };
//    int ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
//    if (ret < 1)
//    {
//        //TODO:  ERROR- pabort("Error sending SPI message");
//    }
//    close (spi_fd);
//
//    // End read cycle
//    value = '1';
//    write (gpio_fd, &value, 1); // Disable chip select
}


