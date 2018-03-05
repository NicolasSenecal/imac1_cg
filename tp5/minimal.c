#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

static const float SCALE_X = 16.;
static const float SCALE_Y = 16.;

/* Nombre de polygones d'un cercle */
static const unsigned int CERCLE_LIGNES = 60;

void resizeViewport() {
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-SCALE_X / 2, SCALE_X / 2, -SCALE_Y / 2, SCALE_Y / 2);
  SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

void drawLandmark() {
  int i;
  float gradSize = 0.1;
  glBegin(GL_LINES);
  glColor3ub(255, 0, 0);
  glVertex2f(-SCALE_X, 0);
  glVertex2f(SCALE_X, 0);
  for (i = -SCALE_X; i <= SCALE_X; i++) {
    if (i != 0) {
      glVertex2f(i, gradSize);
      glVertex2f(i, -gradSize);
    }
  }
  glColor3ub(0, 255, 0);
  glVertex2f(0, -SCALE_Y);
  glVertex2f(0, SCALE_Y);
  for (i = -SCALE_Y; i <= SCALE_Y; i++) {
    if (i != 0) {
      glVertex2f(gradSize, i);
      glVertex2f(-gradSize, i);
    }
  }
  glEnd();
}

/*
 contour : 0 = forme pleine, autre = taille du contour
 */
void drawCircle(float contour, int r, int v, int b) {
  if (contour == 0) {
    glBegin(GL_POLYGON);
  } else {
    glLineWidth(contour);
    glBegin(GL_LINE_STRIP);
  }
  glColor3ub(r, v, b);

  for (int i = 0; i <= CERCLE_LIGNES; i++) {
    glVertex2f(0.5 * cos(2 * M_PI * i / CERCLE_LIGNES), 0.5 * sin(2 * M_PI * i / CERCLE_LIGNES));
  }
  glEnd();
}

void drawGraduation(float size) {
  glPushMatrix();
  glTranslatef(0, SCALE_Y - 9.25, 0);
  glBegin(GL_QUADS);
  glColor3ub(0, 0, 0);

  glVertex2f(-size / 9, size / 2);
  glVertex2f(size / 9, size / 2);
  glVertex2f(size / 9, -size / 2);
  glVertex2f(-size / 9, -size / 2);

  glEnd();
  glPopMatrix();
}

void drawGraduations() {
  float size;
  for (int i = 0; i < 60; i++) {
    if (i % 5 == 0)
      size = 0.6;
    else
      size = 0.3;
    drawGraduation(size);
    glRotatef(-6, 0.0, 0.0, 1.0);
  }
}

void drawHourHand(float rad) {
  glPushMatrix();
  glRotatef(-rad, 0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glColor3ub(0, 0, 0);

  glVertex2f(-0.12, SCALE_Y - 13);
  glVertex2f(0.12, SCALE_Y - 13);
  glVertex2f(0.12, -0.5);
  glVertex2f(-0.12, -0.5);

  glEnd();
  glPopMatrix();
}

void drawMinuteHand(float rad) {
  glPushMatrix();
  glRotatef(-rad, 0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glColor3ub(0, 0, 0);

  glVertex2f(-0.08, SCALE_Y - 10);
  glVertex2f(0.08, SCALE_Y - 10);
  glVertex2f(0.08, -1);
  glVertex2f(-0.08, -1);

  glEnd();
  glPopMatrix();
}

void drawSecondHand(float rad) {
  glPushMatrix();
  glRotatef(-rad, 0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glColor3ub(0, 0, 0);

  glVertex2f(-0.05, SCALE_Y - 9);
  glVertex2f(0.05, SCALE_Y - 9);
  glVertex2f(0.05, -1);
  glVertex2f(-0.05, -1);

  glEnd();
  glPopMatrix();
}

void drawShapeClock() {
  glPushMatrix();
  glScalef(SCALE_X - 0.2, SCALE_Y - 0.2, 0);
  drawCircle(0, 0, 0, 0);
  glPopMatrix();
  glPushMatrix();
  glScalef(SCALE_X - 0.8, SCALE_Y - 0.8, 0);
  drawCircle(0, 235, 235, 235);
  glPopMatrix();
  glPushMatrix();
  glScalef(SCALE_X - 1, SCALE_Y - 1, 0);
  drawCircle(2., 0, 0, 0);
  glPopMatrix();
}

int rand_a_b(int a, int b) {
  return rand() % (b - a) + a;
}

int main(int argc, char** argv) {
  time_t rawtime;
  struct tm * timeinfo;

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
  SDL_WM_SetCaption("td05", NULL);


  resizeViewport();


  int loop = 1;
  while (loop) {

    Uint32 startTime = SDL_GetTicks();
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    drawShapeClock();
    drawGraduations();
    glPushMatrix();
    drawSecondHand(timeinfo->tm_sec * 6);
    drawMinuteHand(timeinfo->tm_min * 6 + timeinfo->tm_sec / 10);
    drawHourHand(timeinfo->tm_hour * 30 + timeinfo->tm_min / 2);
    glPopMatrix();

    glPushMatrix();

    glPopMatrix();







    SDL_Event e;
    while (SDL_PollEvent(&e)) {

      switch (e.type) {
        case SDL_KEYUP:
          switch (e.key.keysym.sym) {
            case SDLK_q:
              loop = 0;
              break;
            default:
              break;
          }
          break;
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

  // Liberation des ressources associées à la SDL
  SDL_Quit();

  return EXIT_SUCCESS;
}
