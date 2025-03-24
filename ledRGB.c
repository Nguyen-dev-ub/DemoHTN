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
    
    softPwmCreate(red, 0, 1023);
    softPwmCreate(green, 0, 1023);
    softPwmCreate(blue, 0, 1023);

    /* led sang dan
    softPwmCreate(red, 0, PWM_MAX);
    softPwmCreate(green, 0, PWM_MAX);
    softPwmCreate(blue, 0, PWM_MAX);
    */

    while (1==1)
    {
        if (digitalRead(bt1) == 1) {
            softPwmWrite(red, 512);  // PWM 50%
        } else {
            softPwmWrite(red, 0);  // Tắt LED
        }

        if (digitalRead(bt2) == 1) {
            softPwmWrite(green, 512);
        } else {
            softPwmWrite(green, 0);
        }

        if (digitalRead(bt3) == 1) {
            softPwmWrite(blue, 512);
        } else {
            softPwmWrite(blue, 0);
        }
        delay(10);  // th

        /*led sang dan
        fadeLED(red);   // Sáng dần, tối dần LED đỏ
        fadeLED(green); // Sáng dần, tối dần LED xanh lá
        fadeLED(blue);  // Sáng dần, tối dần LED xanh dương
        */
    }

    return(0);
    

        

    }
