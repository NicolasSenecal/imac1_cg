#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 1000;
static unsigned int WINDOW_HEIGHT = 800;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre de polygones d'un cercle */
static const unsigned int CERCLE_LIGNES = 50;

static const float SCALE_X = 100.;
static const float SCALE_Y = 80.;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/*
 contour : 0 = forme pleine, autre = taille du contour
 */
void drawSquare(float contour, int r, int v, int b) {
    if (contour == 0) {
        glBegin(GL_QUADS);
    } else {
        glLineWidth(contour);
        glBegin(GL_LINE_STRIP);
    }
    glColor3ub(r, v, b);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glEnd();
}

void drawLandmark() {
    float i;
    float gradSize = 0.25;
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

void drawRoundedSquare(float contour, float cornerRadius, int r, int v, int b) {
    int i, j;
    glPushMatrix();
    /**/glScalef(1. - 2 * cornerRadius, 1, 0);
    /**/drawSquare(contour, r, v, b);
    glPopMatrix();
    glPushMatrix();
    /**/glScalef(1, 1 - 2 * cornerRadius, 0);
    /**/drawSquare(contour, r, v, b);
    glPopMatrix();
    for (i = -1; i <= 1; i += 2) {
        for (j = -1; j <= 1; j += 2) {
            glPushMatrix();
            /**/glTranslatef(i * (0.5 - cornerRadius), j * (0.5 - cornerRadius), 0);
            /**/glScalef(2 * cornerRadius, 2 * cornerRadius, 0);
            /**/drawCircle(contour, r, v, b);
            glPopMatrix();
        }
    }
}

void drawFirstArm(float contour, int r, int v, int b) {
    glPushMatrix();
    /**/glLineWidth(0.5);
    /**/glBegin(GL_LINES);
    /**/glColor3ub(r, v, b);
    /**/glVertex2f(-30, 20);
    /**/glVertex2f(30, 10);
    /**/glVertex2f(30, -10);
    /**/glVertex2f(-30, -20);
    /**/glEnd();
    glPopMatrix();
    glPushMatrix();
    /**/glTranslatef(-30, 0, 0);
    /**/glScalef(20 * 2, 20 * 2, 0);
    /**/drawCircle(contour, r, v, b);
    glPopMatrix();
    glPushMatrix();
    /**/glTranslatef(30, 0, 0);
    /**/glScalef(10 * 2, 10 * 2, 0);
    /**/drawCircle(contour, r, v, b);
    glPopMatrix();
}

void drawSecondArm(float contour, int r, int v, int b) {
    glPushMatrix();
    /**/glTranslatef(-45 / 2, 0, 0);
    /**/glScalef(10, 10, 0);
    /**/drawRoundedSquare(0, 0.1, r + 55, v + 55, b + 55);
    glPopMatrix();
    glPushMatrix();
    /**/glTranslatef(45 / 2, 0, 0);
    /**/glScalef(10, 10, 0);
    /**/drawRoundedSquare(0, 0.1, r + 55, v + 55, b + 55);
    glPopMatrix();
    glPushMatrix();
    /**/glScalef(46, 6, 0);
    /**/drawSquare(0, r, v, b);
    glPopMatrix();
}

void drawThirdArm(float contour, int r, int v, int b) {
    glPushMatrix();
    /**/glTranslatef(-18, 0, 0);
    /**/glScalef(6, 6, 0);
    /**/drawRoundedSquare(0, 0.1, r + 55, v + 55, b + 55);
    glPopMatrix();
    glPushMatrix();
    /**/glScalef(40, 4, 0);
    /**/drawSquare(contour, r, v, b);
    glPopMatrix();
    glPushMatrix();
    /**/glTranslatef(20, 0, 0);
    /**/glScalef(8, 8, 0);
    /**/drawCircle(contour, r, v, b);
    glPopMatrix();
}

/*
 *  Convertie un point en px de la fenetre en coordonnées du plan
 */
float convertCoordonnees(float pos, char type) {
    if (type == 'x') return (SCALE_X * (-1 + 2. * pos / WINDOW_WIDTH));
    else return (-SCALE_Y * (-1 + 2. * pos / WINDOW_HEIGHT));
}

int main(int argc, char** argv) {

    /* Initialisation de la SDL */
    if (-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }


    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if (NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /* Titre de la fenêtre */
    SDL_WM_SetCaption("TP03", NULL);


    /* Boucle d'affichage */
    int loop = 1;
    while (loop) {
        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glScalef(1. / SCALE_X, 1. / SCALE_Y, 0);


        //drawFirstArm(0.2, 0, 0, 0);
        //drawSecondArm(0.2, 0, 0, 0);
        //drawThirdArm(0, 0, 0, 0);


        drawLandmark();
        /* Echange du front et du back buffer : mise à jour de la fenêtre */
        SDL_GL_SwapBuffers();

        /* Boucle traitant les evenements */
        SDL_Event e;
        while (SDL_PollEvent(&e)) {


            /* L'utilisateur ferme la fenêtre : */
            if (e.type == SDL_QUIT) {
                loop = 0;
                break;
            }
            switch (e.type) {
                case SDL_VIDEORESIZE:
                    WINDOW_WIDTH = e.resize.w;
                    WINDOW_HEIGHT = e.resize.h;
                    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();
                    gluOrtho2D(-1., 1., -1., 1.);
                    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE); /* FIX ME - Écran en partie noir pendant le redimensionnement */
                    break;
                case SDL_KEYUP:
                    switch (e.key.keysym.sym) {
                        case SDLK_q:
                            loop = 0;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        /* Calcul du temps écoulé */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        /* Si trop peu de temps s'est écoulé, on met en pause le programme */
        if (elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Liberation des ressources associées à la SDL */
    SDL_Quit();

    return EXIT_SUCCESS;
}
