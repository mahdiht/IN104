#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "include/SDL.h"
#include "include/SDL_image.h"
#include "include/SDL_ttf.h"

#define width 800
#define height 600

#define LEN_MAX 7 //9adeh el utilisateur inajem yekteb men 7arf nbadloha mba3d nrodoha tetbadel bel difficulteet

// Utility macros ntestiw beha el code
#define CHECK_ERROR(test, message) \
    do { \
        if((test)) { \
            fprintf(stderr, "%s\n", (message)); \
            exit(1); \
        } \
    } while(0)

// initilisation jeu hangman comme variables globals
	const char* word= "bonjour";  //nbadolha mba3d ki na9raw 7seb el dictionnaire
	int essais=0;
	char brouillon[10];
	char stickman[13] = "Hangman_0.png";


// fonction qui va charger l'image en une texture lisible par SDL
SDL_Texture *ImporterImage(const char* file_name, SDL_Renderer *renderer){
	SDL_Surface *image = IMG_Load(file_name);
	SDL_Texture * image_texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);
	return image_texture;
}

// fonction pour créer des rectangles
SDL_Rect rectangle(int x,int y,int w ,int h){
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
    
	return rect;
}

// fonction pour creer le text et l'espace qui lui sera accordé
void Ecrire(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *destination) {
	int text_width;
	int text_height;
	SDL_Surface *surface;
	
	if (text[0] == '\0'){
		SDL_Color textColor = {255, 255, 255, 255};	// solution mel 7it taw n5amemou fi 7al 5ir mba3d 
		surface = TTF_RenderText_Solid(font, "Hello", textColor);
	}
	else {
		SDL_Color textColor = {0, 0, 0, 255};
		surface = TTF_RenderText_Solid(font, text, textColor);
	}
	
	*texture = SDL_CreateTextureFromSurface(renderer, surface);
	text_width = surface->w;
	text_height = surface->h;
	SDL_FreeSurface(surface);

	*destination = rectangle(x,y,text_width,text_height);
}

void hangman(char* saisi){
		
	if (strlen(saisi)==1){
		if (strchr(word, saisi[0]) != NULL) {
			puts("Bravo, la lettre est dans le mot !");
			for (int i = 0; i < strlen(word); i++) {
				if (word[i] == saisi[0]) {
					brouillon[i] = saisi[0];
				}
			}
		}
		else {
			essais++;
		}
	}
		
	else{

		if (strcmp(saisi, word) == 0) {
			strcpy(brouillon,saisi);
		}
		else{
			puts("ce n'est pas le mot");
			essais++;
		}
	}
	
}	

int main(int argc, char **argv){

// initialisation du brouillon
	for (int i = 0; i < strlen(word); i++) {
		brouillon[i] = '_';
	}
	brouillon[strlen(word)] = '\0';	
	


// initialisation de SDL
	CHECK_ERROR(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0, SDL_GetError());
	IMG_Init(IMG_INIT_PNG);
	
// creation d'une fenetre 
	SDL_Window *window = SDL_CreateWindow("HANGMAN TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	CHECK_ERROR(window == NULL, SDL_GetError());

// creation d'un renderer 
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);    
	CHECK_ERROR(renderer == NULL, SDL_GetError());


// chargement de l'image
	SDL_Texture * image_texture = ImporterImage(stickman, renderer);
	int image_width, image_height;
	SDL_QueryTexture(image_texture, NULL, NULL, &image_width, &image_height);
	SDL_Rect image_destination = rectangle(100,50,image_width,image_height);

// parametre initial de la fentre
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
// initialisation TTF
	TTF_Init();
	
// initialisation text input
	SDL_StartTextInput();
	char saisi[LEN_MAX + 1] = {0};
	int len=0;
	
	
// initialisation police 
	char *font_path;
	font_path ="font.ttf"; //lezem nbadloha ama rasi wja3ni donc hay hezi ama mech mezyena
	TTF_Font *font = TTF_OpenFont(font_path, 24);

// initialisation affichage brouillon
	SDL_Rect brouillon_destination;
	SDL_Texture *brouillon_texture;

	Ecrire(renderer, 0, image_destination.y + image_destination.h, brouillon, font, &brouillon_texture, &brouillon_destination);

// initialisation affichage texte de saisie
	SDL_Rect texte_destination;
	SDL_Texture *texte_texture;

	Ecrire(renderer, 0, brouillon_destination.y + brouillon_destination.h, saisi, font, &texte_texture, &texte_destination);


	
// gestion des evenements 
	SDL_Event event;	//pile pour gerer les evenement 
	bool fin = false; 	//verifie si le programme est en marche
	while (!fin){
		bool asaisi=false; 	//verifie si l'utilisateur a entree du texte
		
		SDL_WaitEvent(&event);	//attendre indefiniment un evenement
		
		if (event.type == SDL_QUIT){	//fermeture
			fin = true;
		}
		
		else if(event.type == SDL_KEYDOWN){	//pression de touche
			
			if (event.key.keysym.sym == SDLK_BACKSPACE && len){	//effacer
				saisi[len - 1] = 0;
				len--;
				asaisi=true;
			}
			
			else if (event.key.keysym.sym==SDLK_RETURN && len){  //confirmer la saisie
				hangman(saisi);
				
				Ecrire(renderer, 0, image_destination.y + image_destination.h, brouillon, font, &brouillon_texture, &brouillon_destination);
			
			
			//verification fin du jeu	
			if (strcmp(word, brouillon) == 0) {
				puts("Bravo, vous avez trouvé le mot !");
			}	
			else if(essais<6){
				stickman[8]='0'+essais;
				image_texture = ImporterImage(stickman, renderer);
				char message[50];
    				sprintf(message, "Il vous reste %d essais\n", 6 - essais);
				puts(message);
			}
			else{
				stickman[8]='0'+6;
				image_texture = ImporterImage(stickman, renderer);
				puts("Game over");
			}
										
				memset(saisi, 0, sizeof(saisi));	//vider saisi completement
				len=0;
				asaisi=true;			
			}
		
		}
		else if (event.type == SDL_TEXTINPUT){	//saisie de texte
			int l = strlen(event.text.text);
			
			int l1;
			if (len + l < LEN_MAX){
				l1=l;
			}
			else {
				l1=LEN_MAX-len;
			}
						
			strncpy(saisi + len, event.text.text, l1);
			len += l1;
			asaisi=true;
		}
			
		if (asaisi){
			puts(saisi);
			Ecrire(renderer, 0, brouillon_destination.y + brouillon_destination.h, saisi, font, &texte_texture, &texte_destination);
		}
			
		//dessin
		SDL_RenderClear(renderer); //clear
		
		SDL_RenderCopy(renderer, image_texture, NULL, &image_destination);
		SDL_RenderCopy(renderer, brouillon_texture, NULL, &brouillon_destination);
		SDL_RenderCopy(renderer, texte_texture, NULL, &texte_destination);
			
		SDL_RenderPresent(renderer); //present
		

	}

// liberation et destruction
	SDL_StopTextInput();
	SDL_DestroyTexture(texte_texture);
	SDL_DestroyTexture(brouillon_texture);
	SDL_DestroyTexture(image_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
