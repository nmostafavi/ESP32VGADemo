#include <ESP32Lib.h>

class Ball
{
  public:
    int x, y, vx, vy;
    unsigned short color;

    Ball()
    {
        x = random(0, 320);
        y = random(0, 200);
        vx = random(-10, 10);
        vy = random(-10, 10);
        color = random(0xffff);
    }

    float dist(float pX, float pY) const
    {
        return Vector::sqrt((pX - x) * (pX - x) + (pY - y) * (pY - y));
    }
};