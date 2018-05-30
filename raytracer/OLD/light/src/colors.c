#include "colors.h"

Color3f createColor(float r, float g, float b) {
  Color3f color;
  color.r = r;
  color.g = g;
  color.b = b;
  return color;
}

Color3f AddColors(Color3f c1, Color3f c2) {
  c1.r += c2.r;
  c1.g += c2.g;
  c1.b += c2.b;
  return c1;
};

Color3f SubColors(Color3f c1, Color3f c2) {
  c1.r -= c2.r;
  c1.g -= c2.g;
  c1.b -= c2.b;
  return c1;
}

Color3f MultColors(Color3f c1, Color3f c2) {
  c1.r *= c2.r;
  c1.g *= c2.g;
  c1.b *= c2.b;
  return c1;
}

Color3f DivColors(Color3f c1, Color3f c2) {
  c1.r /= c2.r;
  c1.g /= c2.g;
  c1.b /= c2.b;
  return c1;
}

Color3f MultColor(Color3f c, float a) {
  c.r *= a;
  c.g *= a;
  c.b *= a;
  return c;
}

Color3f DivColor(Color3f c, float a) {
  c.r /= a;
  c.g /= a;
  c.b /= a;
  return c;
}

Color3f clampColor(Color3f c) {
  if (c.r < 0) c.r = 0;
  else if (c.r > 1) c.r = 1;
  if (c.g < 0) c.g = 0;
  else if (c.g > 1) c.g = 1;
  if (c.b < 0) c.b = 0;
  else if (c.b > 1) c.b = 1;
  return c;
}
