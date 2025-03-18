#include <stdio.h>
#include <unistd.h>
#include <wiringPiSPI.h>
#include <stdint.h>  

#define spi0     0
int num;
uint8_t buff[2];


uint8_t ngang [8][8] = {
    {0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF}
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
    send_data(0x0A, 0x01);
    // scan limit
    send_data(0x0B, 7);
    // turn off display test, no shutdow
    send_data(0x0F, 0);
    send_data(0x0C, 1);
}


void displayNgang(int row[],int size) {
    uint8_t rowState[8] = {0};
    for (int i = 0; i < size; i++) {
        if (row[i] >= 1 && row[i] <= 8) {
            rowState[row[i] - 1] = 0xFF; 
        }
    }

    // Gửi dữ liệu lên LED
    for (int i = 0; i < 8; i++) {
        send_data(i + 1, rowState[i]);
    }
}

void displayHeart(uint8_t heart[8])
{
    for (int i = 0; i < 8; i++) {
        send_data(i + 1, heart[i]);
    }
}
int main(void){
    wiringPiSPISetup(spi0, 4000000);
    init_max7219();
    while (1==1) {

        //======================================================================================================
        //NHAP NHIEU SO DE HIEN THI HANG TUONG UNG
        printf("Nhập số hàng cần sáng (cách nhau bởi dấu cách, nhập -1 để thoát): ");
        int row[8];  // Lưu tối đa 8 số
        int count = 0;
        while (count < 8) {
            int temp;
            if (scanf("%d", &temp) != 1) break; // Dừng nếu không nhập số
            if (temp == -1) {
                printf("Thoát chương trình...\n");
                return 0;
            }
            row[count++] = temp;
            if (getchar() == '\n') break; // Dừng khi nhấn Enter
        }
        displayNgang(row, count);

        //======================================================================================================
        // NHAP CAC SO TU 1 DEN 8 DE SANG CAC HANG TUONG UNG
        // printf("Nhap so bat ky tu 1 den 8 (hoac -1 de thoat): ");
        // scanf("%d", &num);

        // if (num == -1) {
        //     displayHeart(heartbit);
        //     printf("Thoat chuong trinh..\n");
        //     break;
        // }

        // if (num >= 1 && num <= 8) {
        //     displayNgang(num);
        // } else {
        //     printf("Nhap so hop le tu 1 den 8 lai cho tao!\n");
        // }
    }

}