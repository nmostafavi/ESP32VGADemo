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
}

void update()
{
    for (int i = 0; i < numBalls; i++)
    {
        Ball& b = balls[i];
        b.x += b.vx;
        b.y += b.vy;

        if (b.x < 0 || b.x > vga.xres)
        {
            b.vx *= -1;
        }
        if (b.y < 0 || b.y > vga.yres)
        {
            b.vy *= -1;
        }
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

    // circles
    for (int i = 0; i < numBalls; i++)
    {
        const Ball& b = balls[i];
        vga.circle(b.x, b.y, 10, 0xffff);
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
                    v += 1.0f / b.dist(x, y);
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
    int fps = 1000 / (t - lastMillis);
    lastMillis = t;

    vga.clear();

    update();
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
