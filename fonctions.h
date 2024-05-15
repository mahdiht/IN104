#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string.h>

//fenetre
#define width 800
#define height 600

//clavier
#define BUTTON_WIDTH 40
#define BUTTON_HEIGHT 40
#define NUM_LETTERS 26
#define LETTERS_PER_ROW_1 10
#define LETTERS_PER_ROW_2 10
#define LETTERS_PER_ROW_3 6


//varaibles du jeu	
extern char word[10];		//le mot a devier
extern char hint[80];		//indice
extern int essais;		//nombre dessais
extern char brouillon[10];	//brouillon
extern char stickman[14];	//image du pendu (nom du fichier image)
extern char difficulte[13];	//niveau de diffuclte (nom du fichier des mots)

//lecture a partir du clavier
extern char saisi[10];
extern int len_s;

//paramtere de la fenetre
extern SDL_Window *window;			//fenetre
extern SDL_Renderer *renderer;		//renderer
extern SDL_Surface* icon;			//icone
extern int scene;				//gestion de la scene actuel
extern SDL_Texture * backgroundTexture;	//arriere plan
extern TTF_Font *font;				//police principale
extern bool newscene;				//verifie si une nouvelle scene a ete ouverte


// hangman.png
extern SDL_Texture * image_texture;		//texture de l'image du pendu
extern SDL_Rect image_destination;		//destination de la texture

//varibales global du clavier
extern char gLetters[NUM_LETTERS];		//list des lettres
extern TTF_Font* gFont;		//police du bouton
extern SDL_Color buttonHoverColor;		// couleur lorsque la souris est au dessu
extern SDL_Color buttonDefaultColor; 		// couleur par defaut
extern SDL_Color buttonPressedColorAorH; 	// couleur click correct
extern SDL_Color buttonPressedColorOther;	// couleur click incorrect

extern bool gButtonState[NUM_LETTERS]; 	//activation ou desactivation des boutons

extern int gHoveredButton;	//quel bouton se trouve sous la souris
extern bool gMouseOverButton; //verification que la souris est au dessus dun bouton

//fonctions.c
void separateWords(char *input_string);		//separer le mot de son indice a partir d'une ligne du fichier texte
void lireligneAleatoire(const char* nomFichier);	//tirer une ligne aleatoire du fichier texte


//init.c
void init();		//initialiser sdl et la fenetre
void destroy();	//detruire sdl et la fenetre


//Imporation.c
SDL_Texture *ImporterImage(const char* file_name, SDL_Renderer *renderer);				//importer une image en une texture utilisable
SDL_Rect rectangle(int x,int y,int w ,int h);								//instance d'un rectangle 
void afficherTexte(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y);	//afficher du texte dans la fenetre
void afficherArrierePlan(SDL_Texture *backgroundTexture);						//afficher l'arriere plan

//play.c
void choixmot();		//choic aleatoire du mot (emploi les fonctions de fonctions.c)
void hangman(char* saisi);	//base du jeu du pendu 
void initjeu();		//initalise la fenetre du jeu
void destroyjeu();		//libere les variable de la fenetre du jeu
bool play(SDL_Event event);	//gestion des evenements du jeu

//mainmenu.c
void initmenu();		//initialise la fenetre du menu principal	
bool menu(SDL_Event event);	//gestion des evenements de la fenetre principal

//option.c
void initoption();		//initalise la fenetre des options
bool option(SDL_Event event);	//gestion des evenements des options

//clavier.c
void createButtons();				//creation des boutons
void checkMouseOverButton(int x, int y);	//verification que la souris est au dessus du clavier
void drawKeyboard();				//dessin du clavier
void initclavier();				//initalisation de la police du clavier
void destroyclavier();				//liberer les variables du clavier

//initgameover.c
void initgameover();		//initalisation de la fenetre game over
bool gameover(SDL_Event);	//gestion des evenements de game over

//initbravo.c
void initbravo();
bool bravo(SDL_Event);
