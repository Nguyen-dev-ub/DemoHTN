#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPiSPI.h>
#include <stdint.h> 
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

void setColor(int r, int g, int b) {
    softPwmWrite(red, r);
    softPwmWrite(green, g);
    softPwmWrite(blue, b);
}
//button 1
void inputColor() {
    int r, g, b;
    printf("Nhập giá trị RGB (0-100) theo định dạng r g b: ");
    scanf("%d %d %d", &r, &g, &b);
    
    if (r < 0 || r > 100 || g < 0 || g > 100 || b < 0 || b > 100) {
        printf("Giá trị không hợp lệ! Nhập lại trong khoảng 0-100.\n");
        return;
    }
    
    setColor(r, g, b);
}

//mode 1
void fadeLED() {
    for (int i = 0; i <= PWM_MAX; i++) {
        setColor(i, i, i);
        delay(20);
    }
    for (int i = PWM_MAX; i >= 0; i--) {
        setColor(i, i, i);
        delay(20);
    }
}

//mode 2
void cycleColors() {
    int colors[8][3] = {
        {100,   0,      0}, 
        {0,     100,    0}, 
        {0,     0,      100}, 
        {100,   100,    0},
        {0,     100,    100}, 
        {100,   0,      100}, 
        {50,    50,     50}, 
        {100,   100,    100}
    };

    for (int i = 0; i < 8; i++) {
        setColor(colors[i][0], colors[i][1], colors[i][2]);
        delay(500);
    }
}
int main(void)
    {
        wiringPiSetupPhys();
    wiringPiSPISetup(spi0, 4000000);
    
    //thiet lap che do hoat dong OUTPUT cho cac chan (BCM)
    pinMode(red,OUTPUT);
    pinMode(green,OUTPUT);
    pinMode(blue,OUTPUT);
    pinMode(bt1,INPUT);
    pinMode(bt2,INPUT);
    pinMode(bt3,INPUT);
    pinMode(bt4,INPUT);

    pullUpDnControl(bt1, PUD_DOWN);
    pullUpDnControl(bt2, PUD_DOWN);

    // Cấu hình PWM cho LED RGB
    softPwmCreate(red, 0, PWM_MAX);
    softPwmCreate(green, 0, PWM_MAX);
    softPwmCreate(blue, 0, PWM_MAX);

    while (1) {
        if (digitalRead(bt1) == 1) {
            delay(300); // Chống dội phím
            inputColor();
        }
        
        if (digitalRead(bt2) == 1) {
            delay(300);
            printf("\nChọn chế độ:\n1 - Tăng giảm độ sáng\n2 - Chuyển màu 8 màu\n");
            int mode;
            scanf("%d", &mode);
            
            if (mode == 1) {
                printf("MODE 1: TANG GIAM DO SANG\n");
                fadeLED();
            } else if (mode == 2) {
                printf("MODE 2: CHUYEN MAU QUA LAI\n");
                cycleColors();
            } else {
                printf("Lựa chọn không hợp lệ!\n");
            }
        }
    }
    return 0;
}