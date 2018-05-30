/*
 * raytracer.h
 * Structures et fonctions utilitaires pour manipuler les rayons
 */

#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "geometry.h"
#include "colors.h"
#include "shape.h"
#include "sdl_tools.h"
#include <math.h>

#define SCENE_SHAPE_TAB_SIZE 10

typedef struct {
  Point3D o; /* Position de l'origine du rayon */
  Vector3D d; /* Direction du rayon */
} Ray;

typedef struct {
  Point3D p; /* Position de l'intersection */
  Color3f color; /* Couleur du point d'intersection */
} Intersection;

typedef struct {
  Sphere sphere[SCENE_SHAPE_TAB_SIZE];
  int sphereTabSize;
  Cube cube[SCENE_SHAPE_TAB_SIZE];
  int cubeTabSize;
} Scene;

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

/*
 * Créer et initialiser les différents champs de la structure Scene
 */
Scene createScene();

/*
 * Ajoute la sphere <sphere> dans la Scene <scene>
 * Retourne 0 si erreur d'allocation, 1 sinon
 */
int addSphereToScene(Scene* scene, Sphere sphere);

/*
 * Boucle sur tous les objets d’une scène afin de trouver 
 * l’intersection la plus proche avec le rayon passé en paramètre
 * Retourne 1 s’il y a une intersection, 0 sinon
 */
int throwRayOnScene(Ray ray, const Scene* scene, Intersection* intersection);

/*
 * Itère sur chacun des pixels du framebuffer
 * Pour chaque pixel elle calcule le rayon avec throwRayOnScene
 * puis envoie ce rayon dans la scène. Si une intersection est
 * trouvée elle place la couleur du point d’intersection à la position 
 * correspondante du framebuffer (avec PutPixel de sdl_tools.h)
 * Sinon elle ne touche pas au pixel
 */
void simpleRaytracing(const Scene* scene, SDL_Surface* framebuffer);

#endif