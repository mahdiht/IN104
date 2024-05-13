#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "fonctions.h"



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


void initjeu(){

	// chargement de l'image
	image_texture = ImporterImage(stickman, renderer);
	int image_width, image_height;
	SDL_QueryTexture(image_texture, NULL, NULL, &image_width, &image_height);
	SDL_Rect image_destination = rectangle(300,20,image_width,image_height);
	
	SDL_RenderCopy(renderer, image_texture, NULL, &image_destination);	
	
	// initialisation text input
	SDL_StartTextInput();

	
	// affichage text
	afficherTexte(renderer, font, brouillon, 0, 100);
//	afficherTexte(renderer, font, saisi, 0, 200);
}


void destroyjeu(){
	SDL_StopTextInput();
	SDL_DestroyTexture(image_texture);
}
