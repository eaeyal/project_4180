#include "mbed.h"
#include "uLCD.hpp"
#include "ICM20948.hpp"

PinName tx(p13);
PinName rx(p14);
PinName reset(p15);
uLCD lcd(tx, rx, reset, uLCD::BAUD_9600);

PinName sda(p28);
PinName scl(p27);
IMU_ICM20948 imu(sda, scl);

int main() {
    imu.calibrate();
    float ball_x = 64.0f;
    float ball_y = 64.0f;
    float vx = 0.0f;
    float vy = 0.0f;
    int radius = 5;
    int screen_width = 128;
    int screen_height = 128;
    int color = 0xFFFF; // white
    bool colorToggle = false;
    float dt = 0.1;       
    float scale = 0.05f;

    while (true) {
        float gx, gy, gz;
        imu.getGyroscopeData(&gx, &gy, &gz);
        printf("Gyro: X = %f, Y = %f, Z = %f\n", gx, gy, gz);
        vx += scale * gx * dt;
        vy += scale * gy * dt;
        ball_x += vx * 10;
        ball_y += vy * 10;
        bool bounced = false;
        if (ball_x <= radius) {
            ball_x = radius;
            vx = -vx;
            bounced = true;
        }
        if (ball_x >= screen_width - radius) {
            ball_x = screen_width - radius;
            vx = -vx;
            bounced = true;
        }
        if (ball_y <= radius) {
            ball_y = radius;
            vy = -vy;
            bounced = true;
        }
        if (ball_y >= screen_height - radius) {
            ball_y = screen_height - radius;
            vy = -vy;
            bounced = true;
        }
        if (bounced) {
            colorToggle = !colorToggle;
            color = colorToggle ? 0xFF00 : 0xFFFF;
        }
        lcd.cls();
        lcd.drawCircle((int)ball_x, (int)ball_y, radius, color);

        wait_us(100000);
    }
}
