#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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
	char word[10];
	char hint[80];
	int essais=0;
	char brouillon[10];
	char stickman[14] = "Hangman_0.png";
	char difficulte[13]="5lettres.txt";
	
//saisi clavier	
	char saisi[10] = {0};
	int len_s=0;

//textures	
	SDL_Texture * backgroundTexture;
	SDL_Texture * image_texture;
	SDL_Rect image_destination;


//  initialisation de la fenetre comme variable global

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface* icon;
	int scene=0;
	TTF_Font *font;
	bool newscene=true;

int main(int argc, char **argv){


	init();	//initialisation SDL 
	

	
// initialisation police 
	font = TTF_OpenFont("font.ttf", 24);
	


	
// gestion des evenements 
	afficherArrierePlan(backgroundTexture); //initialisation arriere plan
	
	SDL_Event event;	//file pour gerer les evenement 
	bool fin = false; 	//verifie si le programme est en marche
	while (!fin){
		
		SDL_WaitEvent(&event);	//attendre indefiniment un evenement
		
		if (event.type == SDL_QUIT){	//fermeture
			fin = true;
			break;
		}	
		else if (!newscene){
			if(scene==0){	//menu principal
				if (menu(event))
					continue;
			}
			else if(scene==1){	//options	
				if (option(event))
					continue;
			}
			else{	//jeu
				if (play(event))
					continue;

			}
		}
		
		if(newscene){
			SDL_RenderClear(renderer); //clear
			afficherArrierePlan(backgroundTexture); //arriere plan
			if(scene==0){	//menu principal
				initmenu();
			}
			else if(scene==1){	//options	
				initoption();
			}
			else{	//jeu
				choixmot();	//choix du mot
				initjeu();
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
