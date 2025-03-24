#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
// - tần số lấy mẫu > 500 Hz
// - Mở low pass filter 94 Hz
// - Khoảng đo Acc là +- 2g, khoảng đo Gyro là +-1000 độ/s
#define Sample_rate     25  // 0x19
#define Config          26  // 0x1A
#define Gyro_config     27  // 0x1B
#define Acc_config      28  // 0x1C          
#define Temp            65  // 0x41

int mpu;
int16_t read_temp(unsigned char sensor)
{
    int16_t high, low, data;
    high = wiringPiI2CReadReg8 (mpu,0x41);
    low  =  wiringPiI2CReadReg8 (mpu,0x42);
    data = (high<<8) | low;
    return data;
}

void Init(void)
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
int main(void)
{
    //setup giao tiep I2C
    int mpu;
    mpu = wiringPiI2CSetup(0x68); //chân ID0 mức 0 là ox68
    // thiết lập chết độ cho mpu6050
    Init_6050();
    while(1)
    {
    // đọc giá trị đo nhiệt độ
    
    printf(....);
    delay(1000);
    }

}