#include <stdio.h>
#include <unistd.h>
#include <wiringPiSPI.h>
#include <stdint.h>  

#define spi0     0
int num;
uint8_t buff[2];

const uint8_t font[36][8] = {
    // Số 0-9
    {0b00111100, 
     0b01100110, 
     0b01101110, 
     0b01110110, 
     0b01100110, 
     0b01100110, 
     0b00111100, 
     0b00000000},
    
    
    // Chữ A-Z
    {0b01111100, 0b10000010, 0b10000010, 0b10000010, 0b11111110, 0b10000010, 0b10000010, 0b00000000}, // A
    {0b11111100, 0b10000010, 0b10000010, 0b11111100, 0b10000010, 0b10000010, 0b11111100, 0b00000000}, // B
    {0b01111100, 0b10000010, 0b10000000, 0b10000000, 0b10000000, 0b10000010, 0b01111100, 0b00000000}, // C
};

uint8_t heartbit [8] = {0x66,0x99,0x81,0x81,0x81,0x42,0x24,0x18};

void send_data(uint8_t address, uint8_t data){
    buff[0] = address;
    buff[1] = data;
    wiringPiSPIDataRW(spi0, buff, 2);
}
void displayChar(char c) {
    int index;
    
    if (c >= '0' && c <= '9') {
        index = c - '0';  // Ví dụ '3' - '0' = 3 (vị trí trong mảng font)
    } 
    else if (c >= 'A' && c <= 'Z') {
        index = c - 'A' + 10;  // A = 10, B = 11, C = 12, ...
    } 
    else {
        printf("Ký tự không hỗ trợ!\n");
        return;
    }

    // Hiển thị trên LED matrix
    for (int i = 0; i < 8; i++) {
        send_data(i + 1, font[index][i]);  // Gửi dữ liệu từng hàng
    }
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


int main(void) {
    wiringPiSPISetup(spi0, 4000000);
    init_max7219();

    char input;
    
    while (1) {
        printf("Nhập ký tự (A-Z, 0-9) hoặc 'X' để thoát: ");
        scanf(" %c", &input);

        if (input == 'X') {
            printf("Thoát chương trình...\n");
            break;
        }

        displayChar(input);
    }

    return 0;
}
