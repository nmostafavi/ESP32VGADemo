#include <ESP32Lib.h>

class Label
{
  private:
    String text;
    int x, y;

  public:
    Label(String t, int pX, int pY)
      : text(t), x(pX), y(pY) { }

    void draw(VGA14Bit vga, int t)
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