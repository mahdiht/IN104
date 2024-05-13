#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "fonctions.h"



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
	char difficulte[12]="3lettres.txt";
	
//saisi clavier	
	char saisi[MAX_WORD_LENGTH] = {0};
	int len_s=0;

//textures	
	SDL_Texture * backgroundTexture;
	SDL_Texture * image_texture;


//  initialisation de la fenetre comme variable global

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface* icon;
	int scene=0;
	TTF_Font *font;
	

int main(int argc, char **argv){

	init();

// initialisation du brouillon
	for (int i = 0; i < strlen(word); i++) {
		brouillon[i] = '_';
	}
	brouillon[strlen(word)] = '\0';	
	

	
// initialisation police 
	font = TTF_OpenFont("font.ttf", 24);



	
// gestion des evenements 
	SDL_Event event;	//file pour gerer les evenement 
	bool fin = false; 	//verifie si le programme est en marche
	while (!fin){
		
		SDL_RenderClear(renderer); //clear
		afficherArrierePlan(backgroundTexture); //arriere plan
		
		SDL_WaitEvent(&event);	//attendre indefiniment un evenement
		
		if (event.type == SDL_QUIT)	//fermeture
			fin = true;	
		else{
			switch(scene){
				case 0:	//menu principal
					initmenu();
					menu(event);
					break;
				 
				case 1:	//options
					initoption();
					option(event);
					break;
				
				default:	//jeu
					initjeu();
					break;
			}
		} 
		
		
		

		
		
				
		SDL_RenderPresent(renderer); //present
		
	}

// liberation et destruction
	destroyjeu();
	TTF_CloseFont(font);
	destroy();
	

	return 0;
}
