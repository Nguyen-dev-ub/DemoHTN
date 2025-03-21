#include <stdio.h>
#include <unistd.h>
#include <wiringPiSPI.h>
#include <stdint.h>  

#define spi0     0
int num;
uint8_t buff[2];

uint8_t letters[26][8] = {
    {0x1C,0x22,0x22,0x22,0x3E,0x22,0x22,0x22}, // A
    {0x3C,0x22,0x22,0x3C,0x22,0x22,0x22,0x3C}, // B
    {0x1C,0x22,0x20,0x20,0x20,0x20,0x22,0x1C}, // C
    {0x3C,0x22,0x22,0x22,0x22,0x22,0x22,0x3C}, // D
    {0x3E,0x20,0x20,0x3C,0x20,0x20,0x20,0x3E}, // E
    {0x3E,0x20,0x20,0x3C,0x20,0x20,0x20,0x20}, // F
    {0x1C,0x22,0x20,0x20,0x26,0x22,0x22,0x1C}, // G
    {0x22,0x22,0x22,0x3E,0x22,0x22,0x22,0x22}, // H
    {0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x3E}, // I
    {0x02,0x02,0x02,0x02,0x02,0x22,0x22,0x1C}, // J
    {0x22,0x24,0x28,0x30,0x28,0x24,0x22,0x22}, // K
    {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3E}, // L
    {0x22,0x36,0x2A,0x2A,0x22,0x22,0x22,0x22}, // M
    {0x22,0x22,0x32,0x2A,0x26,0x22,0x22,0x22}, // N
    {0x1C,0x22,0x22,0x22,0x22,0x22,0x22,0x1C}, // O
    {0x3C,0x22,0x22,0x3C,0x20,0x20,0x20,0x20}, // P
    {0x1C,0x22,0x22,0x22,0x2A,0x24,0x1A,0x02}, // Q
    {0x3C,0x22,0x22,0x3C,0x28,0x24,0x22,0x22}, // R
    {0x1C,0x22,0x20,0x1C,0x02,0x02,0x22,0x1C}, // S
    {0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08}, // T
    {0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x1C}, // U
    {0x22,0x22,0x22,0x22,0x22,0x22,0x14,0x08}, // V
    {0x22,0x22,0x22,0x2A,0x2A,0x36,0x22,0x22}, // W
    {0x22,0x22,0x14,0x08,0x08,0x14,0x22,0x22}, // X
    {0x22,0x22,0x14,0x08,0x08,0x08,0x08,0x08}, // Y
    {0x3E,0x02,0x04,0x08,0x10,0x20,0x20,0x3E}  // Z
};

uint8_t heartbit [8] = {0x66,0x99,0x81,0x81,0x81,0x42,0x24,0x18};

void send_data(uint8_t address, uint8_t data) {
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

void displayLetter(char ch) {
    if (ch < 'A' || ch > 'Z') return;  
    int index = ch - 'A';  
    for (int i = 0; i < 8; i++) {
        send_data(i + 1, letters[index][i]);
    }
}

void displayHeart(uint8_t heart[8]){
    for (int i = 0; i < 8; i++) {
        send_data(i + 1, heart[i]);
    }

}
int main(void) {
    while (1==1){

    wiringPiSPISetup(spi0, 4000000);
    init_max7219();

    // HIEN THI CAC CHU CAI TU A DEN J
    for (char ch = 'A'; ch <= 'Z'; ch++) 
    {  
        displayLetter(ch);  
        sleep(1); 
    }
    
    //======================================================================================================
    //NHAP CAC CHU CAI TU A DEN Z
    // char input
    // printf("NHAP MOT CHU CAI TU A-Z ( 0 DE HIEN THI TRAI TIM, X DE THOAT): ");
    // scanf(" %c", &input);  
    
    // if (input == 'X') {
    //     printf("Thoát chương trình...\n");
    //     break;
    // }

    // if (input == '0') {
    //     displayHeart(heartbit);
    //     printf("Hiển thị hình trái tim!\n");
    // } else {
    //     displayLetter(input);  // Hiển thị chữ cái lên LED ma trận
    // }
}
    return 0;
}
