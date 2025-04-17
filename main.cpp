#include "mbed.h"
#include "uLCD.hpp"
#include "Motor.h"
#include <stdio.h>
#include "ultrasonic.h"

PwmOut speaker(p21);

DigitalIn bUp(p9);
DigitalIn bCenter(p8);
DigitalIn bLeft(p7);
DigitalIn bDown(p6);
DigitalIn bRight(p5);

DigitalOut yellow(p10);
DigitalOut blue(p11);
DigitalOut green(p12);
DigitalOut red(p13);

PinName tx(p28);
PinName rx(p27);
PinName reset(p29);
uLCD lcd(tx, rx, reset, uLCD::BAUD_9600);
int hours = 0;
int hoursSelected = 1;
int minutes = 0;
int minutesSelected = 0;
int seconds = 0;
int secondsSelected = 0;
char hourString[3];
char minuteString[3];
char secondString[3];

int easySelected = 1;
int mediumSelected = 0;
int hardSelected = 0;

Timer seed;

Motor right(p26, p25, p24); // pwm, fwd, rev
Motor left(p23, p22, p30); // left

void draw_grid() {
    for (int i = 0; i < 128; i+=5) {
        for (int j = 0; j < 128; j += 5) {
            lcd.setPixel(i, j, 0xFFFF);
        }
    }
}

void set_time() {
    lcd.cls();
    wait_us(200000);
    while (1) {
        if (hours == 0 && minutes == 0 && seconds == 0) {
            seconds = 1;
        }
        lcd.cls();
        lcd.locate(1, 0);
        lcd.setFontSize(2, 2);
        lcd.setTextColor(0xFFFF);
        sprintf(hourString, "%d", hours);
        if (hours < 10) {
            sprintf(hourString, "0%d", hours);
        }
        sprintf(minuteString, "%d", minutes);
        if (minutes < 10) {
            sprintf(minuteString, "0%d", minutes);
        }
        sprintf(secondString, "%d", seconds);
        if (seconds < 10) {
            sprintf(secondString, "0%d", seconds);
        }
        lcd.printf("%s:%s:%s\n", hourString, minuteString, secondString);
        if (hoursSelected) {
            lcd.drawRectangleFilled(10, 15, 30, 15, 0xFFFF);
        } 
        else if (minutesSelected) {
            lcd.drawRectangleFilled(52, 15, 72, 15, 0xFFFF);
        }
        else {
            lcd.drawRectangleFilled(94, 15, 114, 15, 0xFFFF);
        }
        while (1) {
            if (bDown == 0 && hoursSelected) {
                hours = (hours + 1) % 24;
                break;
            }
            if (bDown == 0 && minutesSelected) {
                minutes = (minutes + 1) % 60;
                break;
            }
            if (bDown == 0 && secondsSelected) {
                seconds = (seconds + 1) % 60;
                break;
            }
            if (bUp == 0 && hoursSelected) {
                hours = (hours - 1); 
                if (hours < 0) {
                    hours = 23;
                }
                break;
            }
            if (bUp == 0 && minutesSelected) {
                minutes = (minutes - 1);
                if (minutes < 0) {
                    minutes = 59;
                }
                break;
            }
            if (bUp == 0 && secondsSelected) {
                seconds = seconds - 1;
                if (seconds < 0) {
                    seconds = 59;
                }
                break;
            }
            if (bLeft == 0) {
                if (hoursSelected) {
                    hoursSelected = 0;
                    minutesSelected = 1;
                }
                else if (minutesSelected) {
                    minutesSelected = 0;
                    secondsSelected = 1;
                }
                else {
                    secondsSelected = 0;
                    hoursSelected = 1;
                }
                break;
            }
            if (bRight == 0) {
                if (hoursSelected) {
                    hoursSelected = 0;
                    secondsSelected = 1;
                }
                else if (minutesSelected) {
                    minutesSelected = 0;
                    hoursSelected = 1;
                } 
                else {
                    secondsSelected = 0;
                    minutesSelected = 1;
                }
                break;
            }
            if (bCenter == 0) {
                return;
            }
        }
        wait_us(200000);
    }
}

