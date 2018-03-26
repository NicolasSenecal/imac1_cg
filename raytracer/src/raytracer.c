/*
 * raytracer.c
 * Structures et fonctions utilitaires pour manipuler les rayons
 */
#include "raytracer.h"

Ray createRay(Point3D o, Vector3D d) {
  Ray r;
  r.o = o;
  r.d = d;
  return r;
}

Intersection createIntersection(Point3D p, Color3f color) {
  Intersection i;
  i.p = p;
  i.color = color;
  return i;
}

int intersectsSphere(Ray r, Sphere s, Intersection* i) {
  /* Équation du second degré */
  float a = 1; /* on normalise la direction du rayon (a=|d|²=1) */
  float b = dot(multVector(r.o, 2.), pointPlusVector(multVector(s.o, -1.), r.d));
  float c = pow(norm(r.o), 2) + pow(norm(s.o), 2) - 2 * dot(s.o, r.o);
  float delta = pow(b, 2) - 4. * a * c;
  float t = -1.;
  float t1 = 0.;
  float t2 = 0.;

  if (delta < 0.)
    return 0;
  else if (delta = 0.)
    t = -b / (2. * a);
  else {
    t1 = (-b - sqrt(delta)) / (2. * a);
    t2 = (-b + sqrt(delta)) / (2. * a);
    if (t1 < t2 && t1 > 0)
      t = t1;
    else if (t1 > t2 && t2 > 0)
      t = t2;
    else
      t = t1;
  }

  if (t < 0)
    return 0;

  *i = createIntersection(pointPlusVector(r.o, multVector(r.d, t)), s.color);
  return 1;
}