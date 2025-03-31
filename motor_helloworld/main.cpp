// Sweep the motor speed from full-speed reverse (-1.0) to full speed forwards (1.0)

#include "mbed.h"
#include "Motor.h"

Motor m1(p22, p5, p6); // pwm, fwd, rev
Motor m2(p23, p7, p8);

DigitalOut standby(p7);

int main() {
    standby = 1;
    while (true){
        for (float s = -1.0; s < 1.0; s += 0.01) {
            m1.speed(s);
            wait_us(20000);
        }
       /*m1.speed(1.0); 
       wait_us(200000);
       m1.speed(-1.0); 
       wait_us(200000);
       m1.speed(0.0); 
       m2.speed(1.0); 
       wait_us(200000);
       m2.speed(-1.0); 
       wait_us(200000);
       m2.speed(0.0);*/
    //}
    }
}
