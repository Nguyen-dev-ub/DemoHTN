#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <wiringPiSPI.h>
#include <softPwm.h>

#define spi0 0
#define red 11
#define green 12
#define blue 13
#define bt1 15
#define bt2 22
#define bt3 18
#define bt4 16
#define PWM_MAX 100

#define M_PI 3.14159265358979323846
#define Sample_rate     25  // 0x19
#define Config          26  // 0x1A
#define Gyro_config     27  // 0x1B
#define Acc_config      28  // 0x1C          
#define Interrupt       56  // 0x38
#define PWR_Managment   107 // 0x6B
#define Acc_X           59  // 0x3B
#define Acc_Y           61  // 0x3D
#define Acc_Z           63  // 0x3F
#define spi0            0

uint8_t buff[2];
int mpu;

uint8_t digit_codes[10] = {
    0b01111110, // 0
    0b00110000, // 1
    0b01101101, // 2
    0b01111001, // 3
    0b00110011, // 4
    0b01011011, // 5
    0b01011111, // 6
    0b01110000, // 7
    0b01111111, // 8
    0b01111011  // 9
};

int16_t read_sensor(unsigned char sensor)
{
    int16_t high, low, data;
    high = wiringPiI2CReadReg8 (mpu,sensor);
    low  =  wiringPiI2CReadReg8 (mpu,sensor +1);
    data = (high<<8) | low;
    return data;
}

void send_data(uint8_t address, uint8_t data) {
    buff[0] = address;
    buff[1] = data;
    wiringPiSPIDataRW(spi0, buff, 2);
}
//Mpu 6050
void Init_6050(void)
    {
    // register 25 ->28 ,56,107 cấu hình thanh ghi của mpu 6050.tổng 6 thanh ghi
    // sample rate 400Hz, cách tính sample rate = 8000/(1+X)=400 => x=19
    // trong datasheet, gyro output 
    // rate =8kHz khi DLPF =(0 or 7) (do bộ lọc xung tắt)
    // rate =1kHz khi DLPF =1 (do bộ lọc xung bật) đọc Reg 26
    wiringPiI2CWriteReg8(mpu,Sample_rate,19 );
    //không sử dụng nguồn xung ngoài, tắt bộ lọc số (DLPf) 
    wiringPiI2CWriteReg8(mpu,Config,0);
    //gyro FS: +-2000 o/s
    wiringPiI2CWriteReg8(mpu,Gyro_config,0x24); // 00011000
    // acc FS: +-4g
    wiringPiI2CWriteReg8(mpu,Acc_config,0x10); //00001000
    // mở interrupt của data ready
    wiringPiI2CWriteReg8(mpu,Interrupt,1);
    // chọn nguồn xung Gyro X
    wiringPiI2CWriteReg8(mpu,PWR_Managment,0x01);
    }
//Max 7219
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

void display_digit(int pos, int num) {
    if (num < 0 || num > 9) return;
    send_data(pos, digit_codes[num]);
    }

void display_values( float Az) {

    int negative = (Az < 0); // Kiểm tra xem số có âm không
    if (negative) Az = -Az;  // Chuyển thành số dương để xử lý

    int integer_part = (int)Az;  // Phần nguyên
    int decimal_part = (int)(Az * 100) % 100; // Lấy 2 số thập phân
    
    int az_int = (int)(Az * 10);
    int az_frac = (int)(Az * 100) % 100;    
    int d1 = az_int / 10;   // Chữ số hàng chục
    int d2 = az_int % 10;   // Chữ số hàng đơn vị
    int d3 = az_frac / 10;  // Chữ số thập phân đầu tiên
    int d4 = az_frac % 10;  // Chữ số thập phân thứ hai

    if (negative) {
        send_data(8,0b00001000);
        send_data(7, 0b00000001);         // Hiển thị dấu '-' ở vị trí đầu
        send_data(6, digit_codes[d1]); // Hàng chục
        send_data(5, digit_codes[d2] | 0b10000000); // Hàng đơn vị + dấu chấm
        send_data(4, digit_codes[d3]); // Số thập phân đầu tiên
        send_data(3, digit_codes[d4]); // Số thập phân thứ hai
        send_data(2, 0b00011101); // Số thập phân thứ hai
        send_data(1, 0b00001000); // Số thập phân thứ hai
    } else {
        send_data(8,0b00001000);
        send_data(7, 0b00000001);         // Hiển thị dấu '-' ở vị trí đầu
        send_data(6, digit_codes[d1]); // Hàng chục
        
        send_data(4, digit_codes[d3]); // Số thập phân đầu tiên
        send_data(3, digit_codes[d4]); // Số thập phân thứ hai
        send_data(2, 0b00011101); // Số thập phân thứ hai
        send_data(1, 0b00001000); // Số thập phân thứ hai
    }
    
}

