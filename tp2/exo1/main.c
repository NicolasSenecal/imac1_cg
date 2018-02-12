#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

typedef struct Point {
    float x, y; /* Position 2D du point */
    unsigned char r, g, b; /* Couleur du point */
    struct Point* next; /* Point suivant à dessiner */
} Point, *PointList;

Point* allocPoint(float x, float y, unsigned char r, unsigned char g, unsigned char b) {
    Point* point = (Point*) malloc(sizeof (point));
    if (!point) {
        printf("Memory run out\n");
        exit(1);
    }
    point->x = x;
    point->y = y;
    point->r = r;
    point->g = g;
    point->b = b;
    point->next = NULL;
    return point;
}

void addPointToList(Point* point, PointList* list) {
    if (*list != NULL) {
        addPointToList(point, &(*list)->next);
    } else {
        *list = point;
    }
}

void drawPoints(PointList list) {
    while (list != NULL) {
        glColor3ub(list->r, list->g, list->b);
        glVertex2f(list->x, list->y);
        list = list->next;
    }
}

void deletePoints(PointList* list) {
    while (*list != NULL) {
        Point* nextPoint = (*list)->next;
        free(*list);
        *list = nextPoint;
    }
}

int main(int argc, char** argv) {

    PointList points = NULL;

    /* Initialisation de la SDL */
    if (-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /* Désactivation du double buffering */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);

    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if (NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Paint", NULL);


    /* Boucle d'affichage */
    int loop = 1;
    while (loop) {
        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        if (points != NULL) {
            glBegin(GL_LINE_STRIP);
            drawPoints(points);
            glEnd();
        }

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
                case SDL_MOUSEBUTTONUP:
                    addPointToList(allocPoint(-1 + 2. * e.button.x / WINDOW_WIDTH, -(-1 + 2. * e.button.y / WINDOW_HEIGHT), 0, 0, 0), &points);

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

    deletePoints(&points);

    /* Liberation des ressources associées à la SDL */
    SDL_Quit();

    return EXIT_SUCCESS;
}
