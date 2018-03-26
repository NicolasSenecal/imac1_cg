/*
 * raytracer.h
 * Structures et fonctions utilitaires pour manipuler les rayons
 */

#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "geometry.h"
#include "color.h"
#include <math.h>

typedef struct {
  Point3D o; /* Position de l'origine du rayon */
  Vector3D d; /* Direction du rayon */
} Ray;

typedef struct {
  Point3D p; /* Position de l'intersection */
  Color3f color; /* Couleur du point d'intersection */
} Intersection;

/*
 * Construit le rayon d'origine <o> et de direction <d>
 */
Ray createRay(Point3D o, Vector3D d);

/*
 * Construit l'Intersection de position <p> et de couleur <color>
 */
Intersection createIntersection(Point3D p, Color3f color);

/*
 * Teste si le rayon <r> intersecte la sphère <s>
 * Si c’est le cas, elle remplie les champs de l’intersection <i> et retourner 1. 
 * S’il n’y a pas d’intersection, elle retourne 0.
 */
int intersectsSphere(Ray r, Sphere s, Intersection* i);

#endif