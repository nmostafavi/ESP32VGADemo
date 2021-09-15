#include <ESP32Lib.h>

class Ball
{
  private:
    int x, y, vx, vy;

  public:
    Ball()
    {
      x = random(0, 320);
      y = random(0, 200);
      vx = random(-100, 100);
      vy = random(-100, 100);
    }

    float dist(float pX, float pY) const
    {
      return Vector::sqrt((pX - x) * (pX - x) + (pY - y) * (pY - y));
    }

    void update(int dt)
    {
      float delta = dt / 1000.0;
      x += vx * delta;
      y += vy * delta;

      if (x < 0 || x > 320)
      {
          vx *= -1;
      }
      if (y < 0 || y > 200)
      {
          vy *= -1;
      }
    }

    void draw(VGA14Bit vga) const
    {
      vga.circle(x, y, 10, 0xf00f);
    }
};