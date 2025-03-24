#include <stdio.h>
#include <unistd.h>
#include <wiringPiSPI.h>
#include <stdint.h>  

#define spi0     0

uint8_t buff[2];

void send_data(uint8_t address, uint8_t data){
    buff[0] = address;
    buff[1] = data;
    wiringPiSPIDataRW(spi0, buff, 2);
}

void send_number_with_dot(uint8_t position, uint8_t num) {
    send_data(position, number[num] & 0x7F | 0x80); // Bật DP cho số tương ứng
}


void init_max7219(void){
    // decode mode: use code B for all digits -> 0x09FF
    send_data(0x09, 0xFF);
    // intensity: 70% --> 0x0A0B
    send_data(0x0A, 0x01);
    // scan limit
    send_data(0x0B, 7);
    // turn off display test, no shutdow
    send_data(0x0F, 0);
    send_data(0x0C, 1);
}

int main(void){
    printf("Load SPI driver, speed = 9MHz \n");
    wiringPiSPISetup(spi0, 4000000);
    printf("Setup ma7219\n");
    init_max7219();
    printf("display student number \n");
    // 22146089
    // uint8_t mssv[8]={2,2,3,4,6,2,2,1};
    // for(int i=0; i<8; i++){
    //     send_number_with_dot(i+1,mssv[7-i]);    
    // }

    send_data(7,so[5]);  

    return 0;
}