#include <Arduino.h>

static const unsigned short hslRamp[16] = {
  0x00ff, 0x06ff, 0x0bff, 0x0fdf, 0x0f8f, 0x0f2f, 0x4f0f, 0x9f0f, 
  0xff0f, 0xf90f, 0xf40f, 0xf02f, 0xf07f, 0xf0df, 0xb0ff, 0x60ff};

class ColorRamp
{
  public:
    static unsigned short getColor(const unsigned short ramp[16], float intensity)
    {
      intensity *= (16 - 1);
      int low = roundf(intensity);
      return ramp[low];
    }
};