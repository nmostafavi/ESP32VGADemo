#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>
#include <ColorRamp.h>
#include <Label.h>
#include <Ball.h>

// VGA pins
const int redPins[] = {2, 4, 12, 13, 14};
const int greenPins[] = {15, 16, 17, 18, 19};
const int bluePins[] = {21, 22, 23, 27};
const int hsyncPin = 32;
const int vsyncPin = 33;

VGA14Bit vga;

const int numBalls = 3;
Ball *balls = nullptr;

const int numGravityBalls = 100;
GravityBall *gravityBalls = nullptr;

void setup()
{
  // init
  Serial.begin(115200);
  randomSeed(analogRead(0));

  // graphics and framebuffer
  vga.setFrameBufferCount(2);
  vga.init(vga.MODE320x200, redPins, greenPins, bluePins, hsyncPin, vsyncPin);
  vga.setFont(Font6x8);

  balls = new Ball[numBalls];
  gravityBalls = new GravityBall[numGravityBalls];
}

void update(int dt)
{
  for (int i = 0; i < numBalls; i++)
  {
    Ball& b = balls[i];
    b.update(dt);
  }

  for (int i = 0; i < numGravityBalls; i++)
  {
    GravityBall& gb = gravityBalls[i];
    gb.update(dt);
  }
}

void draw(int t)
{
  // gradient
  for (int x = 0; x < 320; x++)
  {
    for (int y = 0; y < 200; y++)
    {
      vga.dot(x, y, vga.RGBA(x*255/320, x*255/320, x*255/320, 255));
    }
  }

  // balls
  for (int i = 0; i < numBalls; i++)
  {
    const Ball& b = balls[i];
    b.draw(vga);
  }

  // more balls
  for (int i = 0; i < numGravityBalls; i++)
  {
    const GravityBall& gb = gravityBalls[i];
    gb.draw(vga);
  }

  // metaballs
  for (int x = 140; x < 300; x++)
  {
    for (int y = 40; y < 120; y++)
    {
      float v = 0.0f;
      for (int i = 0; i < numBalls; i++)
      {
        const Ball& b = balls[i];
        float dist = b.dist(x, y);
        if (dist > 0.0f)
        {
          v += 1.0f / dist;
        }
      }

      v = constrain(v*10.0, 0.0, 1.0);
      v = fmodf((v), 1.0);
      unsigned short c = ColorRamp::getColor(hslRamp, v);
      vga.dot(x, y, c);
    }
  }

  // wave labels
  Label("feed me metaballs", 20, 40).draw(vga, t);
  Label("feed me metaballs", 20, 50).draw(vga, t);
  Label("feed me metaballs", 20, 60).draw(vga, t);
  Label("feed me metaballs", 20, 70).draw(vga, t);
}

void loop()
{
    // FPS counter
    static int lastMillis = 0;
    int t = millis();
    int dt = (t - lastMillis);
    int fps = 1000 / dt;
    lastMillis = t;

    vga.clear();

    update(dt);
    draw(t);

    // Debug text
    vga.setTextColor(0xffffff);
    vga.setCursor(0, 0);
    vga.print("mem: ");
    vga.println((int)heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
    vga.print("t: ");
    vga.print(t);
    vga.println(" ms");
    vga.print("fps: ");
    vga.print(fps, 10, 2);

    vga.show();
}
