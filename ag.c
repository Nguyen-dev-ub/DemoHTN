//sang lan luot cac doan led tu a den g 
#include <stdio.h>
#include <unistd.h>
#include <wiringPiSPI.h>
#include <stdint.h>  

#define spi0 0

uint8_t buff[2];

// Dữ liệu để bật lần lượt các đoạn từ A đến G (mô phỏng 7 thanh LED)
uint8_t AG[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00};

void send_data(uint8_t address, uint8_t data) {
    buff[0] = address;
    buff[1] = data;
    wiringPiSPIDataRW(spi0, buff, 2);
}

void init_max7219(void) {
    send_data(0x09, 0x00);  // Không dùng chế độ decode BCD
    send_data(0x0A, 0x0F);  // Độ sáng tối đa
    send_data(0x0B, 0x07);  // Hiển thị cả 8 hàng
    send_data(0x0F, 0x00);  // Tắt chế độ test
    send_data(0x0C, 0x01);  // Bật hiển thị
}

void clear_max7219(void) {
    for (int i = 1; i <= 8; i++) {
        send_data(i, 0x00);  // Tắt tất cả các hàng
    }
}

int main() {
    wiringPiSPISetup(spi0, 4000000);
    init_max7219();

    // Bật lần lượt từng đoạn từ A đến G
    for (int i = 0; i < 7; i++) {
        send_data(1, AG[i]);
        send_data(2, AG[i]);
        send_data(3, AG[i]);
        send_data(4, AG[i]);
        send_data(5, AG[i]);
        send_data(6, AG[i]);
        send_data(7, AG[i]);
        send_data(8, AG[i]); 
        usleep(500000);      
    }

    // Giữ trạng thái sáng trong 2 giây
    sleep(2);

    // Tắt tất cả LED
    clear_max7219();

    return 0;
}