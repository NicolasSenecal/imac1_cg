#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void resizeViewport() {
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1., 1., -1., 1.);
  SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

void setTexture(const char* filename, GLuint textureID) {
  SDL_Surface *img = IMG_Load(filename);
  if (!img) {
    printf("Oups... something went wrong\n");
    exit(1);
  }

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  
  float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

  glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RGBA,
          img->w,
          img->h,
          0,
          GL_RGBA,
          GL_UNSIGNED_BYTE,
          img->pixels);

  glBindTexture(GL_TEXTURE_2D, 0);
  // Libération des données CPU
  SDL_FreeSurface(img);
}

int main(int argc, char** argv) {
  // Initialisation de la SDL
  if (-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }

  // Ouverture d'une fenêtre et création d'un contexte OpenGL
  if (NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  SDL_WM_SetCaption("td04", NULL);
  resizeViewport();



  GLuint textureID[11];
  glGenTextures(11, textureID);
  char buf[50];
  for (int i = 0; i < 10; i++) {
    sprintf(buf, "./numbers/%d.png", i);
    setTexture(buf, textureID[i]);
  }
  setTexture("./numbers/colon.png", textureID[10]);

  // Gestion de l'opacité
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Boucle de dessin (à décommenter pour l'exercice 3)
  int loop = 1;
  glClearColor(0.1, 0.1, 0.1, 1.0);
  while (loop) {

    Uint32 startTime = SDL_GetTicks();

    // TODO: Code de dessin 

    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D); // On précise qu’on veut activer la fonctionnalité de texturing
    glBindTexture(GL_TEXTURE_2D, textureID[5]); // On bind la texture pour pouvoir l’utiliser

    glPushMatrix();


    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(-0.5, 0.5);
    glTexCoord2f(1, 0);
    glVertex2f(0.5, 0.5);
    glTexCoord2f(1, 1);
    glVertex2f(0.5, -0.5);
    glTexCoord2f(0, 1);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D); // On désactive le sampling de texture
    glBindTexture(GL_TEXTURE_2D, 0); // On débind la texture

    // Fin du code de dessin

    SDL_Event e;
    while (SDL_PollEvent(&e)) {

      switch (e.type) {

        case SDL_QUIT:
          loop = 0;
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
  glDeleteTextures(11, textureID);

  // Liberation des ressources associées à la SDL
  SDL_Quit();

  return EXIT_SUCCESS;
}
