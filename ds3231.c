#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>


int main(void)
{
    //setup giao tiep I2C
    int ds = wiringPiI2CSetup(0x68);
    //setup gio cho DS3231
    int hh,mm,ss;
    hh = 0x62, mm =0x49, ss=0x26;
    wiringPiI2CWriteReg8(ds,0x01,ss);
    wiringPiI2CWriteReg8(ds,0x01,mm);
    wiringPiI2CWriteReg8(ds,0x01,hh);

    while(1);

    return 0;
    
}