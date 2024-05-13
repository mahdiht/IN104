#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define width 800
#define height 600

#define MAX_WORD_LENGTH 200
#define LEN_MAX 25 //9adeh el utilisateur inajem yekteb men 7arf nbadloha mba3d nrodoha tetbadel bel difficulteet


extern const char* word;
extern int essais;
extern char brouillon[10];
extern char stickman[13];
extern char difficulte[12];

extern char saisi[MAX_WORD_LENGTH];
extern int len_s;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Surface* icon;
extern int scene;
extern SDL_Texture * backgroundTexture;
extern TTF_Font *font;


// jeu
extern SDL_Texture * image_texture;

//fonctions.c
void separateWords(char *input_string, char **word, char **hint);
void lireligneAleatoire(const char* nomFichier,char **word, char **hint);


//init.c
void init();
void destroy();


//Imporation.c
SDL_Texture *ImporterImage(const char* file_name, SDL_Renderer *renderer);
SDL_Rect rectangle(int x,int y,int w ,int h);
void afficherTexte(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y);
void afficherArrierePlan(SDL_Texture *backgroundTexture);

//play.c
void hangman(char* saisi);
void initjeu();
void destroyjeu();

//mainmenu.c
void initmenu();
void menu(SDL_Event event);

//option.c
void initoption();
void option(SDL_Event event);
