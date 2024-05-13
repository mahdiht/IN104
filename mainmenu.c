#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "fonctions.h"



void initmenu(){
	// Afficher le titre du menu principal
	afficherTexte(renderer, font, "Menu Principal", 300, 50);

	 // Définir les dimensions et la position des cadres rectangulaires pour chaque bouton
	SDL_Rect cadreJouer = {250, 150, 300, 50};
	SDL_Rect cadreOptions = {250, 250, 300, 50};

	// Dessiner les cadres rectangulaires colorés en gris pour chaque bouton
	SDL_SetRenderDrawColor(renderer, 150, 50, 100, 40); // Couleur gris clair
	SDL_RenderFillRect(renderer, &cadreJouer);
	SDL_RenderFillRect(renderer, &cadreOptions);

	// Afficher le texte des boutons centrés dans les cadres rectangulaires
	afficherTexte(renderer, font, "Jouer", cadreJouer.x + cadreJouer.w / 2 - 30, cadreJouer.y + cadreJouer.h / 2 - 12);
	afficherTexte(renderer, font, "Options", cadreOptions.x + cadreOptions.w / 2 - 40, cadreOptions.y + cadreOptions.h / 2 - 12);
}

void menu(SDL_Event event){

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			int x = event.button.x;
			int y = event.button.y;
			
			if (x >= 250 && x <= 550 && y >= 150 && y <= 200) 
				scene=2;
			else if (x >= 250 && x <= 550 && y >= 250 && y <= 300)
				scene=1;
		}
	}	                        

}