uint16_t red_color = uLCD::get4DGLColor("0xFF0000");
uint16_t green_color = uLCD::get4DGLColor("0x00FF00");
uint16_t blue_color = uLCD::get4DGLColor("0x0000FF");
uint16_t purple_color = uLCD::get4DGLColor("0xA020F0");
uint16_t yellow_color = uLCD::get4DGLColor("0xFFFF00");


void set_simon_difficulty() {
    seed.start();
    while (1) {
        lcd.cls();
        lcd.locate(1, 0);
        lcd.setFontSize(2, 2);
        lcd.setTextColor(0xFFFF);
        lcd.printf("%s:%s:%s\n\n", hourString, minuteString, secondString);

        lcd.setTextColor(blue_color);
        lcd.printf("EASY\n");
        lcd.setTextColor(green_color);
        lcd.printf("MEDIUM\n");
        lcd.setTextColor(red_color);
        lcd.printf("HARD");

        if (easySelected) {
            lcd.drawCircleFilled(70, 38, 5, 0xFFFF);
        } 
        if (mediumSelected) {
            lcd.drawCircleFilled(100, 55, 5, 0xFFFF);
        }
        if (hardSelected) {
            lcd.drawCircleFilled(70, 72, 5, 0xFFFF);
        }

        while (1) {
            if (bUp == 0) {
                if (easySelected) {
                    easySelected = 0;
                    mediumSelected = 1;
                }
                else if (mediumSelected) {
                    mediumSelected = 0;
                    hardSelected = 1;
                }
                else {
                    hardSelected = 0;
                    easySelected = 1;
                }
                break;
            }
            if (bDown == 0) {
                if (easySelected) {
                    easySelected = 0;
                    hardSelected = 1;
                }
                else if (mediumSelected) {
                    mediumSelected = 0;
                    easySelected = 1;
                }
                else {
                    hardSelected = 0;
                    mediumSelected = 1;
                }
                break;
            }
            if (bCenter == 0) {
                seed.stop();
                return;
            }
        }
    }
}

void config_mode() {
    set_time();
    set_simon_difficulty();
}

void countdown() {
    int hoursLeft = hours;
    int minutesLeft = minutes;
    int secondsLeft = seconds;
    while (1) {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        secondsLeft--;
        if (secondsLeft < 0) {
            secondsLeft = 59;
            minutesLeft--;
        }
        if (minutesLeft < 0) {
            minutesLeft = 59;
            hoursLeft--;
        }
        if (hoursLeft < 0) {
            return;
        }
        lcd.cls();
        lcd.locate(1, 3);
        lcd.setFontSize(2, 2);
        lcd.setTextColor(0xFFFF);
        sprintf(hourString, "%d", hoursLeft);
        if (hoursLeft < 10) {
            sprintf(hourString, "0%d", hoursLeft);
        }
        sprintf(minuteString, "%d", minutesLeft);
        if (minutesLeft < 10) {
            sprintf(minuteString, "0%d", minutesLeft);
        }
        sprintf(secondString, "%d", secondsLeft);
        if (secondsLeft < 10) {
            sprintf(secondString, "0%d", secondsLeft);
        }
        lcd.printf("%s:%s:%s\n", hourString, minuteString, secondString);
        gettimeofday(&end, NULL);
        int elapsed_usec = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
        wait_us(1000000 - elapsed_usec);

    }
}
volatile int done = 0;
void alarm() {
    lcd.cls();
    // User view- left:yellow, up:green, right:red, down:blue
    // bLeft:red, bUp:blue, bRight:yellow, bDown:green
    int patternLength = 4;
    if (mediumSelected) {
        patternLength = 8;
    }
    if (hardSelected) {
        patternLength = 12;
    }
    srand(seed.read_us());
    int pattern[patternLength];
    for (int i = 0; i < patternLength; i++) {
        pattern[i] = rand() % 4;
    }

    while (1) {
        lcd.cls();
        lcd.drawRectangleFilled(0, 0, 128, 128, yellow_color);
        lcd.locate(0, 3);
        lcd.setTextColor(blue_color);
        lcd.setTextBackground(yellow_color);
        lcd.setFontSize(3, 4);
        lcd.printf("good\nMORNING\n>:D");
        for (int i = 0; i < patternLength; i++) {
            switch (pattern[i]) {
                case 0:
                    yellow = 1;
                    red = 0;
                    green = 0;
                    blue = 0;
                    break;
                case 1:
                    yellow = 0;
                    red = 1;
                    green = 0;
                    blue = 0;
                    break;
                case 2:
                    yellow = 0;
                    red = 0;
                    green = 1;
                    blue = 0;
                    break;
                case 3:
                    yellow = 0;
                    red = 0;
                    green = 0;
                    blue = 1;
                    break;
            }
            wait_us(1000000);
            yellow = 0;
            red = 0;
            green = 0;
            blue = 0;
            wait_us(500000);
        }
        for (int p = 0; p < patternLength; p++) {
            int a;
            while (1) {
                if (bUp == 0) {
                    a = 3;
                    blue = 1;
                    break;
                }
                if (bLeft == 0) {
                    a = 1;
                    red = 1;
                    break;
                }
                if (bRight == 0) {
                    a = 0;
                    yellow = 1;
                    break;
                }
                if (bDown == 0) {
                    a = 2;
                    green = 1;
                    break;
                }
            }
            if (pattern[p] != a) {
                lcd.cls();
                lcd.drawRectangleFilled(0, 0, 128, 128, red_color);
                lcd.locate(1, 6);
                lcd.setFontSize(3, 3);
                lcd.setTextColor(0xFFFF);
                lcd.setTextBackground(red_color);
                lcd.printf("WRONG");
                wait_us(1000000);
                break;
            }
            if (p + 1 == patternLength) {
                yellow = 0;
                blue = 0;
                green = 0;
                red = 0;
                done = 1;
                return;
            }
            wait_us(500000);
            yellow = 0;
            blue = 0;
            green = 0;
            red = 0;
        }
    }
}

