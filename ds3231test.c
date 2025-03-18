#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

// Địa chỉ I2C của DS3231
#define DS3231_ADDR  0x68  

// Hàm chuyển đổi số hệ thập phân sang BCD
int decToBcd(int val) {
    return ((val / 10 * 16) + (val % 10));
}

// Hàm chuyển đổi số BCD sang thập phân
int bcdToDec(int val) {
    return ((val / 16 * 10) + (val % 16));
}

int main(void) {
    // Khởi tạo I2C
    int ds3231 = wiringPiI2CSetup(DS3231_ADDR);
    if (ds3231 == -1) {
        printf("Không thể kết nối với DS3231\n");
        return -1;
    }

    // Nhập thời gian từ bàn phím
    int hh, mm, ss;
    printf("Nhập giờ (0-23): ");
    scanf("%d", &hh);
    printf("Nhập phút (0-59): ");
    scanf("%d", &mm);
    printf("Nhập giây (0-59): ");
    scanf("%d", &ss);

    // Ghi thời gian vào DS3231 (dùng BCD)
    wiringPiI2CWriteReg8(ds3231, 0x00, decToBcd(ss)); // Thanh ghi giây
    wiringPiI2CWriteReg8(ds3231, 0x01, decToBcd(mm)); // Thanh ghi phút
    wiringPiI2CWriteReg8(ds3231, 0x02, decToBcd(hh)); // Thanh ghi giờ

    printf("Thời gian đã được thiết lập!\n");

    // Đọc và hiển thị thời gian mỗi giây
    while (1) {
        ss = bcdToDec(wiringPiI2CReadReg8(ds3231, 0x00)); // Đọc giây
        mm = bcdToDec(wiringPiI2CReadReg8(ds3231, 0x01)); // Đọc phút
        hh = bcdToDec(wiringPiI2CReadReg8(ds3231, 0x02)); // Đọc giờ

        printf("Thời gian: %02d:%02d:%02d\n", hh, mm, ss);
        sleep(1); // Đợi 1 giây
    }

    return 0;
}
