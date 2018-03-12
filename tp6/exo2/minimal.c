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

const char* filename = "kana.png";

void resizeViewport() {
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1.5, 1.5, -1.5, 1.5);
  SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

void drawSquare(int full, int x, int y) {
  if (full == 0) {
    glBegin(GL_LINE_LOOP);
  } else {
    glBegin(GL_TRIANGLE_FAN);
  }
  glTexCoord2f(x / 16., y / 6.);
  glVertex2f(-0.5f, 0.5f);
  glTexCoord2f((x + 1) / 16., y / 6.);
  glVertex2f(0.5f, 0.5f);
  glTexCoord2f((x + 1) / 16., (y + 1) / 6.);
  glVertex2f(0.5f, -0.5f);
  glTexCoord2f(x / 16., (y + 1) / 6.);
  glVertex2f(-0.5f, -0.5f);
  glEnd();
}

void drawTexturedSquare(GLuint textureID, int x, int y) {
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureID);
  drawSquare(1, x, y);
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void drawColorMask(float r, float v, float b, float a) {
  glBlendFunc(GL_DST_COLOR, GL_ZERO);
  glColor4f(r, v, b, a);
  glBegin(GL_QUADS);
  glVertex2f(-0.5, 0.5);
  glVertex2f(0.5, 0.5);
  glVertex2f(0.5, -0.5);
  glVertex2f(-0.5, -0.5);
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


  glClearColor(0., 0., 0., 1.0);
  glClear(GL_COLOR_BUFFER_BIT);


  // option de blending OpenGL
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  // Chargement et traitement de la texture
  SDL_Surface *kanaPng = IMG_Load(filename);
  if (!kanaPng) {
    printf("Oups... something went wrong\n");
    exit(1);
  }
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RGBA,
          kanaPng->w,
          kanaPng->h,
          0,
          GL_RGBA,
          GL_UNSIGNED_BYTE,
          kanaPng->pixels);

  glBindTexture(GL_TEXTURE_2D, 0);

  // Libération des données CPU
  SDL_FreeSurface(kanaPng);

  int loop = 1;
  while (loop) {

    Uint32 startTime = SDL_GetTicks();

    // Code de dessin
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    drawTexturedSquare(textureID, 2, 2);
    glPopMatrix();


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

  // TODO: Libération des données GPU
  glDeleteTextures(1, &textureID);

  // Liberation des ressources associées à la SDL
  SDL_Quit();

  return EXIT_SUCCESS;
}