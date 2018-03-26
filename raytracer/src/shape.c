/*
 * shape.c
 * Structures et fonctions utilitaires pour manipuler des formes géométrique
 */
#include "shape.h"

Sphere createSphere(Point3D o, float r, Color3d color) {
  Sphere s;
  s.o = o;
  s.r = r;
  s.color = color;
  return s;
}

Cube createCube(Point3D min, Point3D max, Color3f color) {
  Cube c;
  c.min = min;
  c.max = max;
  c.color = color;
  return c;
}