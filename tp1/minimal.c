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
    {0, 0, 0},
    {100, 100, 100},
    {206, 206, 206},
    {239, 239, 239},
    {255, 255, 255},
    {173, 57, 14},
    {30, 127, 203},
    {127, 221, 76},
    {167, 103, 38},
    {136, 77, 167},
    {230, 126, 48}
};

/* Nombre de couleurs */
static const unsigned int NB_COULEURS = sizeof(COULEURS) / 3;

int main(int argc, char** argv) {

    int couleurActu = 1; /* numéro de la couleur par defaut */
    char mode = 'p'; /* Mode de dessin ( p = points, l = lignes, t = triangle, c = couleur (menu) */

    /* Initialisation de la SDL */
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /* Désactivation du double buffering */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);

    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE )) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Ceci est un Titre", NULL);

    /* Placer ici le code de dessin */
    glClear(GL_COLOR_BUFFER_BIT);


    /* Boucle d'affichage */
    int loop = 1;
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        /* Echange du front et du back buffer : mise à jour de la fenêtre */
        SDL_GL_SwapBuffers();

        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) {


            /* L'utilisateur ferme la fenêtre : */
            if(e.type == SDL_QUIT) {
                loop = 0;
                break;
            }
            /* Quelques exemples de traitement d'evenements : */
            switch(e.type) {

                    /*On redimensionne le viewport*/
                case SDL_VIDEORESIZE:
                    WINDOW_WIDTH = e.resize.w;
                    WINDOW_HEIGHT=e.resize.h;
                    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();
                    gluOrtho2D(-1., 1., -1., 1.);
                    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE ); /* FIX ME - Écran en partie noir pendant le redimensionnement */
                    break;


                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    /*
                    glClearColor((e.button.x/255)%255,((e.button.y/255)%255),0,1);
                    */
                    glBegin(GL_POINTS);
                        glColor3ub(255, 255, 255);
                        glVertex2f(-1 + 2. * e.button.x / WINDOW_WIDTH, - (-1 + 2. * e.button.y / WINDOW_HEIGHT));
                    glEnd();
                    SDL_GL_SwapBuffers();
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

                    switch(e.key.keysym.sym) {
                        case SDLK_p:
                            mode = 'p';
                            break;
                        case SDLK_l:
                            mode = 'l';
                            break;
                        case SDLK_t:
                            mode = 't';
                            break;
                        case SDLK_q:
                            loop=0;
                            break;
                        default:
                            break;
                    }
                    break;

                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_SPACE) {
                        mode = 'c';
                    }
                    break;
                default:
                    break;
            }
        }

        /* Calcul du temps écoulé */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        /* Si trop peu de temps s'est écoulé, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Liberation des ressources associées à la SDL */
        SDL_Quit();

    return EXIT_SUCCESS;
}
