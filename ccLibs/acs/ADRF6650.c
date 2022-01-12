#include "ADRF6650.h"

// private scoped function prototypes
UI_8 ConnectSerial(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void InitSPI(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
UI_8 spi_in(register_addr_t addr, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void spi_out(register_addr_t addr, UI_8 data, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void reset_chip(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void config_ADRF6650(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void config_TDD(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_D(I_16 value, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_R(I_16 value, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_T(I_16 value, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_frac1(I_16 data, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_frac2(I_16 data, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_mod(I_16 data, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_int(I_16 data, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_phase(I_16 data, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_bleed_en(UI_8 enable, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_bicp(I_16 data, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_cp_current(I_16 data, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_vco_freq(double frequency, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_ref_input(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_lo_freq(double frequency, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_mixer_en(UI_8 enable, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_lo_out_en(UI_8 enable, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_dvga_mode(dvga_mode_t mode, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_dvga1_atten(I_16 data, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void set_dvga2_atten(I_16 data, struct ADRF6650DataStruct* ADRF6650DataPtrIn);





//#include <fcntl.h>
//#include <stdio.h>
//#include <string.h>
//#include <math.h>
//#include <unistd.h>
//#include <sys/ioctl.h>
//#include <cstdlib>
//#include <linux/types.h>
//#include <linux/spi/spidev.h>
//
//// TODO: Collect into SPI class
//static    int spi_fd, gpio_fd;
//static    const char *gpio_number = "68";
//static    const char *spi_device = "/dev/spidev1.1";
//static    const char *gpio_device = "/sys/class/gpio/gpio68/";
//static    const char *gpio_direction = "direction";
//static    const char *gpio_value = "value";
//static    uint32_t mode = 0;
//static    uint8_t bits = 8;
//static    uint32_t sck_speed = 100000;
//static    uint16_t delay = 0;
//
////ADRF6650class::ADRF6650class(struct ADRF6650DataStruct* ADRF6650DataPtrIn) : spi_class() 
//ADRF6650class::ADRF6650class(struct ADRF6650DataStruct* ADRF6650DataPtrIn) 
//{
//    ADRF6650DataPtr = ADRF6650DataPtrIn;
//}
//
//void ADRF6650class::Setup()
//{
//    InitSPI();
//
//    reset_chip();
//
//    //initialize chip's spi port
//    config_ADRF6650();
//    //config_TDD();// XXX: Leave TDD in default state for now
//    set_dvga_mode (DVGA_SPI_MODE);
//
//    // Turn off LO output
//    set_mixer_en(false);
//    set_lo_out_en(false);
//
//    // Turn off IF outputs
//    set_dvga1_atten(DVGA_MAX_ATTEN);
//    set_dvga2_atten(DVGA_MAX_ATTEN);
//
//}
//
//void ADRF6650class::InitSPI()
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
//}
//
//bool ADRF6650class::ResetADRF6550 ()
//{
//    // Reset chip
//    reset_chip ();
//
//}
//
//// Set Frequency of interest
//bool ADRF6650class::SetFrequency(float frequency)
//{
//    // TODO:  Do bounds checking.  If outside bounds,
//    // assume we want to turn off downconverter
//
//
//
//    // Assuming we want 140Mhz IF output
//    return (SetLOFreq (frequency + 140000000));
//}
//
//bool ADRF6650class::SetLOFreq (float frequency)
//{
//
//    set_ref_input();
//    set_lo_freq (frequency);
//
//#define PLL_LOCK_TIMEOUT 5     //wait no more than 5 seconds
//    int counter = 0;
//    while ( (false == is_PLL_Locked()) && (counter < 5) )
//    {
//        counter++;
//        sleep(1);
//    }
//
//    if (counter < 5)
//    {
//        // Turn on LO outputs
//        set_lo_out_en(true);
//        set_mixer_en(true);
//        // Turn on IF outputs
//        set_dvga1_atten(0);
//        set_dvga2_atten(0);
//    }
//    else
//    {
//        //ERROR: What to do?
//        printf ("ERROR: PLL lock timeout\n"); //TESTTESTTEST
//        return false;
//    }
//
//    return true;
//}
//
//bool ADRF6650class::AdjustChannelSpacing (int spacing)
//{
//
//}
//
//bool ADRF6650class::SetReferenceFrequency (int frequency)
//{
//
//}
//
//bool ADRF6650class::SetRDividerValue (int RDivider)
//{
//
//}
//
//bool ADRF6650class::is_PLL_Locked ()
//{
//    uint8_t data = spi_in (LOCK_DETECT_ADDR);
//    if (data & MASK_LOCK_DETECT)
//    {
//        return (true);
//    }
//    return (false);
//}
//
//bool ADRF6650class::EnableMixerBuffer (bool enable) // Enable/disable Mixer output buffer
//{
//    set_mixer_en(enable);
//}
//
//bool ADRF6650class::EnableLOBuffer (bool enable) // Enable/disable LO output buffer
//{
//    set_lo_out_en(enable);
//}
//
//uint8_t ADRF6650class::spi_in (register_addr_t addr)
//{
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
//}
//
//void ADRF6650class::spi_out(register_addr_t addr, uint8_t data)
//{
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
//
//
//}
//
//void ADRF6650class::reset_chip (void)
//{
//    spi_out (ADI_SPI_CONFIG_ADDR, (MASK_SOFTRESET_ | MASK_SOFTRESET));
//}
//
//void ADRF6650class::config_ADRF6650 (void)
//{
//    // Set SPI to 4-wire
//    spi_out (ADI_SPI_CONFIG_ADDR, (MASK_SDOACTIVE | MASK_SDOACTIVE_));
//
//    // Set SPI to 3.3V
//    //XXX: Leave at 1.8V for EVAL board
//    //    uint8_t data = spi_in (CONFIG_ADDR);
//    //    spi_out (CONFIG_ADDR, data | MASK_SPI_18_33_SEL);
//    //    data = spi_in (MULTI_FUNC_CTRL_ADDR);
//    //    spi_out (MULTI_FUNC_CTRL_ADDR, data | MASK_SPI_1P8_3P3_CTRL);
//
//    // Set up Lock Detection  NOTE: Data sheet recommends this bit remain '1'
//    // Data sheet seems to be wrong.  Leave SD_SM_2 at default value.  PLL_LOCK can be read in this case.
//    //    uint8_t data = spi_in (SD_CTRL_ADDR);
//    //    data &= ~MASK_SD_SM_2;
//    //    spi_out (SD_CTRL_ADDR, data);
//
//}
//
//#define PFD_FREQUENCY  20000000  //20MHz
//#define SYSTEM_REF_IN_FREQ 10000000 //10MHz
//#define DESIRED_CHANNEL_SPACING   10000000 //10MHz
//#define CHARGE_PUMP_CURRENT 2.4  //2.4mA (for 20KHz loop filter)
//#define PFD_OFFSET         2     //2.0ns (for 20KHz loop filter)
//
//void ADRF6650class::config_TDD(void)
//{
//    uint8_t data;
//
//    //Set EN_MASK register (0x102) to 0xff.
//    data = (MASK_PLL_ENB_CH12_MASK |
//            MASK_DVGA_ENB_CH2_MASK | 
//            MASK_DVGA_ENB_CH1_MASK | 
//            MASK_IF_ENB_CH2_MASK   | 
//            MASK_IF_ENB_CH1_MASK   | 
//            MASK_LO_STG23_ENB_CH2_MASK |
//            MASK_LO_STG23_ENB_CH1_MASK |
//            MASK_LO_STG1_ENB_MASK);            
//    // Reset these bits
//    //data &= ~(MASK_PLL_ENB_CH12_MASK | MASK_LO_STG1_ENB_MASK);
//    spi_out (EN_MASK_ADDR, data);
//
//    // Set up TDD_BYPASS register (0x100)
//    // Bypass all TDD operation
//    data = spi_in(TDD_BYPASS_ADDR);
//    data |= MASK_BYPASS_TDD;  // bypass TDD operation
//    spi_out (TDD_BYPASS_ADDR, data);
//}
//
//
//void ADRF6650class::set_D(int value)
//{
//    value &= MASK_DOUBLER_EN >> SHIFT_DOUBLER_EN;  //limit to valid input
//    uint8_t data = spi_in(SYNTH_0_ADDR);
//    data |= MASK_DOUBLER_EN & (value << SHIFT_DOUBLER_EN);
//    spi_out (SYNTH_0_ADDR, data);
//}
//
//void ADRF6650class::set_R(int value)
//{
//    value &= MASK_R_DIV;  //limit to valid input
//    uint8_t data = spi_in(R_DIV_ADDR);
//    data &= ~MASK_R_DIV;
//    data |= MASK_R_DIV & value;
//    spi_out (R_DIV_ADDR, data);
//}
//
//void ADRF6650class::set_T(int value)
//{
//    value &= MASK_RDIV2_SEL >> SHIFT_RDIV2_SEL;  //limit to valid input
//    uint8_t data = spi_in(SYNTH_0_ADDR);
//    data |= MASK_RDIV2_SEL & (value << SHIFT_RDIV2_SEL);
//    spi_out (SYNTH_0_ADDR, data);
//}
//
//void ADRF6650class::set_frac1(int data)
//{
//    spi_out(FRAC1_L_ADDR, (uint8_t)(data & MASK_FRAC7_0));
//    data >>= 8;
//    spi_out(FRAC1_M_ADDR, (uint8_t)(data & MASK_FRAC15_8));
//    data >>= 8;
//    spi_out(FRAC1_H_ADDR, (uint8_t)(data & MASK_FRAC23_16));
//}
//
//void ADRF6650class::set_frac2(int data)
//{
//    spi_out(FRAC2_L_ADDR, (uint8_t)(data & MASK_FRAC27_0));
//    data >>= 8;
//    spi_out(FRAC2_H_ADDR, (uint8_t)(data & MASK_FRAC213_8));
//}
//
//void ADRF6650class::set_mod(int data)
//{
//    spi_out(MOD_L_ADDR, (uint8_t)(data & MASK_MOD7_0));
//    data >>= 8;
//    spi_out(MOD_H_ADDR, (uint8_t)(data & MASK_MOD13_8));
//}
//
//void ADRF6650class::set_int(int data)
//{
//    spi_out(INT_H_ADDR, (uint8_t)((data >> 8) & MASK_INT_DIV15_8));
//    // spi_out(INT_L_ADDR, (uint8_t)(data & MASK_INT_DIV7_0)); //PLL should autotune here
//    //spi_out(0x000a,(uint8_t) 0x01); //Write scratchpad to provide additional SCLK pulses
//
//
//    uint8_t addr1, addr0;
//    uint8_t tx[6]; //handle terminating 0 in format string
//    uint8_t rx[6];
//
//    tx[1] = (uint8_t)(INT_L_ADDR & 0x00ff);
//    tx[0] = (uint8_t)(INT_L_ADDR >> 8);
//    tx[2] = (uint8_t)(data & MASK_INT_DIV7_0);
//    tx[3] = 0x0a;     //dummy write to gen extra cycles
//    tx[4] = tx[5] = 0;//dummy write to gen extra cycles
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
//    // Start write cycle
//    char value = '0';
//    write (gpio_fd, &value, 1); // Enable chip select
//
//    struct spi_ioc_transfer tr = {
//        .tx_buf = (unsigned long)&tx,
//        .rx_buf = (unsigned long)&rx,
//        .len = 6,
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
//}
//
//void ADRF6650class::set_phase(int data)
//{
//    spi_out(SD_PHASE_L_0_ADDR, (uint8_t)(data & MASK_PHASE7_0));
//    data >>= 8;
//    spi_out(SD_PHASE_M_0_ADDR, (uint8_t)(data & MASK_PHASE15_8));
//    data >>= 8;
//    spi_out(SD_PHASE_H_0_ADDR, (uint8_t)(data & MASK_PHASE23_16));
//}
//
//void ADRF6650class::set_bleed_en (bool enable)
//{
//    uint8_t data = spi_in (MULTI_FUNC_SYNTH_CTL_022D_ADDR);
//    if (enable)
//    {
//        data |= MASK_BLEED_EN;
//        spi_out (MULTI_FUNC_SYNTH_CTL_022D_ADDR, (uint8_t)(data)); 
//    }
//    else
//    {
//        data &= ~MASK_BLEED_EN;
//        spi_out (MULTI_FUNC_SYNTH_CTL_022D_ADDR, (uint8_t)(data)); 
//    }
//}
//
//void ADRF6650class::set_bicp(int data)
//{
//    data &= MASK_BICP;
//    spi_out (BICP_ADDR, (uint8_t)data);
//}
//
//// Should be '7' for 2.4ma cp output
//void ADRF6650class::set_cp_current(int data)
//{
//    data &= MASK_CP_CURRENT;
//    spi_out (CP_CURR_ADDR, (uint8_t)data);
//}
//
//void ADRF6650class::set_vco_freq (double frequency)
//{
//    int Int, Frac1, Frac2, Mod;
//    double N, N_fraction;
//    double working_float;  //temp variable for chaining calculations
//    double temp_int;  //modf 'integer' output is float
//    //float Fpfd;
//
//    // 
//    //
//
//    // Calculate INT
//    N = frequency / PFD_FREQUENCY;  
//    N_fraction = modf (N, &temp_int);
//    Int = (int)temp_int;
//    //printf ("INT=%d\n", Int); //TESTTESTTEST
//
//    // Calculate FRAC1
//    working_float = N_fraction * pow(2,24);
//    working_float = modf (working_float, &temp_int);
//    Frac1 = (int)temp_int;
//    //printf ("FRAC1=%d\n", Frac1); //TESTTESTTEST
//
//    // Calculate MOD
//    //  1) calculate GCD of Fpfd and desired channel spacing
//    //  2) MOD will equal Fpfd/GCD
//    //  Assumes Fpfd and channel spacing are integers
//    int n1 = PFD_FREQUENCY;
//    int n2 = DESIRED_CHANNEL_SPACING;
//    while(n1!=n2)
//    {
//        if(n1 > n2)
//            n1 -= n2;
//        else
//            n2 -= n1;
//    }
//    // n1 and n2  now represent GCD
//    Mod = (int)(PFD_FREQUENCY / n1);
//    //printf ("MOD=%d\n", Mod); //TESTTESTTEST
//
//    // Calculate Frac2
//    Frac2 = (int)((N_fraction * pow(2,24)) - Frac1) * Mod;
//    //printf ("FRAC2=%d\n", Frac2); //TESTTESTTEST
//
//    // "After determing PLL values, set SD_EN_FRAC0 bit (Register 0x122A, Bit 4) to 1." NOTE: should be bit5
//    uint8_t data = spi_in (SD_CTRL_ADDR);
//    data |= MASK_SD_EN_FRAC0;
//    spi_out (SD_CTRL_ADDR, data);
//
//    set_frac2(Frac2);
//    set_frac1(Frac1);
//    set_mod(Mod);
//    set_int(Int);
//    //printf ("VCO Output = %f\n", frequency); //TESTTESTTEST
//}
//
//void ADRF6650class::set_ref_input ()
//{
//    // Set up REF Input
//    // Assumptions:
//    //  1) REF=10MHz
//    //  2) PFD=20MHz
//    //  3) cp current = 2.4mA
//
//    set_D(1);
//    set_R(1);
//    set_T(0);
//    set_phase(0);
//    set_cp_current(7);
//    set_bicp (26);
//    set_bleed_en(true);
//}
//
//void ADRF6650class::set_lo_freq (double frequency)
//{
//    uint8_t divisor;
//
//    // Determine divisor (Table 15 Data Sheet)
//    if (frequency >= 450000000 && frequency <= 500000000)  //between 450 and 500 MHz inclusive
//    {
//        divisor = 0x10;
//    }
//    else if (frequency > 500000000 && frequency <= 1000000000)
//    {
//        divisor = 0x08;
//    }
//    else if (frequency > 1000000000 && frequency <= 2000000000)
//    {
//        divisor = 0x04;
//    }
//    else if (frequency > 2000000000 && frequency <= 2900000000)
//    {
//        divisor = 0x02;
//    }
//    else
//    {
//        //TODO: Error
//    }
//
//    //printf ("OUT DIV=%d\n", (int)divisor); //TESTTESTTEST
//
//    // Set divisor
//    uint8_t data = spi_in(LO_CNTRL8_ADDR);
//    data &= ~MASK_OUT_DIVRATIO;
//    data |= (MASK_OUT_DIVRATIO & divisor);
//    spi_out (LO_CNTRL8_ADDR, data);
//
//    // Set vco frequency
//    set_vco_freq (frequency * divisor);
//}
//
//void ADRF6650class::set_mixer_en(bool enable)
//{
//    uint8_t data = spi_in(LO_CNTRL8_ADDR);
//    if (true == enable)
//    {
//        data |= MASK_MIX_OE;
//    }
//    else
//    {
//        data &= ~MASK_MIX_OE;
//    }
//
//    spi_out (LO_CNTRL8_ADDR, data);
//}
//
//void ADRF6650class::set_lo_out_en(bool enable)
//{
//    uint8_t data = spi_in(LO_CNTRL8_ADDR);
//    if (true == enable)
//    {
//        data |= MASK_LO_OE;
//    }
//    else
//    {
//        data &= ~MASK_LO_OE;
//    }
//
//    spi_out (LO_CNTRL8_ADDR, data);
//}
//
//
//void ADRF6650class::set_dvga_mode(dvga_mode_t mode)
//{
//    uint8_t data = spi_in (DVGA_MODE_ADDR);  //Addr 0x103
//    if (DVGA_SPI_MODE == mode)
//    {
//        data |= 0x01;
//        spi_out (DVGA_MODE_ADDR, data); 
//    }
//    else if (DVGA_UP_DOWN_MODE == mode)
//    {
//        ; //will need to know up/down step size in this case
//    }
//    else
//    {
//        //TODO: error
//    }
//
//    // when modes are changed, a reset needs to be issued to
//    // the dvgas
//    data = spi_in (BLOCK_RESETS_A_ADDR);
//    data |= (MASK_DVGA_CH1_RSTB | MASK_DVGA_CH2_RSTB);
//    spi_out (BLOCK_RESETS_A_ADDR, data);
//}
//
//void ADRF6650class::set_dvga1_atten(int data)
//{
//    if ((data <= DVGA_MAX_ATTEN) && (data >= 0))
//    {
//        spi_out (DVGA_GAIN1_ADDR, (uint8_t) data);
//    }
//    else
//    {
//        //TODO: error: out of range
//
//    }
//}
//
//void ADRF6650class::set_dvga2_atten(int data)
//{
//    if ((data <= DVGA_MAX_ATTEN) && (data >= 0))
//    {
//        spi_out (DVGA_GAIN2_ADDR, (uint8_t) data);
//    }
//    else
//    {
//        //TODO: error: out of range
//    }
//}
//
//
//
