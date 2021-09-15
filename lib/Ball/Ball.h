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

    float distSq(float pX, float pY) const
    {
      return (pX - x) * (pX - x) + (pY - y) * (pY - y);
    }

    float dist(float pX, float pY) const
    {
      return Vector::sqrt(distSq(pX, pY));
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

class GravityBall
{
  private:
    float x, y, vx, vy;
    unsigned int c;
  
  public:
    GravityBall()
    {
      x = random(200, 320);
      y = random(100, 200);
      vx = random(-20, 20);
      vy = 0;
      c = 0x00ff;
    }

    void update(int dt)
    {
      float delta = dt / 1000.0;

      if (x < 200 || x > 320)
      {
        vx *= -1;
      }
      if (y > 200 && vy > 0)
      {
        vy *= -1; // bounce off the ground
        vy += 20; // lose some velocity each time
      }

      x += vx * delta;
      y += vy * delta;
      vy += 100.0 * delta;  // gravity
    }

    void draw(VGA14Bit vga) const
    {
      vga.dot(x, y, c);
    }
};
