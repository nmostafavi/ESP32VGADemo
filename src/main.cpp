#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>

// VGA pins
const int redPins[] = {2, 4, 12, 13, 14};
const int greenPins[] = {15, 16, 17, 18, 19};
const int bluePins[] = {21, 22, 23, 27};
const int hsyncPin = 32;
const int vsyncPin = 33;

VGA14Bit vga;
Mode mode = vga.MODE320x200;

class Ball
{
public:
	int x, y, vx, vy;
	unsigned short color;
	Ball()
	{
		x = random(0, vga.xres);
		y = random(0, vga.yres);
		vx = random(-10, 10);
		vy = random(-10, 10);
		color = random(0xffff);
	}

	float dist(float pX, float pY) const
	{
		return Vector::sqrt((pX - x) * (pX - x) + (pY - y) * (pY - y));
	}
};

const int numBalls = 3;
Ball *balls = nullptr;

class Label
{
private:
	String text;
	int x, y;
public:
	Label(String t, int pX, int pY)
		: text(t), x(pX), y(pY) { }
	void draw(int t)
	{
		int pX = x;
		int pY = y;
		for (int i = 0; i < text.length(); i++)
		{
			int yOffset = 3.0 * cosf(2.0*PI*t/1000.0f + PI*pX/50.0f);
			vga.drawChar(pX, (pY + yOffset), text[i]);
			pX += 6;
		}
	}
};

float hue2rgb(float p, float q, float t) 
{
	// credit: michael jackson (a-hee-hee!)
	if (t < 0.0) t += 1.0;
	if (t > 1.0) t -= 1.0;
	if (t < 1.0/6.0) return p + (q - p) * 6.0 * t;
	if (t < 1.0/2.0) return q;
	if (t < 2.0/3.0) return p + (q - p) * (2.0/3.0 - t) * 6.0;
	return p;
}

void hslToRgb(float h, float s, float l, int& outR, int& outG, int& outB)
{
	// Once again, Michael Jackson saves the day with his HSL to RGB conversion code.
	// https://stackoverflow.com/a/9493060
	// in: h, s, l [0, 1]
	// out: r, g, b [0, 255]

    float r, g, b;

    if (s == 0) 
	{
        r = g = b = l;
    } 
	else 
	{
        float q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        float p = 2.0 * l - q;
        r = hue2rgb(p, q, h + 1.0/3.0);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1.0/3.0);
    }

	outR = r * 255;
	outG = g * 255;
	outB = b * 255;
}

unsigned short colorRamp(float intensity)
{
	intensity = constrain(intensity, 0.0, 1.0);
	int r, g, b;
	hslToRgb(intensity, 0.5, 0.5, r, g, b);
	return vga.RGBA(r, g, b, 255);
}

void setup()
{
	// init
	Serial.begin(115200);
	randomSeed(analogRead(0));

	// graphics and framebuffer
	vga.setFrameBufferCount(2);
	vga.init(mode, redPins, greenPins, bluePins, hsyncPin, vsyncPin);
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
		vga.circle(b.x, b.y, 10, 0xf00f);
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
			unsigned short c = colorRamp(v*10.0f);
			vga.dot(x, y, c);
		}
	}

	// wave labels
	Label("feed me metaballs", 20, 40).draw(t);
	Label("feed me metaballs", 20, 50).draw(t);
	Label("feed me metaballs", 20, 60).draw(t);
	Label("feed me metaballs", 20, 70).draw(t);
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
