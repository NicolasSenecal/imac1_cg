/*
 * colors.c
 * Structures et fonctions utilitaires pour manipuler des couleurs
 */
#include "colors.h"

float verifCanal(float a) {
  if (a > 1.) return 1.;
  if (a < 0.) return 0.;
  return a;
}

Color3f color3f(float r, float g, float b) {
  Color3f newColor;
  newColor.r = verifCanal(r);
  newColor.g = verifCanal(g);
  newColor.b = verifCanal(b);
  return newColor;
}

Color3f addColors(Color3f c1, Color3f c2) {
  Color3f res;
  res.r = verifCanal(c1.r + c2.r);
  res.g = verifCanal(c1.g + c2.g);
  res.b = verifCanal(c1.b + c2.b);
  return res;
}

Color3f subColors(Color3f c1, Color3f c2) {
  Color3f res;
  res.r = verifCanal(c1.r - c2.r);
  res.g = verifCanal(c1.g - c2.g);
  res.b = verifCanal(c1.b - c2.b);
  return res;
}

Color3f multColors(Color3f c1, Color3f c2) {
  Color3f res;
  res.r = verifCanal(c1.r * c2.r);
  res.g = verifCanal(c1.g * c2.g);
  res.b = verifCanal(c1.b * c2.b);
  return res;
}

Color3f multColor(Color3f c, float a) {
  Color3f res;
  res.r = verifCanal(c.r * a);
  res.g = verifCanal(c.g * a);
  res.b = verifCanal(c.b * a);
  return res;
}

Color3f divColor(Color3f c, float a) {
  Color3f res;
  res.r = verifCanal(c.r / a);
  res.g = verifCanal(c.g / a);
  res.b = verifCanal(c.b / a);
  return res;
}