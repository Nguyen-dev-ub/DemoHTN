    #include <wiringPi.h>
    #include <wiringPiI2C.h>
    #include <stdio.h>
    #include <stdint.h>
    #include <math.h>

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
    int mpu;    

    int16_t read_sensor(unsigned char sensor)
    {
        int16_t high, low, data;
        high = wiringPiI2CReadReg8 (mpu,sensor);
        low  =  wiringPiI2CReadReg8 (mpu,sensor +1);
        data = (high<<8) | low;
        return data;
    }
    uint8_t buff[2];

void send_data(uint8_t address, uint8_t data){
    buff[0] = address;
    buff[1] = data;
    wiringPiSPIDataRW(spi0, buff, 2);
}

void init_max7219(void){
    // decode mode: use code B for all digits -> 0x09FF
    send_data(0x09, 0xFF); // cau hinh 8 led sang
    // intensity: 70% --> 0x0A0B
    send_data(0x0A, 0x1); // muc sang
    // scan limit
    send_data(0x0B, 7); // hien thi 7 so
    // turn off display test, no shutdow
    send_data(0x0F, 0); 
    send_data(0x0C, 1);
}
    void Init_6050(void)
        {
        // register 25 ->28 ,56,107 cấu hình thanh ghi của mpu 6050.tổng 6 thanh ghi
        // sample rate 500Hz, cách tính sample rate = 8000/(1+X)=500 => x=15
        // trong datasheet, gyro output rate =8kHz
        // khi DLPF =(0 or 7) (do bộ lọc xung tắt)
        // khi DLPF =1 (do bộ lọc xung bật) đọc Reg 26
        wiringPiI2CWriteReg8(mpu,Sample_rate,15);
        //không sử dụng nguồn xung ngoài, tắt bộ lọc số (DLPf) 
        wiringPiI2CWriteReg8(mpu,Config,0);
        //gyro FS: +-500 o/s
        wiringPiI2CWriteReg8(mpu,Gyro_config,0x08); // 00001000
        // acc FS: +-8g
        wiringPiI2CWriteReg8(mpu,Acc_config,0x10); //00010000
        // mở interrupt của data ready
        wiringPiI2CWriteReg8(mpu,Interrupt,1);
        // chọn nguồn xung Gyro X
        wiringPiI2CWriteReg8(mpu,PWR_Managment,0x01);
        }
    int main(void)
    {
        //setup giao tiep I2C
        mpu = wiringPiI2CSetup(0x68); //chân ID0 mức 0 là ox68
        wiringPiSPISetup(spi0, 9000000);
        // thiết lập chết độ cho mpu60504
        Init_6050();
        while(1)
        {
        // đọc giá trị đo
        float Ax = (float)read_sensor(Acc_X)/4096.0;
        float Ay = (float)read_sensor(Acc_Y)/4096.0;
        float Az = (float)read_sensor(Acc_Z)/4096.0;
        
        float pitch = atan2(Ax,sqrt(pow(Ay,2)+pow(Az,2)))*180/M_PI;
        float roll = atan2(Ay,sqrt(pow(Ax,2)+pow(Az,2)))*180/M_PI;

        printf("Ax = %.4f,          Ay = %.4f,          Az = %.4f\n", Ax, Ay, Az);
        printf("        Pitch = %.4f,         Roll = %.4f\n", pitch, roll);

        delay(1000);
        }
        return 0;


    }