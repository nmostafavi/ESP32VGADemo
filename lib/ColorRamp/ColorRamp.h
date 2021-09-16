#include <Arduino.h>

static const unsigned short hslRamp[16] = {
  0x00ff, 0x06ff, 0x0bff, 0x0fdf, 0x0f8f, 0x0f2f, 0x4f0f, 0x9f0f, 
  0xff0f, 0xf90f, 0xf40f, 0xf02f, 0xf07f, 0xf0df, 0xb0ff, 0x60ff };
static const unsigned short funkyRamp[16] = { 
  0x598f, 0x489f, 0x47af, 0x46bf, 0x35cf, 0x34cf, 0x23df, 0x22ef, 
  0x22ff, 0x23ff, 0x25ff, 0x36ff, 0x37ff, 0x38ff, 0x49ff, 0x4aff };
static const unsigned short freshRamp[16] = { 
  0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 
  0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff };

class ColorRamp
{
  public:
    static unsigned short getColor(const unsigned short ramp[16], float intensity)
    {
      intensity *= (16 - 1);
      int low = roundf(intensity);
      return ramp[low];
    }

    static unsigned short getColorInterpolated(const unsigned short ramp[16], float intensity)
    {
      intensity *= (16 - 1);
      int low = floorf(intensity);
      int high = ceilf(intensity);
      int mix = intensity - low;
      return (ramp[low] * (1.0 - mix) + ramp[high] * mix);
    }
};