void display_yaw( float yaw) {

    int negative = (yaw < 0); // Kiểm tra xem số có âm không
    if (negative) yaw = -yaw;  // Chuyển thành số dương để xử lý

    int yaw_int = (int)(yaw );
    int yaw_frac = (int)(yaw * 100) % 100;    

    int d1 = yaw_int / 10;   // Chữ số hàng chục
    int d2 = yaw_int % 10;   // Chữ số hàng đơn vị
    int d3 = yaw_frac / 10;  // Chữ số thập phân đầu tiên
    int d4 = yaw_frac % 10;  // Chữ số thập phân thứ hai

    if (negative) {
        send_data(8,0b00001000);
        send_data(7, 0b00000001);         // Hiển thị dấu '-' ở vị trí đầu
        send_data(6, digit_codes[d1]); // Hàng chục
        send_data(5, digit_codes[d2] | 0b10000000); // Hàng đơn vị + dấu chấm
        send_data(4, digit_codes[d3]); // Số thập phân đầu tiên
        send_data(3, digit_codes[d4]); // Số thập phân thứ hai
        send_data(2, 0b00011101); // Số thập phân thứ hai
        send_data(1, 0b00001000); // Số thập phân thứ hai
    } else {
        send_data(8,0b00001000);
    
        send_data(6, digit_codes[d1]); // Hàng chục
        send_data(5, digit_codes[d2] | 0b10000000);
        send_data(4, digit_codes[d3]); // Số thập phân đầu tiên
        send_data(3, digit_codes[d4]); // Số thập phân thứ hai
        send_data(2, 0b00011101); // Số thập phân thứ hai
        send_data(1, 0b00001000); // Số thập phân thứ hai
    }
    
}

void fadeLED(int pin) {
    int delayTime = 20;  // Độ trễ giữa các mức (20ms)

    // Sáng dần trong 2 giây
    for (int i = 0; i <= PWM_MAX; i++) {
        softPwmWrite(pin, i);
        delay(delayTime);
    }

    // Tối dần trong 2 giây
    for (int i = PWM_MAX; i >= 0; i--) {
        softPwmWrite(pin, i);
        delay(delayTime);
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
    softPwmCreate(red, 0, PWM_MAX);
    softPwmCreate(green, 0, PWM_MAX);
    softPwmCreate(blue, 0, PWM_MAX);


    while(1)
    {
    // đọc giá trị đo
    float Ax = (float)read_sensor(Acc_X)/4096.0;
    float Ay = (float)read_sensor(Acc_Y)/4096.0;
    float Az = (float)read_sensor(Acc_Z)/4096.0;
        

    float yaw   = atan2(Az,sqrt(pow(Ax,2)+pow(Ay,2)))*180/M_PI;

    printf("Goc xoay quanh truc Z (YAW)= %.4f\n", yaw);
    if (yaw < 45) 
    {
        fadeLED(green);
    }
    //display_values(Az);
    display_yaw(yaw);
    delay(500);
    }
    return 0;


}