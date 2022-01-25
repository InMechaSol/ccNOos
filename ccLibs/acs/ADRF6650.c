#include "ADRF6650.h"
#include "spi_dev.h"

// private scoped function prototypes
UI_8 ConnectSerial(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void InitSPI(struct ADRF6650DataStruct* ADRF6650DataPtrIn);
UI_8 spi_in(api_dev_struct_t *adrf6650_spi_dev, register_addr_t addr, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
void spi_out(api_dev_struct_t *adrf6650_spi_dev, register_addr_t addr, UI_8 data, struct ADRF6650DataStruct* ADRF6650DataPtrIn);
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


static api_dev_struct_t adrf6650_spi_dev;


void Setup(struct ADRF6650DataStruct* ADRF6650DataPtrIn)
{
    //Set up data pointers
    ADRF6650DataPtr = ADRF6650DataPtrIn;

    //SPI specific requirements.
    adrf6650_spi_dev.spi_channel = 0;
    adrf6650_spi_dev.spi_chip_select = 0;
    adrf6650_spi_dev.mode = 0;
    adrf6650_spi_dev.bits = 8;
    adrf6650_spi_dev->words = 3;
    adrf6650_spi_dev.sck_speed = 100000;
    adrf6650_spi_dev.delay = 0;
    InitSPI(&adrf6650_spi_dev); //TODO: register spi device

    reset_chip();

    //initialize chip's spi interface
    config_ADRF6650();
    //config_TDD();// XXX: Leave TDD in default state for now
    set_dvga_mode (DVGA_SPI_MODE);

    // Turn off LO output
    set_mixer_en(false);
    set_lo_out_en(false);

    // Turn off IF outputs
    set_dvga1_atten(DVGA_MAX_ATTEN);
    set_dvga2_atten(DVGA_MAX_ATTEN);

}


bool ResetADRF6550 ()
{
    adrf6650_spi_dev->words = 3;
    // Reset chip
    reset_chip ();

}

// Set Frequency of interest
bool SetFrequency(float frequency)
{
    adrf6650_spi_dev->words = 3;
    // TODO:  Do bounds checking.  If outside bounds,
    // assume we want to turn off downconverter



    // Assuming we want 140Mhz IF output
    return (SetLOFreq (frequency + 140000000));
}

bool SetLOFreq (float frequency)
{

    adrf6650_spi_dev->words = 3;
    set_ref_input();
    set_lo_freq (frequency);

#define PLL_LOCK_TIMEOUT 5     //wait no more than 5 seconds
    int counter = 0;
    while ( (false == is_PLL_Locked()) && (counter < 5) )
    {
        counter++;
        sleep(1);
    }

    if (counter < 5)
    {
        // Turn on LO outputs
        set_lo_out_en(true);
        set_mixer_en(true);
        // Turn on IF outputs
        set_dvga1_atten(0);
        set_dvga2_atten(0);
    }
    else
    {
        //ERROR: What to do?
        printf ("ERROR: PLL lock timeout\n"); //TESTTESTTEST
        return false;
    }

    return true;
}

bool AdjustChannelSpacing (int spacing)
{

}

bool SetReferenceFrequency (int frequency)
{

}

bool SetRDividerValue (int RDivider)
{

}

bool is_PLL_Locked ()
{
    adrf6650_spi_dev->words = 3;
    uint8_t data = spi_in (&adrf6650_spi_dev, LOCK_DETECT_ADDR);
    if (data & MASK_LOCK_DETECT)
    {
        return (true);
    }
    return (false);
}

bool EnableMixerBuffer (bool enable) // Enable/disable Mixer output buffer
{
    adrf6650_spi_dev->words = 3;
    set_mixer_en(enable);
}

bool EnableLOBuffer (bool enable) // Enable/disable LO output buffer
{
    adrf6650_spi_dev->words = 3;
    set_lo_out_en(enable);
}


void reset_chip (void)
{
    adrf6650_spi_dev->words = 3;
    spi_out (&adrf6650_spi_dev, ADI_SPI_CONFIG_ADDR, (MASK_SOFTRESET_ | MASK_SOFTRESET));
}

void config_ADRF6650 (void)
{
    adrf6650_spi_dev->words = 3;
    // Set SPI to 4-wire
    spi_out (&adrf6650_spi_dev, ADI_SPI_CONFIG_ADDR, (MASK_SDOACTIVE | MASK_SDOACTIVE_));

    // Set SPI to 3.3V
    //XXX: Leave at 1.8V for EVAL board
    //    uint8_t data = spi_in (&adrf6650_spi_dev, CONFIG_ADDR);
    //    spi_out (&adrf6650_spi_dev, CONFIG_ADDR, data | MASK_SPI_18_33_SEL);
    //    data = spi_in (&adrf6650_spi_dev, MULTI_FUNC_CTRL_ADDR);
    //    spi_out (&adrf6650_spi_dev, MULTI_FUNC_CTRL_ADDR, data | MASK_SPI_1P8_3P3_CTRL);

    // Set up Lock Detection  NOTE: Data sheet recommends this bit remain '1'
    // Data sheet seems to be wrong.  Leave SD_SM_2 at default value.  PLL_LOCK can be read in this case.
    //    uint8_t data = spi_in (&adrf6650_spi_dev, SD_CTRL_ADDR);
    //    data &= ~MASK_SD_SM_2;
    //    spi_out (&adrf6650_spi_dev, SD_CTRL_ADDR, data);

}

#define PFD_FREQUENCY  20000000  //20MHz
#define SYSTEM_REF_IN_FREQ 10000000 //10MHz
#define DESIRED_CHANNEL_SPACING   10000000 //10MHz
#define CHARGE_PUMP_CURRENT 2.4  //2.4mA (for 20KHz loop filter)
#define PFD_OFFSET         2     //2.0ns (for 20KHz loop filter)

void config_TDD(void)
{
    adrf6650_spi_dev->words = 3;
    uint8_t data;

    //Set EN_MASK register (0x102) to 0xff.
    data = (MASK_PLL_ENB_CH12_MASK |
            MASK_DVGA_ENB_CH2_MASK | 
            MASK_DVGA_ENB_CH1_MASK | 
            MASK_IF_ENB_CH2_MASK   | 
            MASK_IF_ENB_CH1_MASK   | 
            MASK_LO_STG23_ENB_CH2_MASK |
            MASK_LO_STG23_ENB_CH1_MASK |
            MASK_LO_STG1_ENB_MASK);            
    // Reset these bits
    //data &= ~(MASK_PLL_ENB_CH12_MASK | MASK_LO_STG1_ENB_MASK);
    spi_out (&adrf6650_spi_dev, EN_MASK_ADDR, data);

    // Set up TDD_BYPASS register (0x100)
    // Bypass all TDD operation
    data = spi_in(&adrf6650_spi_dev, TDD_BYPASS_ADDR);
    data |= MASK_BYPASS_TDD;  // bypass TDD operation
    spi_out (&adrf6650_spi_dev, TDD_BYPASS_ADDR, data);
}


void set_D(int value)
{
    adrf6650_spi_dev->words = 3;
    value &= MASK_DOUBLER_EN >> SHIFT_DOUBLER_EN;  //limit to valid input
    uint8_t data = spi_in(&adrf6650_spi_dev, SYNTH_0_ADDR);
    data |= MASK_DOUBLER_EN & (value << SHIFT_DOUBLER_EN);
    spi_out (&adrf6650_spi_dev, SYNTH_0_ADDR, data);
}

void set_R(int value)
{
    adrf6650_spi_dev->words = 3;
    value &= MASK_R_DIV;  //limit to valid input
    uint8_t data = spi_in(&adrf6650_spi_dev, R_DIV_ADDR);
    data &= ~MASK_R_DIV;
    data |= MASK_R_DIV & value;
    spi_out (&adrf6650_spi_dev, R_DIV_ADDR, data);
}

void set_T(int value)
{
    adrf6650_spi_dev->words = 3;
    value &= MASK_RDIV2_SEL >> SHIFT_RDIV2_SEL;  //limit to valid input
    uint8_t data = spi_in(&adrf6650_spi_dev, SYNTH_0_ADDR);
    data |= MASK_RDIV2_SEL & (value << SHIFT_RDIV2_SEL);
    spi_out (&adrf6650_spi_dev, SYNTH_0_ADDR, data);
}

void set_frac1(int data)
{
    adrf6650_spi_dev->words = 3;
    spi_out(&adrf6650_spi_dev, FRAC1_L_ADDR, (uint8_t)(data & MASK_FRAC7_0));
    data >>= 8;
    spi_out(&adrf6650_spi_dev, FRAC1_M_ADDR, (uint8_t)(data & MASK_FRAC15_8));
    data >>= 8;
    spi_out(&adrf6650_spi_dev, FRAC1_H_ADDR, (uint8_t)(data & MASK_FRAC23_16));
}

void set_frac2(int data)
{
    adrf6650_spi_dev->words = 3;
    spi_out(&adrf6650_spi_dev, FRAC2_L_ADDR, (uint8_t)(data & MASK_FRAC27_0));
    data >>= 8;
    spi_out(&adrf6650_spi_dev, FRAC2_H_ADDR, (uint8_t)(data & MASK_FRAC213_8));
}

void set_mod(int data)
{
    adrf6650_spi_dev->words = 3;
    spi_out(&adrf6650_spi_dev, MOD_L_ADDR, (uint8_t)(data & MASK_MOD7_0));
    data >>= 8;
    spi_out(&adrf6650_spi_dev, MOD_H_ADDR, (uint8_t)(data & MASK_MOD13_8));
}

void set_int(int data)
{
    adrf6650_spi_dev->words = 3;
    spi_out(&adrf6650_spi_dev, INT_H_ADDR, (uint8_t)((data >> 8) & MASK_INT_DIV15_8));
    // spi_out(&adrf6650_spi_dev, INT_L_ADDR, (uint8_t)(data & MASK_INT_DIV7_0)); //PLL should autotune here
    //spi_out(&adrf6650_spi_dev, 0x000a,(uint8_t) 0x01); //Write scratchpad to provide additional SCLK pulses

    uint8_t addr1, addr0;
    uint8_t tx[6]; //handle terminating 0 in format string
    uint8_t rx[6];

    tx[1] = (uint8_t)(INT_L_ADDR & 0x00ff);
    tx[0] = (uint8_t)(INT_L_ADDR >> 8);
    tx[2] = (uint8_t)(data & MASK_INT_DIV7_0);
    tx[3] = 0x0a;     //dummy write to gen extra cycles
    tx[4] = tx[5] = 0;//dummy write to gen extra cycles

    adrf6650_spi_dev->words = 6;
    adrf6650_spi_dev->tx_buf = (unsigned long)&tx,
        adrf6650_spi_dev->rx_buf = (unsigned long)&rx,

        spi_out(&adrf6650_spi_dev, MOD_H_ADDR, (uint8_t)(data & MASK_MOD13_8));
}

void set_phase(int data)
{
    adrf6650_spi_dev->words = 3;
    spi_out(&adrf6650_spi_dev, SD_PHASE_L_0_ADDR, (uint8_t)(data & MASK_PHASE7_0));
    data >>= 8;
    spi_out(&adrf6650_spi_dev, SD_PHASE_M_0_ADDR, (uint8_t)(data & MASK_PHASE15_8));
    data >>= 8;
    spi_out(&adrf6650_spi_dev, SD_PHASE_H_0_ADDR, (uint8_t)(data & MASK_PHASE23_16));
}

void set_bleed_en (bool enable)
{
    adrf6650_spi_dev->words = 3;
    uint8_t data = spi_in (&adrf6650_spi_dev, MULTI_FUNC_SYNTH_CTL_022D_ADDR);
    if (enable)
    {
        data |= MASK_BLEED_EN;
        spi_out (&adrf6650_spi_dev, MULTI_FUNC_SYNTH_CTL_022D_ADDR, (uint8_t)(data)); 
    }
    else
    {
        data &= ~MASK_BLEED_EN;
        spi_out (&adrf6650_spi_dev, MULTI_FUNC_SYNTH_CTL_022D_ADDR, (uint8_t)(data)); 
    }
}

void set_bicp(int data)
{
    adrf6650_spi_dev->words = 3;
    data &= MASK_BICP;
    spi_out (&adrf6650_spi_dev, BICP_ADDR, (uint8_t)data);
}

// Should be '7' for 2.4ma cp output
void set_cp_current(int data)
{
    adrf6650_spi_dev->words = 3;
    data &= MASK_CP_CURRENT;
    spi_out (&adrf6650_spi_dev, CP_CURR_ADDR, (uint8_t)data);
}

void set_vco_freq (double frequency)
{
    adrf6650_spi_dev->words = 3;
    int Int, Frac1, Frac2, Mod;
    double N, N_fraction;
    double working_float;  //temp variable for chaining calculations
    double temp_int;  //modf 'integer' output is float
    //float Fpfd;

    // 
    //

    // Calculate INT
    N = frequency / PFD_FREQUENCY;  
    N_fraction = modf (N, &temp_int);
    Int = (int)temp_int;
    //printf ("INT=%d\n", Int); //TESTTESTTEST

    // Calculate FRAC1
    working_float = N_fraction * pow(2,24);
    working_float = modf (working_float, &temp_int);
    Frac1 = (int)temp_int;
    //printf ("FRAC1=%d\n", Frac1); //TESTTESTTEST

    // Calculate MOD
    //  1) calculate GCD of Fpfd and desired channel spacing
    //  2) MOD will equal Fpfd/GCD
    //  Assumes Fpfd and channel spacing are integers
    int n1 = PFD_FREQUENCY;
    int n2 = DESIRED_CHANNEL_SPACING;
    while(n1!=n2)
    {
        if(n1 > n2)
            n1 -= n2;
        else
            n2 -= n1;
    }
    // n1 and n2  now represent GCD
    Mod = (int)(PFD_FREQUENCY / n1);
    //printf ("MOD=%d\n", Mod); //TESTTESTTEST

    // Calculate Frac2
    Frac2 = (int)((N_fraction * pow(2,24)) - Frac1) * Mod;
    //printf ("FRAC2=%d\n", Frac2); //TESTTESTTEST

    // "After determing PLL values, set SD_EN_FRAC0 bit (Register 0x122A, Bit 4) to 1." NOTE: should be bit5
    uint8_t data = spi_in (&adrf6650_spi_dev, SD_CTRL_ADDR);
    data |= MASK_SD_EN_FRAC0;
    spi_out (&adrf6650_spi_dev, SD_CTRL_ADDR, data);

    set_frac2(Frac2);
    set_frac1(Frac1);
    set_mod(Mod);
    set_int(Int);
    //printf ("VCO Output = %f\n", frequency); //TESTTESTTEST
}

void set_ref_input ()
{
    adrf6650_spi_dev->words = 3;
    // Set up REF Input
    // Assumptions:
    //  1) REF=10MHz
    //  2) PFD=20MHz
    //  3) cp current = 2.4mA

    set_D(1);
    set_R(1);
    set_T(0);
    set_phase(0);
    set_cp_current(7);
    set_bicp (26);
    set_bleed_en(true);
}

void set_lo_freq (double frequency)
{
    uint8_t divisor;
    adrf6650_spi_dev->words = 3;

    // Determine divisor (Table 15 Data Sheet)
    if (frequency >= 450000000 && frequency <= 500000000)  //between 450 and 500 MHz inclusive
    {
        divisor = 0x10;
    }
    else if (frequency > 500000000 && frequency <= 1000000000)
    {
        divisor = 0x08;
    }
    else if (frequency > 1000000000 && frequency <= 2000000000)
    {
        divisor = 0x04;
    }
    else if (frequency > 2000000000 && frequency <= 2900000000)
    {
        divisor = 0x02;
    }
    else
    {
        //TODO: Error
    }

    //printf ("OUT DIV=%d\n", (int)divisor); //TESTTESTTEST

    // Set divisor
    uint8_t data = spi_in(&adrf6650_spi_dev, LO_CNTRL8_ADDR);
    data &= ~MASK_OUT_DIVRATIO;
    data |= (MASK_OUT_DIVRATIO & divisor);
    spi_out (&adrf6650_spi_dev, LO_CNTRL8_ADDR, data);

    // Set vco frequency
    set_vco_freq (frequency * divisor);
}

void set_mixer_en(bool enable)
{
    adrf6650_spi_dev->words = 3;
    uint8_t data = spi_in(&adrf6650_spi_dev, LO_CNTRL8_ADDR);
    if (true == enable)
    {
        data |= MASK_MIX_OE;
    }
    else
    {
        data &= ~MASK_MIX_OE;
    }

    spi_out (&adrf6650_spi_dev, LO_CNTRL8_ADDR, data);
}

void set_lo_out_en(bool enable)
{
    adrf6650_spi_dev->words = 3;
    uint8_t data = spi_in(&adrf6650_spi_dev, LO_CNTRL8_ADDR);
    if (true == enable)
    {
        data |= MASK_LO_OE;
    }
    else
    {
        data &= ~MASK_LO_OE;
    }

    spi_out (&adrf6650_spi_dev, LO_CNTRL8_ADDR, data);
}


void set_dvga_mode(dvga_mode_t mode)
{
    adrf6650_spi_dev->words = 3;
    uint8_t data = spi_in (&adrf6650_spi_dev, DVGA_MODE_ADDR);  //Addr 0x103
    if (DVGA_SPI_MODE == mode)
    {
        data |= 0x01;
        spi_out (&adrf6650_spi_dev, DVGA_MODE_ADDR, data); 
    }
    else if (DVGA_UP_DOWN_MODE == mode)
    {
        ; //will need to know up/down step size in this case
    }
    else
    {
        //TODO: error
    }

    // when modes are changed, a reset needs to be issued to
    // the dvgas
    data = spi_in (&adrf6650_spi_dev, BLOCK_RESETS_A_ADDR);
    data |= (MASK_DVGA_CH1_RSTB | MASK_DVGA_CH2_RSTB);
    spi_out (&adrf6650_spi_dev, BLOCK_RESETS_A_ADDR, data);
}

void set_dvga1_atten(int data)
{
    adrf6650_spi_dev->words = 3;
    if ((data <= DVGA_MAX_ATTEN) && (data >= 0))
    {
        spi_out (&adrf6650_spi_dev, DVGA_GAIN1_ADDR, (uint8_t) data);
    }
    else
    {
        //TODO: error: out of range

    }
}

void set_dvga2_atten(int data)
{
    adrf6650_spi_dev->words = 3;
    if ((data <= DVGA_MAX_ATTEN) && (data >= 0))
    {
        spi_out (&adrf6650_spi_dev, DVGA_GAIN2_ADDR, (uint8_t) data);
    }
    else
    {
        //TODO: error: out of range
    }
}



