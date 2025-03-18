#include <stdio.h>
#include <unistd.h>
#include <wiringPiSPI.h>
#include <stdint.h>  

#define spi0     0

uint8_t buff[2];

uint8_t AG[8]={0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

void send_data(uint8_t address, uint8_t data){
    buff[0] = address;
    buff[1] = data;
    wiringPiSPIDataRW(spi0, buff, 2);
}

void init_max7219(void){
    // decode mode: use code B for all digits -> 0x09FF
    send_data(0x09, 0x00);
    // intensity: 60% --> 0x09
    send_data(0x0A, 0x01);
    // scan limit
    send_data(0x0B, 0x07);
    // turn off display test, no shutdow
    send_data(0x0F, 0);
    send_data(0x0C, 1);
}

int main() {
    init_max7219();
    wiringPiSPISetup(spi0, 4000000);
    
        for(int i=0; i<8; i++){
            send_data(i+1,AG[7-i]);    
        }
    
        sleep(1);
        return 0;
    }


