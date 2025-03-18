#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>


int main(void)
{
    //setup giao tiep I2C
    int ds = wiringPiI2CSetup(0x68)
}