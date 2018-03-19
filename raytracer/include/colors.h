/*
 * colors.h
 * Structures et fonctions utilitaires pour manipuler des couleurs
 */

#ifndef COLORS_H
#define COLORS_H

typedef struct {
  float r; /* compris entre 0.0 et 1.0 */
  float g; /* compris entre 0.0 et 1.0 */
  float b; /* compris entre 0.0 et 1.0 */
} Color3f;

/*
 * Vérifie que <a> soit bien compris entre 0.0 et 1.0
 * returne:
 *    1. si une valeurs est supérieur a 1.
 *    0. Si une valeurs est inférieur a 0.
 *    a  sinon
 */
float verifCanal(float a);

/*
 * Fonction qui creer une couleurs
 */
Color3f color3f(float r, float g, float b);

/*
 * Fonction qui calcule la somme de deux couleurs
 */
Color3f addColors(Color3f c1, Color3f c2);

/*
 * Fonction qui calcule la soustraction de deux couleurs
 */
Color3f subColors(Color3f c1, Color3f c2);

/*
 * Fonction qui calcule la multiplication de deux couleurs
 */
Color3f multColors(Color3f c1, Color3f c2);

/*
 * Fonction qui calcule la multiplication d'une couleur par un scalaire
 */
Color3f multColor(Color3f c, float a);

/*
 * Fonction qui calcule la division d'une couleur par un scalaire
 */
Color3f divColor(Color3f c, float a);
#endif