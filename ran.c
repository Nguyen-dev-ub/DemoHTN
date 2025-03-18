#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#define spi0     0

uint8_t buff[2];

void send_data(uint8_t address, uint8_t data){
    buff[0] = address;
    buff[1] = data;
    wiringPiSPIDataRW(spi0, buff, 2);
}

void init_max7219(void){
    // decode mode: use code B for all digits -> 0x09FF
    send_data(0x09, 0xFF); // cau hinh 8 led sang
    // intensity: 70% --> 0x0A0B
    send_data(0x0A, 0xf); // muc sang
    // scan limit
    send_data(0x0B, 7); // hien thi 7 so
    // turn off display test, no shutdow
    send_data(0x0F, 0);
    send_data(0x0C, 1);
}

int main(void){
    wiringPiSPISetup(spi0, 60);
    while (1==1)
    {   
        int n= rand() %10000+1;
        sleep(1);
        int a=n/10000;
        int b=(n%10000)/1000;
        int c=(n%1000)/100;
        int d=(n%100)/10;
        int e=n%10;
        uint8_t s[8] = {0, 0, 0, a, b, c, d, e};
        for (int i = 0; i < 8; i++)
        {
            send_data(i + 1, s[7 - i]);
        }
    
    }
    return(0);
}