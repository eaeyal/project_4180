#include "mbed.h"

DigitalIn bUp(p5);
DigitalIn bCenter(p6);
DigitalIn bLeft(p7);
DigitalIn bDown(p8);
DigitalIn bRight(p9);

DigitalOut led(p10);
DigitalOut led2(p11);
DigitalOut led3(p12);
DigitalOut led4(p13);



int main() {
bUp.mode(PullUp);
bLeft.mode(PullUp);
bDown.mode(PullUp);
bRight.mode(PullUp);
bCenter.mode(PullUp);

while (true) {
if (bUp == 0) {
led = 1;
led2 = 0;
led3 = 0;
led4 = 0;
}
if (bDown == 0) {
led = 0;
led2 = 1;
led3 = 0;
led4 = 0; }
if (bLeft == 0) {
led = 0;
led2 = 0;
led3 = 1;
led4 = 0; }
if (bRight == 0) {
led = 0;
led2 = 0;
led3 = 0;
led4 = 1; }
if (bCenter == 0) {
led = 1;
led2 = 1;
led3 = 1;
led4 = 1; }
wait_us(500000);
}
}