    #include <wiringPiSPI.h>
    #include <wiringPi.h>
    int main(void)
    {
        wiringPiSetupGpio();
        //thiet lap che do hoat dong OUTPUT cho cac chan (BCM)
        pinMode(14,OUTPUT);
        pinMode(15,OUTPUT);
        pinMode(18,OUTPUT);
        
        //muc hoat dong LOW
        digitalWrite(15,LOW);
        digitalWrite(14,LOW);
        digitalWrite(18,LOW);

        while(1)
        {
            digitalWrite(14,HIGH);
            delay(1000);
            digitalWrite(14,LOW);
            delay(1000);
            digitalWrite(15,HIGH);
            delay(1000);
            digitalWrite(15,LOW);
            delay(1000);
            digitalWrite(18,HIGH);
            delay(1000);
            digitalWrite(18,LOW);
            delay(1000);
        }
        return 0;
        

    }
