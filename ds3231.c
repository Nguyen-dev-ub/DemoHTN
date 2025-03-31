#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include <wiringPiSPI.h>

#define DS3231 0x68
#define SPI0   0

uint8_t buff[2];

void send_data(uint8_t address, uint8_t data) {
    buff[0] = address;
    buff[1] = data;
    wiringPiSPIDataRW(SPI0, buff, 2);
}

void init_max7219(void) {
    send_data(0x09, 0xFF);  // No decode mode
    send_data(0x0A, 0x01);  // Intensity (brightness)
    send_data(0x0B, 7);     // Scan limit (all 8 digits)
    send_data(0x0C, 1);     // Shutdown mode off
    send_data(0x0F, 0);     // Display test off
}

void display_time(int hh, int mm, int ss) {
    send_data(1, ss % 10);
    send_data(2, ss / 10);
    send_data(3, 0x0A);  // Dấu '-'

    send_data(4, mm % 10);
    send_data(5, mm / 10);
    send_data(6, 0x0A);  // Dấu '-'

    send_data(7, hh % 10);
    send_data(8, hh / 10);
}

int decToBcd(int val) {
    return ((val / 10 * 16) + (val % 10));
}

int bcdToDec(int val) {
    return ((val / 16 * 10) + (val % 16));
}

void set_time(int ds) {
    int hh, mm, ss;
    printf("Nhập giờ (0-23): ");
    scanf("%d", &hh);
    printf("Nhập phút (0-59): ");
    scanf("%d", &mm);
    printf("Nhập giây (0-59): ");
    scanf("%d", &ss);
    
    wiringPiI2CWriteReg8(ds, 0x00, decToBcd(ss));
    wiringPiI2CWriteReg8(ds, 0x01, decToBcd(mm));
    wiringPiI2CWriteReg8(ds, 0x02, decToBcd(hh));
}

void read_time(int ds, int *hh, int *mm, int *ss) {
    *ss = bcdToDec(wiringPiI2CReadReg8(ds, 0x00));
    *mm = bcdToDec(wiringPiI2CReadReg8(ds, 0x01));
    *hh = bcdToDec(wiringPiI2CReadReg8(ds, 0x02));
}

int main() {
    wiringPiSetup();
    int ds = wiringPiI2CSetup(DS3231);
    wiringPiSPISetup(SPI0, 1000000);
    init_max7219();
    
    set_time(ds);

    while (1) {
        int hh, mm, ss;
        read_time(ds, &hh, &mm, &ss);
        display_time(hh, mm, ss);
        printf("Thời gian: %02d-%02d-%02d\n", hh, mm, ss);
        delay(1000);
    }
    return 0;
}
