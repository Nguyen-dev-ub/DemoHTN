#include <stdio.h>
#include <unistd.h>
#include <wiringPiSPI.h>
#include <stdint.h>  

#define spi0     0
int num;
uint8_t buff[2];

uint8_t digits[10][8] = {
    {0x1C,0x22,0x22,0x26,0x2A,0x32,0x22,0x1C}, // 0
    {0x08,0x18,0x28,0x08,0x08,0x08,0x08,0x3E}, // 1
    {0x1C,0x22,0x02,0x04,0x08,0x10,0x20,0x3E}, // 2
    {0x1C,0x22,0x02,0x1C,0x02,0x02,0x22,0x1C}, // 3
    {0x04,0x08,0x10,0x24,0x3E,0x04,0x04,0x04}, // 4
    {0x3E,0x20,0x20,0x3C,0x02,0x02,0x22,0x1C}, // 5
    {0x1C,0x20,0x20,0x1C,0x22,0x22,0x22,0x1C}, // 6
    {0x3E,0x02,0x04,0x04,0x08,0x08,0x08,0x08}, // 7
    {0x1C,0x22,0x22,0x1C,0x22,0x22,0x22,0x1C}, // 8
    {0x1C,0x22,0x22,0x1E,0x02,0x02,0x22,0x1C}  // 9
};
uint8_t heartbit [8] = {0x66,0x99,0x81,0x81,0x81,0x42,0x24,0x18};

void send_data(uint8_t address, uint8_t data){
    buff[0] = address;
    buff[1] = data;
    wiringPiSPIDataRW(spi0, buff, 2);
}


void init_max7219(void){
    // decode mode: use code B for all digits -> 0x09FF
    send_data(0x09, 0x00);
    // intensity: 70% --> 0x0A0B
    send_data(0x0A, 0x01
    );
    // scan limit
    send_data(0x0B, 7);
    // turn off display test, no shutdow
    send_data(0x0F, 0);
    send_data(0x0C, 1);
}

void displayDigit(int num) {
    if (num < 0 || num > 9) return;
    for (int i = 0; i < 8; i++) {
        send_data(i + 1, digits[num][i]);
    }
}

void displayHeart(uint8_t heart[8]){
    for (int i = 0; i < 8; i++) {
        send_data(i + 1, heart[i]);
    }

}

int main(void){
    while(1==1){

    wiringPiSPISetup(spi0, 4000000);
    init_max7219();
    //for(int i=0; i<=9; i++){
    //    displayDigit(i);
    //    sleep(1);   
    //}
    //}
    printf("Nhap so bat ky tu 0 den 9 (hoac -1 de thoat): ");
        scanf("%d", &num);

        if (num == -1) {
            displayHeart(heartbit);
            printf("Thoat chuong trinh..\n");
            break;
        }

        if (num >= 0 && num <= 9) {
            displayDigit(num);
        } else {
            printf("Vui lòng nhập số hợp lệ!\n");
        }
    }
    return 0;
}