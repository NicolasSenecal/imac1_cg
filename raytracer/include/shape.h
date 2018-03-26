/*
 * shape.h
 * Structures et fonctions utilitaires pour manipuler des formes géométrique
 */

#ifndef SHAPE_H
#define SHAPE_H
#include "color.h"

typedef struct {
  Point3D o; /* Position de l'origine du cercle */
  float r; /* Rayon du cercle */
  Color3f color; /* Couleur de la sphere */
} Sphere;

typedef struct {
  Point3D min; /* Position minimum du cube */
  Point3D max; /* Position maximum du cube */
  Color3f color; /* Couleur du cube */
} Cube;

/*
 * Construit la sphere d'origine <o>, de rayon <r> et de couleur <color>
 */
Sphere createSphere(Point3D o, float r, Color3d color);

/*
 * Construit un cube de point minimum <min>, de point maximum <max> (les deux points de sa diagonale) et de sa couleur <color>
 */
Cube createCube(Point3D min, Point3D max, Color3f color);


#endif