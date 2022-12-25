#include "Arduino.h"
#include <Wire.h>

// Register addresses:
#define PCA_MODE1 0x00
#define PCA_MODE2 0x01
#define PCA_LEDOUT0 0x02
#define PCA_LEDOUT1 0x03
#define PCA_LEDOUT2 0x04
#define PCA_LEDOUT3 0x05
#define PCA_GRPPWM 0x06
#define PCA_GRPFREQ 0x07

#define PCA_PWMX_OFFSET 0x08  // PWMX from 0x08 to 0x17

#define PCA_IREFX_OFFSET 0x18 // IREFX from 0x18 to 0x27

#define PCA_RAMP_RATE_GRP0 0x28
#define PCA_STEP_TIME_GRP0 0x29
#define PCA_HOLD_CNTL_GRP0 0x2A
#define PCA_IREF_GRP0 0x2B
#define PCA_RAMP_RATE_GRP1 0x2C
#define PCA_STEP_TIME_GRP1 0x2D
#define PCA_HOLD_CNTL_GRP1 0x2E
#define PCA_IREF_GRP1 0x2F
#define PCA_RAMP_RATE_GRP2 0x30
#define PCA_STEP_TIME_GRP2 0x31
#define PCA_HOLD_CNTL_GRP2 0x32
#define PCA_IREF_GRP2 0x33
#define PCA_RAMP_RATE_GRP3 0x34
#define PCA_STEP_TIME_GRP3 0x35
#define PCA_HOLD_CNTL_GRP3 0x36
#define PCA_IREF_GRP3 0x37

#define PCA_GRAD_MODE_SEL0 0x38
#define PCA_GRAD_MODE_SEL1 0x39
#define PCA_GRAD_GRP_SEL0 0x3A
#define PCA_GRAD_GRP_SEL1 0x3B
#define PCA_GRAD_GRP_SEL2 0x3C
#define PCA_GRAD_GRP_SEL3 0x3D
#define PCA_GRAD_CNTL 0x3E

#define PCA_OFFSET 0x3F
#define PCA_SUBADR1 0x40
#define PCA_SUBADR2 0x41
#define PCA_SUBADR3 0x42
#define PCA_ALLCALLADR 0x43

#define PCA_PWMALL 0x44
#define PCA_IREFALL 0x45
#define PCA_EFLAG0 0x46
#define PCA_EFLAG1 0x47
#define PCA_EFLAG2 0x48
#define PCA_EFLAG3 0x49
// 0x4A to 0x7F reserved

// Auto increment bit masks:
#define PCA_AI_NONE 0b00000000
#define PCA_AI_ALL 0b10000000
#define PCA_AI_PWM 0b10100000
#define PCA_AI_GRP 0b11000000
#define PCA_AI_PWM_GRP 0b11100000

class PCA9955B
{
  private:
    unsigned char addr;
    //const unsigned short modeon[3] = {0b01, 0b10, 0b11};
    unsigned short offon[2] = {0b00, 0b01};  // {OFF, ON}
    union LStype
    {
       unsigned char LSbyte[4];
       unsigned int LS;
    } LS;
    unsigned char read_byte(unsigned char regbyte);
    void set_bit(unsigned int *number, unsigned char n, bool x);
    void write_byte(unsigned char regbyte, unsigned char databyte);
    void write_LS();
    void write_PWMALL(unsigned char value);
    void write_IREFALL(unsigned char value);
  public:
    static unsigned const char SOLID=0b01, PWM0=0b10, PWM1=0b11;
    // PCA9955B(unsigned char address, unsigned char display_type);
    PCA9955B(unsigned char address);
    void mode(unsigned char nmode);
    void set_leds(unsigned short leds);
    // void set(unsigned char number, unsigned char decimal);
    void clear();
    void dutycycle(unsigned char ontime);
    void dutycycle(unsigned char nled, unsigned char ontime);
    void period(unsigned char prescaler);
    void set_allcall(unsigned char allcalladr);
    void clear_allcall();
    unsigned char get_addr();
};
