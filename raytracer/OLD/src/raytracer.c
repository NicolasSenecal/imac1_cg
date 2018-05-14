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

  if (delta < 0.) {
    return 0;
  } else if (delta == 0.) {

    t = -b / (2. * a);
  } else {
          printf("delta > 0");
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

Scene createScene() {
  Scene scene;
  scene.sphereTabSize = 0;
  scene.cubeTabSize = 0;
  return scene;
}

int addSphereToScene(Scene* scene, Sphere sphere) {
  if (scene->sphereTabSize > SCENE_SHAPE_TAB_SIZE)
    return 0;
  scene->sphere[scene->sphereTabSize] = sphere;
  scene->sphereTabSize++;
  return 1;
}

int throwRayOnScene(Ray ray, const Scene* scene, Intersection* intersection) {
  int i;
  Intersection t;
  float normMin = 0.;
  float normTmp = 0.;
  for (i = 0; i < scene->sphereTabSize; i++) {
    if (intersectsSphere(ray, scene->sphere[i], &t)) {
      normTmp = norm(vector(ray.o, t.p));
      if (normMin == 0. || normTmp < normMin) {
        *intersection = t;
        normMin = normTmp;
      }
    }
  }
  return (normMin != 0);
}

void simpleRaytracing(const Scene* scene, SDL_Surface* framebuffer) {
  int x, y;
  Intersection t;
  Ray r;
  r.o = pointXYZ(0, 0, 0);
  for (y = 0; y < framebuffer->h; y++) {
    for (x = 0; x < framebuffer->w; x++) {
      r.d = vectorXYZ(-1. + 2. * (x / framebuffer->w), 1. - 2. * (y / framebuffer->h), -1.);
      if (throwRayOnScene(r, scene, &t)) {
        PutPixel(framebuffer, x, y, SDL_MapRGB(framebuffer->format, t.color.r * 255, t.color.g * 255, t.color.b * 255));
      }
    }
  }
}