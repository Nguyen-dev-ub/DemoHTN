#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <wiringPiSPI.h>

// - tần số lấy mẫu > 500 Hz
// - Mở low pass filter 94 Hz
// - Khoảng đo Acc là +- 2g, khoảng đo Gyro là +-1000 độ/s
#define Sample_rate     25  // 0x19
#define Config          26  // 0x1A
#define Gyro_config     27  // 0x1B
#define Acc_config      28  // 0x1C          
#define Temp            65  // 0x41
#define spi0            0
#define NUM_SAMPLES     5

uint8_t buff[2];
int mpu;

float read_temperature() {
    int16_t high = wiringPiI2CReadReg8(mpu, Temp);   // Đọc byte cao
    int16_t low  = wiringPiI2CReadReg8(mpu, Temp+1); // Đọc byte thấp
    int16_t temp_raw = (high << 8) | low;  // Ghép 2 byte lại

    return temp_raw / 340.0 + 36.53;  // Chuyển đổi sang °C
}

float read_temperature_filtered() {
    float sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        sum += read_temperature();
        delay(10);  // Đợi một chút để tránh đọc quá nhanh
    }
    return sum / NUM_SAMPLES;  // Lấy trung bình
}


void send_data(uint8_t address, uint8_t data) {
    buff[0] = address;
    buff[1] = data;
    wiringPiSPIDataRW(spi0, buff, 2);
}

void Init_6050(void)
    {
    // sample rate 500Hz, cách tính sample rate = 8000/(1+X)=500 => x=15
    // trong datasheet, gyro output rate =8kHz
    // khi DLPF =(0 or 7) (do bộ lọc xung tắt)
    // khi DLPF =1 (do bộ lọc xung bật) đọc Reg 26
    wiringPiI2CWriteReg8(mpu, Sample_rate,15);
    //không sử dụng nguồn xung ngoài, tắt bộ lọc số (DLPf) 
    wiringPiI2CWriteReg8(mpu,Config,0x02); //0x00000010
    //gyro FS: +-1000 o/s
    wiringPiI2CWriteReg8(mpu,Gyro_config,0x16); // 00010000
    // acc FS: +-2g
    wiringPiI2CWriteReg8(mpu,Acc_config,0x00); //00000000
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
    void display_number(float num) {
        int integer_part = (int)num;  // Phần nguyên
        int decimal_part = (int)(num * 100) % 100; // Lấy 2 số thập phân
    
        uint8_t digits[8] = {0};
        digits[7] = integer_part / 10; // Hàng chục
        digits[6] = integer_part % 10 | 0x80; // Hàng đơn vị (có dấu chấm thập phân)
        digits[5] = decimal_part / 10; // Phần thập phân hàng chục
        digits[4] = decimal_part % 10; // Phần thập phân hàng đơn vị
    
        for (int i = 0; i < 8; i++) {
            send_data(i + 1, digits[i]);
        }
    }

int main(void)
{
    //setup giao tiep I2C
    wiringPiSetupPhys();
    wiringPiSetup();
    mpu = wiringPiI2CSetup(0x68); //chân ID0 mức 0 là ox68
    wiringPiSPISetup(spi0, 4000000);
    // thiết lập chết độ cho mpu60504
    Init_6050();
    init_max7219();

    while(1)
    {
    // đọc giá trị đo nhiệt độ
    float temp = read_temperature_filtered();
    display_number(temp);
    printf("Temperature: %.2f°C\n", temp);
    delay(1000);
    }

}