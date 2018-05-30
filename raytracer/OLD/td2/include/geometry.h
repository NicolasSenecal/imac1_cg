/*
 * geometry.h
 * Structures et fonctions utilitaires pour manipuler des vecteurs et des points
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <math.h>
#include <stdio.h>

typedef struct {
  float x, y, z;
} Vec3, Point3D, Vector3D;

/*
 * Construit le point de coordonnée x,y et z
 */
Point3D pointXYZ(float x, float y, float z);

/*
 * Construit le vecteur de coordonnée x,y et z
 */
Vector3D vectorXYZ(float x, float y, float z);

/*
 * Construit le vecteur AB
 */
Vector3D vector(Point3D A, Point3D B);

/*
 * Construit le point P + V
 */
Point3D pointPlusVector(Point3D P, Vector3D V);

/*
 * Fonction d’addition de vecteurs
 */
Vector3D addVectors(Vector3D A, Vector3D B);

/*
 * Fonction soustraction de vecteurs
 */
Vector3D subVectors(Vector3D A, Vector3D B);

/*
 * Fonction de multiplication de vecteurs
 */
Vector3D multVector(Vector3D A, float a);

/*
 * Fonction de division de vecteurs (si a=0, la fonction retourne A)
 */
Vector3D divVector(Vector3D A, float a);

/*
 * Fonction calculant le produit scalaire de deux vecteurs
 * Correspond a la longueur du projeté de u sur v multiplié par la longuer de v
 * 0 = les deux vecteurs sont orthogonaux
 * Positif si u et v sont dans le même sens, négatif sinon
 */
float dot(Vector3D A, Vector3D B);

/*
 * Fonction calculant la norme du vecteur
 */
float norm(Vector3D A);

/*
 * Fonction retournant le vecteur normalisé passé en paramètre
 * Si <A> est le vecteur nul, on le retourne 
 */
Vector3D normalize(Vector3D A);

/*
 * Affiche les coordonnée du vecteur <v>
 */
void printVector3D(Vector3D v);

/*
 * Affiche les coordonnée du point <p>
 */
void printPoint3D(Point3D p);

#endif