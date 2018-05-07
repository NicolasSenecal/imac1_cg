#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "geometry.h"
#include "colors.h"
#include "shape.h"
#include "raytracer.h"

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;

/* Fonction provisoire de test */
void testGeometry() {

  printf("\n pointPlusVector : (0, 0, 0) + (1, 2, 0) = ");
  printVector3D(pointPlusVector(pointXYZ(0, 0, 0), vectorXYZ(1, 2, 0)));

  printf("\n addVectors : (0.5, 1.0, -2.0) + (0.2, -1.0, 0) = ");
  printVector3D(addVectors(vectorXYZ(0.5, 1.0, -2.), vectorXYZ(0.2, -1., 0.)));

  printf("\n subVectors : (0.5, 1.0, -2.0) - (0.2, -1.0, 0) = ");
  printVector3D(subVectors(vectorXYZ(0.5, 1.0, -2.0), vectorXYZ(0.2, -1., 0)));

  printf("\n multVector : (0.5, 1.0, -2.0) * 2.0 = ");
  printVector3D(multVector(vectorXYZ(0.5, 1.0, -2.0), 2.0));

  printf("\n multVector : (0.5, 1.0, -2.0) * 0 = ");
  printVector3D(multVector(vectorXYZ(0.5, 1.0, -2.0), 0));

  printf("\n divVector : (0.5, 1.0, -2.0) / 2.0 = ");
  printVector3D(divVector(vectorXYZ(0.5, 1.0, -2.0), 2.0));

  printf("\n divVector : (0.5, 1.0, -2.0) / 0.0 = ");
  printVector3D(divVector(vectorXYZ(0.5, 1.0, -2.0), 0));

  printf("\n dot((1.0, 0.0, 0.0), (2.0, 0.0, 0.0)) = ");
  printf("%f", dot(vectorXYZ(1.0, 0, 0), vectorXYZ(2., 0., 0.)));

  printf("\n dot((1.0, 0.0, 0.0), (0.0, 1.0, 0.0)) = ");
  printf("%f", dot(vectorXYZ(0.0, 0, 0), vectorXYZ(2., 0., 0.)));

  printf("\n norm((2, 0, 0) = ");
  printf("%f", norm(vectorXYZ(2, 0, 0)));

  printf("\n norm((1, 1, 1) = ");
  printf("%f", norm(vectorXYZ(1, 1, 1)));

  printf("\n norm((0, 0, 0)) = ");
  printf("%f", norm(vectorXYZ(0, 0, 0)));

  printf("\n normalize((1, 1, 1)) = ");
  printVector3D(normalize(vectorXYZ(1, 1, 1)));

  printf("\n normalize((0, 0, 0)) = ");
  printVector3D(normalize(vectorXYZ(0, 0, 0)));
}

int fonctionImplicite(int x, int y) {
  Point3D p1 = pointXYZ(-3, 0, 0);
  Point3D p2 = pointXYZ(3, 0, 0);
  Point3D c = pointXYZ(x, y, 0);
  Vector3D v1 = vector(p1, c);
  Vector3D v2 = vector(p2, c);
  float n1 = norm(v1);
  float n2 = norm(v2);
  return ((9 / (4 * pow(n1, 2))) + (9 / (4 * pow(n2, 2)))) >= 0.5;
}

int main(int argc, char** argv) {

  // Initialisation de la SDL
  if (-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }

  // Création de la fenêtre SDL
  SDL_Surface* screen = NULL;
  if (NULL == (screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_DOUBLEBUF))) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  SDL_WM_SetCaption("Raytracing powa :D", NULL);

  // Création d'une surface SDL dans laquelle le raytracer dessinera
  SDL_Surface* framebuffer = NULL;
  if (NULL == (framebuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, 0, 0, 0, 0))) {
    fprintf(stderr, "Erreur d'allocation pour le framebuffer. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  // Nettoyage du framebuffer par une couleur de fond (noir)
  SDL_FillRect(framebuffer, NULL, SDL_MapRGB(framebuffer->format, 0, 0, 0));


  // Création de la scène
  Scene scene = createScene();

  // Ajout d'objets dans la scène
  Sphere s1 = createSphere(pointXYZ(0, 0, -3), 1, color3f(1, 0, 0));
  addSphereToScene(&scene, s1);

  // Appel de la fonction de raytracing
  simpleRaytracing(&scene, framebuffer);
  
  
  
  /*
  SDL_SaveBMP(framebuffer, "test.bmp");
  return 0;
   */

  int loop = 1;
  while (loop) {

    SDL_BlitSurface(framebuffer, NULL, screen, NULL);
    SDL_Flip(screen);

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        loop = 0;
        break;
      }
    }
  }

  SDL_Quit();

  return EXIT_SUCCESS;
}
