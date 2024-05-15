#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string.h>

#define width 800
#define height 600





	
extern char word[10];
extern char hint[80];
extern int essais;
extern char brouillon[10];
extern char stickman[14];
extern char difficulte[13];

extern char saisi[10];
extern int len_s;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Surface* icon;
extern int scene;
extern SDL_Texture * backgroundTexture;
extern TTF_Font *font;
extern bool newscene;


// jeu
extern SDL_Texture * image_texture;
extern SDL_Rect image_destination;

//fonctions.c
void separateWords(char *input_string);
void lireligneAleatoire(const char* nomFichier);


//init.c
void init();
void destroy();


//Imporation.c
SDL_Texture *ImporterImage(const char* file_name, SDL_Renderer *renderer);
SDL_Rect rectangle(int x,int y,int w ,int h);
void afficherTexte(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y);
void afficherArrierePlan(SDL_Texture *backgroundTexture);

//play.c
void choixmot();
void hangman(char* saisi);
void initjeu();
void destroyjeu();
bool play(SDL_Event event);

//mainmenu.c
void initmenu();
bool menu(SDL_Event event);

//option.c
void initoption();
bool option(SDL_Event event);	
