#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

#define RED     11   // Chân GPIO đèn LED đỏ
#define GREEN   12   // Chân GPIO đèn LED xanh lá
#define BLUE    13   // Chân GPIO đèn LED xanh dương

#define BT1     15   // Nút chọn màu
#define BT2     22   // Nút tăng độ sáng
#define BT3     18   // Nút giảm độ sáng
#define BT4     16
int color = 0;  // 0 = Red, 1 = Green, 2 = Blue, 3 = White
int brightness = 50;  // Độ sáng ban đầu (50%)

void setLED(int r, int g, int b) {
    softPwmWrite(RED, r);
    softPwmWrite(GREEN, g);
    softPwmWrite(BLUE, b);
}

void setup() {
    wiringPiSetupPhys();  // Sử dụng số chân vật lý trên Raspberry Pi

    // Cấu hình LED là output + PWM mềm
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    softPwmCreate(RED, 0, 100);
    softPwmCreate(GREEN, 0, 100);
    softPwmCreate(BLUE, 0, 100);

    // Cấu hình nút nhấn là input (có điện trở kéo lên)
    pinMode(BT1, INPUT);
    pinMode(BT2, INPUT);
    pinMode(BT3, INPUT);
    setLED(0, 0, 0);  // Ban đầu đèn tắt hoàn toàn
}

void loop() {
    while (1) {
        if (digitalRead(BT1) == 1) {  // Nhấn BT1 để chuyển màu
            color = (color + 1) % 4;  
            delay(300);  // Chống dội nút
        } 

        if (digitalRead(BT2) == 1) {  // Nhấn BT2 để tăng sáng
            if (brightness < 100) brightness += 10;
            delay(300);
        }

        if (digitalRead(BT3) == 1) {  // Nhấn BT3 để giảm sáng
            if (brightness > 0) brightness -= 10;
            delay(300);
        }
        if (digitalRead(BT4) == 1) {  // Nhấn BT3 để giảm sáng
            softPwmWrite(BLUE, 0);
            softPwmWrite(RED, 0);
            softPwmWrite(GREEN, 0);
            delay(300);
        }

        // Cập nhật màu theo giá trị `color`
        if (color == 0) setLED(brightness, 0, 0);      // Đỏ
        else if (color == 1) setLED(0, brightness, 0); // Xanh lá
        else if (color == 2) setLED(0, 0, brightness); // Xanh dương
        else setLED(brightness, brightness, brightness); // Trắng

        delay(100);
    }
}

int main() {
    setup();
    loop();
    return 0;
}
