#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>

// Dimensions de la fenêtre
static unsigned int WINDOW_WIDTH = 600;
static unsigned int WINDOW_HEIGHT = 600;

// Nombre de bits par pixel de la fenêtre
static const unsigned int BIT_PER_PIXEL = 32;

// Nombre minimal de millisecondes separant le rendu de deux images
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void resizeViewport() {
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1.5, 1.5, -1.5, 1.5);
  SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

void drawSquare(int full) {
  if (full == 0) {
    glBegin(GL_LINE_LOOP);
  } else {
    glBegin(GL_TRIANGLE_FAN);
  }
  glVertex2f(-0.5f, 0.5f);
  glVertex2f(0.5f, 0.5f);
  glVertex2f(0.5f, -0.5f);
  glVertex2f(-0.5f, -0.5f);
  glEnd();
}

int main(int argc, char** argv) {

  // Initialisation de la SDL
  if (-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }

  // Ouverture d'une fenêtre et création d'un contexte OpenGL
  if (NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL)) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    return EXIT_FAILURE;
  }

  SDL_WM_SetCaption("td06", NULL);

  resizeViewport();

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // option de blending OpenGL
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float tx1 = -0.25;
  float ty1 = 0.25;
  float tx2 = 0.25;
  float ty2 = -0.25;
  float tspeed = 0.01;

  int a1 = 255 / 2;
  int a2 = 255 / 2;
  int aspeed = 5;

  int cubeSelected = 1;

  int loop = 1;
  while (loop) {

    Uint32 startTime = SDL_GetTicks();

    // Code de dessin
    glClear(GL_COLOR_BUFFER_BIT);

    if (cubeSelected == 2) {
      glPushMatrix();
      glTranslatef(tx1, ty1, 0);
      glColor4ub(255, 0, 255, a1);
      drawSquare(1);
      glPopMatrix();

      glPushMatrix();
      glTranslatef(tx2, ty2, 0);
      glColor4ub(255, 255, 0, a2);
      drawSquare(1);
      glPopMatrix();
    } else {
      glPushMatrix();
      glTranslatef(tx2, ty2, 0);
      glColor4ub(255, 255, 0, a2);
      drawSquare(1);
      glPopMatrix();

      glPushMatrix();
      glTranslatef(tx1, ty1, 0);
      glColor4ub(255, 0, 255, a1);
      drawSquare(1);
      glPopMatrix();

    }


    // Event loop

    SDL_Event e;
    while (SDL_PollEvent(&e)) {

      if (e.type == SDL_QUIT) {
        loop = 0;
        break;
      }

      switch (e.type) {

        case SDL_MOUSEBUTTONUP:
          break;

        case SDL_KEYDOWN:
          switch (e.key.keysym.sym) {
            case SDLK_SPACE:
              if (cubeSelected == 1)
                cubeSelected = 2;
              else
                cubeSelected = 1;
              break;

            case SDLK_KP_PLUS:
              if (cubeSelected == 1 && a1 + 5 <= 255)
                a1 += aspeed;
              if (cubeSelected == 2 && a2 + 5 <= 255)
                a2 += aspeed;
              break;

            case SDLK_KP_MINUS:
              if (cubeSelected == 1 && a1 - 5 >= 0)
                a1 -= aspeed;
              if (cubeSelected == 2 && a2 - 5 >= 0)
                a2 -= aspeed;
              break;

            case SDLK_LEFT:
              if (cubeSelected == 1)
                tx1 -= tspeed;
              else
                tx2 -= tspeed;
              break;

            case SDLK_RIGHT:
              if (cubeSelected == 1)
                tx1 += tspeed;
              else
                tx2 += tspeed;
              break;

            case SDLK_UP:
              if (cubeSelected == 1)
                ty1 += tspeed;
              else
                ty2 += tspeed;
              break;

            case SDLK_DOWN:
              if (cubeSelected == 1)
                ty1 -= tspeed;
              else
                ty2 -= tspeed;
              break;

            case SDLK_q:
              loop = 0;
              break;
            default:
              break;
          }

          break;

        case SDL_VIDEORESIZE:
          WINDOW_WIDTH = e.resize.w;
          WINDOW_HEIGHT = e.resize.h;
          resizeViewport();

        default:
          break;
      }
    }

    SDL_GL_SwapBuffers();
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    if (elapsedTime < FRAMERATE_MILLISECONDS) {
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }
  }

  // Liberation des ressources associées à la SDL
  SDL_Quit();

  return EXIT_SUCCESS;
}