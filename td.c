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
unsigned char number_map[10] = { 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B  };
void display_score(int teamA, int teamB, int set) {
    send_data(8, 0x0);
    send_data(7, number_map[teamA / 10]); // Chục của đội A
    send_data(6, number_map[teamA % 10]); // Đơn vị của đội A
    send_data(5, 0x01);                   // Dấu "-" (hiển thị LED A)
    send_data(4, number_map[teamB / 10]); // Chục của đội B
    send_data(3, number_map[teamB % 10]); // Đơn vị của đội B
    send_data(2, 0x0);                   // Khoảng trống
    send_data(1, number_map[set]); }       // Hiệp đấu
int main(void){
    wiringPiSPISetup(spi0, 4000000);
    init_max7219();

    int teamA = 12, teamB = 8, set = 2;
    display_score(teamA, teamB, set);

    return 0;
}