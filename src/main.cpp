#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>

// VGA pins
const int redPins[] = {2, 4, 12, 13, 14};
const int greenPins[] = {15, 16, 17, 18, 19};
const int bluePins[] = {21, 22, 23, 27};
const int hsyncPin = 32;
const int vsyncPin = 33;

// button pins
const int buttonPin = 36;
int counter = 0;

VGA14Bit vga;

void setup()
{
	// graphics and framebuffer
	vga.setFrameBufferCount(2);
	vga.init(vga.MODE320x200, redPins, greenPins, bluePins, hsyncPin, vsyncPin);
	vga.setFont(Font6x8);

	// input button
	pinMode(buttonPin, INPUT);
}

void loop()
{
	for (int y = 0; y < vga.yres / 10; y++)
		for (int x = 0; x < vga.xres / 10; x++)
			vga.fillRect(x * 10, y * 10, 10, 10, (x + y) & 1 ? vga.RGB(0, 128, 0) : vga.RGB(0, 0, 128));

	static int lastMillis = 0;
	int t = millis();
	int fps = 1000 / (t - lastMillis);
	lastMillis = t;

	vga.setTextColor(0xffffff);
	vga.setCursor(0, 0);
	vga.print("mem: ");
	vga.print((int)heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
	vga.print(" fps: ");
	vga.print(fps, 10, 2);
	vga.setCursor(160, 100);
	vga.print("Hello World!");
	if (digitalRead(buttonPin) == HIGH)
	{
		vga.print("HIGH");
	}
	else
	{
		vga.print("LOW");
		counter += 1;
	}
	vga.print(counter, 10, 10);

	vga.show();
}
