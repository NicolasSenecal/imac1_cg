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

/* Couleurs de dessins possibles */
static const int COULEURS[][3] = {
    {255, 255, 255},
    {239, 239, 239},
    {206, 206, 206},
    {100, 100, 100},
    {0, 0, 0},
    {173, 57, 14},
    {30, 127, 203},
    {127, 221, 76},
    {167, 103, 38},
    {136, 77, 167}
};

/* Nombre de couleurs */
static const unsigned int NB_COULEURS = sizeof (COULEURS) / (3 * sizeof (int));

typedef struct Point {
    float x, y; /* Position 2D du point */
    unsigned char r, g, b; /* Couleur du point */
    struct Point* next; /* Point suivant à dessiner */
} Point, *PointList;

typedef struct Primitive {
    GLenum primitiveType;
    PointList points;
    struct Primitive* next;
} Primitive, *PrimitiveList;

void selectColorView() {
    int i;
    glBegin(GL_QUADS);
    for (i = 0; i < NB_COULEURS; ++i) {
        glColor3ub(COULEURS[i][0], COULEURS[i][1], COULEURS[i][2]);

        if (i % 2 == 0) {
            glVertex2f(-1 + i * 2.f / NB_COULEURS, 1);
            glVertex2f(-1 + (i + 2) * 2.f / NB_COULEURS, 1);
            glVertex2f(-1 + (i + 2) * 2.f / NB_COULEURS, 0);
            glVertex2f(-1 + i * 2.f / NB_COULEURS, 0);
        } else {
            glVertex2f(-1 + (i - 1) * 2.f / NB_COULEURS, -1);
            glVertex2f(-1 + (i + 1) * 2.f / NB_COULEURS, -1);
            glVertex2f(-1 + (i + 1) * 2.f / NB_COULEURS, 0);
            glVertex2f(-1 + (i - 1) * 2.f / NB_COULEURS, 0);
        }
    }
    glEnd();
}

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

Primitive* allocPrimitive(GLenum primitiveType) {
    Primitive* primitive = (Primitive*) malloc(sizeof (primitive));
    if (!primitive) {
        printf("Memory run out\n");
        exit(1);
    }
    primitive->primitiveType = primitiveType;
    primitive->points = NULL;
    primitive->next = NULL;
    return primitive;
}

/*  
 *  Ajoute un point au début de la liste
 *  Les points seront ainsi trier par ordre d'apparition
 */
void addPointToList(Point* point, PointList* list) {
    if (*list != NULL) {
        addPointToList(point, &(*list)->next);
    } else {
        *list = point;
    }
}

/*  
 *  Ajoute une primitive en fin de liste
 *  Les primitives seront ainsi trier par ordre d'apparition (les plus récents "écrasent" les autres)
 */
void addPrimitive(Primitive* primitive, PrimitiveList* list) {
    if (*list != NULL) {
        addPrimitive(primitive, &(*list)->next);
    } else {
        *list = primitive;
    }
}

/*
 *  Renvoie la primitive courante (dernière de la liste)
 */
PrimitiveList findLastPrimitive(PrimitiveList list) {
    while (list->next != NULL) {
        list = list->next;
    }
    return list;
}

void drawPoints(PointList list) {
    while (list != NULL) {
        glColor3ub(list->r, list->g, list->b);
        glVertex2f(list->x, list->y);
        list = list->next;
    }
}

void drawPrimitives(PrimitiveList list) {
    while (list != NULL) {
        glBegin(list->primitiveType);
        drawPoints(list->points);
        glEnd();
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

void deletePrimitives(PrimitiveList* list) {
    while (*list != NULL) {
        Primitive* next = (*list)->next;
        deletePoints(&(*list)->points);
        free(*list);
        *list = next;
    }
}

int main(int argc, char** argv) {

    int couleurActu = 4; /* numéro de la couleur par defaut */
    char mode = 0; /* Mode de dessin = 0, choix couleur =1 */

    PrimitiveList primitiveLst = NULL;
    addPrimitive(allocPrimitive(GL_POINTS), &primitiveLst);
    Point *tmpPoint = NULL;

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

        if (mode == 1) {
            selectColorView();
        } else {
            drawPrimitives(primitiveLst);
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
            /* Quelques exemples de traitement d'evenements : */
            switch (e.type) {

                    /*On redimensionne le viewport*/
                case SDL_VIDEORESIZE:
                    WINDOW_WIDTH = e.resize.w;
                    WINDOW_HEIGHT = e.resize.h;
                    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();
                    gluOrtho2D(-1., 1., -1., 1.);
                    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE); /* FIX ME - Écran en partie noir pendant le redimensionnement */
                    break;


                    /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    /*
                    glClearColor((e.button.x/255)%255,((e.button.y/255)%255),0,1);
                     */

                    if (mode == 1) {
                        couleurActu = 2 * (int) (e.button.x / (WINDOW_WIDTH * 2 / NB_COULEURS));
                        if (e.button.y >= WINDOW_HEIGHT / 2) {
                            couleurActu++;
                        }
                    } else {
                        tmpPoint = allocPoint(-1 + 2. * e.button.x / WINDOW_WIDTH, -(-1 + 2. * e.button.y / WINDOW_HEIGHT), COULEURS[couleurActu][0], COULEURS[couleurActu][1], COULEURS[couleurActu][2]);
                        addPointToList(tmpPoint, &(findLastPrimitive(primitiveLst))->points);
                    }

                    break;

                    /* Mouvement de souris */
                case SDL_MOUSEMOTION:
                    /*
                    glClearColor((e.button.x%255)/255.0,((e.button.y%255)/255.0),0,1);
                     */
                    break;

                    /* Touche clavier */
                case SDL_KEYUP:
                    /* printf("touche pressée (code = %d)\n", e.key.keysym.sym); */
                    if (e.key.keysym.sym == SDLK_SPACE) {
                        mode = 0;
                    } else {
                        switch (e.key.keysym.sym) {
                            case SDLK_p:
                                addPrimitive(allocPrimitive(GL_POINTS), &primitiveLst);
                                break;
                            case SDLK_l:
                                addPrimitive(allocPrimitive(GL_LINES), &primitiveLst);
                                break;
                            case SDLK_t:
                                addPrimitive(allocPrimitive(GL_TRIANGLES), &primitiveLst);
                                break;
                            case SDLK_c:
                                addPrimitive(allocPrimitive(GL_QUADS), &primitiveLst);
                                break;
                            case SDLK_q:
                                loop = 0;
                                break;
                            default:
                                break;
                        }
                    }
                    break;

                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_SPACE) {
                        mode = 1;
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

    deletePrimitives(&primitiveLst);

    /* Liberation des ressources associées à la SDL */
    SDL_Quit();

    return EXIT_SUCCESS;
}
