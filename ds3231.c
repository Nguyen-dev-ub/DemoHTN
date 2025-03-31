#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <wiringPiSPI.h>
#define ds3231 0x68
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

// Hiển thị thời gian lên MAX7219
void display_time(int hh, int mm, int ss) {
    send_data(8, hh / 10);     // Giờ hàng chục
    send_data(7, hh % 10);     // Giờ hàng đơn vị
    send_data(6, 0x01);        // Dấu '-' (giá trị 0x0A trong MAX7219 là '-')
    
    send_data(5, mm / 10);     // Phút hàng chục
    send_data(4, mm % 10);     // Phút hàng đơn vị
    send_data(3, 0x01);        // Dấu '-'

    send_data(2, ss / 10);     // Giây hàng chục
    send_data(1, ss % 10);     // Giây hàng đơn vị
}
void set_time() {
    int hh, mm, ss;
    
    printf("Nhập giờ (0-23): ");
    scanf("%d", &hh);
    printf("Nhập phút (0-59): ");
    scanf("%d", &mm);
    printf("Nhập giây (0-59): ");
    scanf("%d", &ss);
    
    wiringPiI2CWriteReg8(ds3231, 0x00, decToBcd(ss)); // Ghi giây
    wiringPiI2CWriteReg8(ds3231, 0x01, decToBcd(mm)); // Ghi phút
    wiringPiI2CWriteReg8(ds3231, 0x02, decToBcd(hh)); // Ghi giờ
}

// Đọc thời gian từ DS3231
void read_time(int *hh, int *mm, int *ss) {
    *ss = bcdToDec(wiringPiI2CReadReg8(ds3231, 0x00));
    *mm = bcdToDec(wiringPiI2CReadReg8(ds3231, 0x01));
    *hh = bcdToDec(wiringPiI2CReadReg8(ds3231, 0x02));
}

int main() {
    wiringPiSetup();
    int ds = wiringPiI2CSetup(ds3231);
    wiringPiSPISetup(spi0, 40000);
    init_max7219();
    set_time();

    while (1) {
        int hh, mm, ss;
        read_time(&hh, &mm, &ss);
        display_time(hh, mm, ss);
        
        printf("Thời gian: %02d-%02d-%02d\n", hh, mm, ss);
        delay(1000);
    }

    return 0;
}
