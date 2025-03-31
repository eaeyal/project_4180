#include "mbed.h"
// speaker sound effect demo using PWM hardware output
PwmOut speaker(p21);

int main()
{
    int i;
// generate a 500Hz tone using PWM hardware output
    speaker.period(1.0/500.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait_us(3000000);
    speaker=0.0; // turn off audio
    wait_us(2000000);

// generate a short 150Hz tone using PWM hardware output
// something like this can be used for a button click effect for feedback
    for (i=0; i<10; i++) {
        speaker.period(1.0/150.0); // 500hz period
        speaker =0.25; //25% duty cycle - mid range volume
        wait_us(20000);
        speaker=0.0; // turn off audio
        wait_us(500000);
    }

// sweep up in frequency by changing the PWM period
    for (i=0; i<8000; i=i+100) {
        speaker.period(1.0/float(i));
        speaker=0.25;
        wait_us(100000);
    }
    wait_us(2000000);

// two tone police siren effect -  two periods or two frequencies
// increase volume - by changing the PWM duty cycle
    for (i=0; i<26; i=i+2) {
        speaker.period(1.0/969.0);
        speaker = float(i)/50.0;
        wait_us(500000);
        speaker.period(1.0/800.0);
        wait_us(500000);
    }
// decrease volume
    for (i=25; i>=0; i=i-2) {
        speaker.period(1.0/969.0);
        speaker = float(i)/50.0;
        wait_us(500000);
        speaker.period(1.0/800.0);
        wait_us(500000);
    }
    speaker =0.0;
    wait_us(2000000);

}