void title_screen() {
    lcd.cls();
    lcd.locate(2, 2);
    lcd.setFontSize(2, 3);
    lcd.setTextBackground(0x0000);
    lcd.setTextColor(purple_color);
    lcd.printf("WAKEBOT\n");
    lcd.setTextColor(green_color);
    lcd.setFontSize(1, 1);
    lcd.printf("\n  Edan Eyal\n\n Will Griffin\n");
    int blink = 0;
    while (1) {
        if (bCenter == 0) {
            return;
        }
        if (blink) {
            lcd.locate(2, 11);
            lcd.setFontSize(1, 1);
            lcd.setTextColor(0xFFFF);
            lcd.printf("Press to Start");
        } else {
            lcd.drawRectangleFilled(0, 80, 120, 120, 0x0000);
        }
        blink = !blink;
        if (bCenter == 0) {
            return;
        }
        wait_us(200000);
    }
}
void sound() {
    while (1) {
        int i;
        for (i=0; i<26; i=i+2) {
            speaker.period(1.0/969.0);
            speaker = float(i)/50.0;
            wait_us(500000);
            speaker.period(1.0/800.0);
            wait_us(500000);
        }
    }
}

int lastDistance;
void motor(int distance) {
    distance /= 10;
    /*lcd.cls();
    lcd.locate(0, 0);
    lcd.setTextColor(0xFFFF);
    lcd.setTextBackground(0x0);
    lcd.printf("%d", distance);*/
    if (distance < 10 || lastDistance == distance) {
        left.speed(0);
        right.speed(-1);
    }
    else {
        left.speed(1);
        right.speed(1);
    }
    lastDistance = distance;
}
//DigitalIn echo(p16);
//DigitalOut trigger(p15);
ultrasonic sonar(p17, p18, .1, 1, &motor);
volatile int distances;
void sonar_distances() {
    sonar.startUpdates();
    while (1) {
        sonar.checkDistance();
    }
}

int main()
{ 
    bUp.mode(PullUp);
    bLeft.mode(PullUp);
    bDown.mode(PullUp);
    bRight.mode(PullUp);
    bCenter.mode(PullUp);
    while (true) {
        Thread sonarThread;
        Thread alarmThread;
        Thread soundThread;
        title_screen();
        config_mode();
        countdown();
        alarmThread.start(alarm);
        sonarThread.start(sonar_distances);
        soundThread.start(sound);
        while (1) {
            ThisThread::sleep_for(1000);
            if (done == 1) {
                alarmThread.terminate();
                sonarThread.terminate();
                soundThread.terminate();
                speaker = 0;
                left.speed(0);
                right.speed(0);
                done = 0;
                break;
            }
        }
    }
}
