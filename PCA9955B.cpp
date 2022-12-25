#include "PCA9955B.h"
// #define PRINTBIN(Num) for (uint32_t t = (1UL<< (sizeof(Num)*8)-1); t; t >>= 1) Serial.write(Num  & t ? '1' : '0'); // Prints a binary number with leading zeros (Automatic Handling)

PCA9955B::PCA9955B(unsigned char address)
{
  Wire.begin(21,22);
  addr = address;
  // if (display_type==LARGE)
  //   Seg2LED = Seg2LED_large;
  // else if (display_type==SMALL)
  //   Seg2LED = Seg2LED_small;
  mode(SOLID);
  write_byte(PCA_PWMALL, 0xFF);
  write_byte(PCA_IREFALL, 0x16); // Imax = 10mA @ Rext = 1k
  write_byte(PCA_MODE1,0b00000001);
  write_byte(PCA_MODE2,0b00000101);
}

void PCA9955B::mode(unsigned char nmode)
{
  offon[1] = nmode;
}

void PCA9955B::write_byte(unsigned char regbyte, unsigned char databyte)
{
  Wire.beginTransmission(addr); // transmit to device #addr
  Wire.write(byte(regbyte));            // sends instruction byte
  Wire.write(byte(databyte));             // sends value byte
  Wire.endTransmission();     // stop transmitting
}

void PCA9955B::write_LS()
{
  Wire.beginTransmission(addr); // transmit to device #addr
  Wire.write(byte(PCA_LEDOUT0 | PCA_AI_ALL)); // sends instruction byte including auto increment set
  Wire.write(byte(LS.LSbyte[0])); // sends value bytes
  Wire.write(byte(LS.LSbyte[1]));
  Wire.write(byte(LS.LSbyte[2]));
  Wire.write(byte(LS.LSbyte[3]));
  Wire.endTransmission();     // stop transmitting
}

void PCA9955B::write_PWMALL(unsigned char value)
{
  Wire.beginTransmission(addr); // transmit to device #addr
  Wire.write(byte(PCA_PWMALL)); // sends instruction byte including auto increment set
  Wire.write(byte(value)); // sends value bytes
  Wire.endTransmission();     // stop transmitting
}

void PCA9955B::write_IREFALL(unsigned char value)
{
  Wire.beginTransmission(addr); // transmit to device #addr
  Wire.write(byte(PCA_IREFALL)); // sends instruction byte including auto increment set
  Wire.write(byte(value)); // sends value bytes
  Wire.endTransmission();     // stop transmitting
}

unsigned char PCA9955B::read_byte(unsigned char regbyte)
{
  Wire.beginTransmission(addr);
  Wire.write(byte(regbyte));
  Wire.endTransmission(false);
  Wire.requestFrom(addr, (byte)1);
  unsigned char databyte = Wire.read();
  return databyte;
}

// Function set_bit sets a bit in a number pointed to:
// - The number to be worked on is represented by a pointer to it, type is unsigned int*
// - The bit to be changed is given by n, tyoe is an unsigned char; LSB is 0
// - The target state of the bit is x, type is boolean
void PCA9955B::set_bit(unsigned int *number, unsigned char n, bool x)
{
  unsigned int newbit = !!x;    // Booleanize to force 0 or 1
  *number ^= (-newbit ^ *number) & (1UL << n);
}

void PCA9955B::set_leds(unsigned short leds)
{
  // Serial.println(bitRead(leds,0));
  // Serial.println(offon[1]);

  for (int n=0; n<16; n++)
  {
    set_bit(&LS.LS, 2*n, offon[bitRead(leds,n)] & 1);
    set_bit(&LS.LS, 2*n+1, offon[bitRead(leds,n)] & 2);
  }
  // PRINTBIN(LS.LS);
  // Serial.println();
  write_LS();
}

// void PCA9955B::set(unsigned char number, unsigned char decimal)
// {
//   set_segments(Numbers[number]+decimal);
// }

void PCA9955B::clear()
{
  set_leds(0);
}

void PCA9955B::dutycycle(unsigned char ontime)
{
  if (offon[1]==PWM0)
    write_byte(PCA_PWMALL, ontime);
  else if (offon[1]==PWM1)
    write_byte(PCA_GRPPWM, ontime);
}

void PCA9955B::dutycycle(unsigned char nled, unsigned char ontime)
{
  write_byte(PCA_PWMX_OFFSET + nled, ontime);
}

void PCA9955B::period(unsigned char prescaler)
{
    write_byte(PCA_GRPFREQ, prescaler);
}

void PCA9955B::set_allcall(unsigned char allcalladr)
{
  write_byte(PCA_ALLCALLADR, allcalladr);
  write_byte(PCA_MODE1, read_byte(PCA_MODE1) | 0x01);
}

void PCA9955B::clear_allcall()
{
  write_byte(PCA_MODE1, read_byte(PCA_MODE1) & 0xFE);
}

unsigned char PCA9955B::get_addr()
{
  return addr;
}
