#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <wiringPi.h>
#define spi0     0
//int a=0,b=0,c=0,d=0,e=0,f=0;
int DO,VANG,XANH;          

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

int main(void){
    // set up thu vien wiringPiPhys
    wiringPiSetupPhys();
    pinMode(8, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(12, OUTPUT);

    digitalWrite(8,LOW);
    digitalWrite(10,LOW);
    digitalWrite(12,LOW);

    printf("nhap giay cua den DO:");
    scanf("%d",&DO);
    printf("nhap giay cua den XANH:");
    scanf("%d",&XANH);
    printf("nhap giay cua den VANG:");
    scanf("%d",&VANG);

    wiringPiSPISetup(spi0, 9000000);
    while (1==1){
        for(int a =DO;a>0;a--)
        {
            
            digitalWrite(10,HIGH);
            uint8_t s[8] = {a/10, a%10, 10, 0, 0, 10, 0, 0};
            for (int j = 0; j < 8; j++)
            {
            send_data(j + 1, s[7 - j]);
            delay(10);
                
            }  
            sleep(1);           
        }
        digitalWrite(10,LOW);

        for(int b =XANH;b>0;b--)
        {
             
            digitalWrite(8,HIGH);
            uint8_t s[8] = {0, 0, 10, b/10, b%10, 10, 0, 0};
            for (int j = 0; j < 8; j++)
            {
            send_data(j + 1, s[7 - j]);
            delay(10);
              
            }
            sleep(1);  
        }
        digitalWrite(8,LOW);

        for(int c =VANG;c>0;c--)
        {
              
            digitalWrite(12,HIGH);
            uint8_t s[8] = {0, 0, 10, 0, 0, 10,c/10, c%10};
            for (int j = 0; j < 8; j++)
            {
            send_data(j + 1, s[7 - j]);
            delay(10);
                 
            }
            sleep(1);         
        }
        digitalWrite(12,LOW); 
    
}
return 0;
}
