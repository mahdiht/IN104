#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "fonctions.h"



void initbravo(){
	// Afficher le titre du menu principal
	afficherTexte(renderer, font, "WELL PLAYED !", 280, 170);

    SDL_Rect cadrereplay = {250, 240, 300, 50};
    SDL_Rect cadrequitter = {250, 300, 300, 50};

	// Dessiner les cadres rectangulaires colorés en gris pour chaque bouton
	SDL_SetRenderDrawColor(renderer, 150, 50, 100, 40); // Couleur gris clair
	SDL_RenderFillRect(renderer, &cadrereplay);
	SDL_RenderFillRect(renderer, &cadrequitter);

	// Afficher le texte des boutons centrés dans les cadres rectangulaires
	afficherTexte(renderer, font, "REPLAY", cadrereplay.x + cadrereplay.w / 2 - 55, cadrereplay.y + cadrereplay.h / 2 - 16);
	afficherTexte(renderer, font, "QUIT", cadrequitter.x + cadrequitter.w / 2 - 40, cadrequitter.y + cadrequitter.h / 2 - 18);
	
	newscene=false;
}

bool bravo(SDL_Event event){

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			int x = event.button.x;
			int y = event.button.y;
			
			if (x >= 250 && x <= 550 && y >= 240 && y <= 290){
                essais=0;
                stickman[8]='0';
                for (int i = 0; i < NUM_LETTERS; ++i) {
                gButtonState[i] = false;
                }
                
				scene=2;
				newscene=true;
				return false;	
			}	
			else if (x >= 250 && x <= 550 && y >= 300 && y <= 350){
                essais=0;
                stickman[8]='0';
                for (int i = 0; i < NUM_LETTERS; ++i) {
                gButtonState[i] = false;
                }                
				scene=0;
				newscene=true;
				return false;
			}
		}
	}
	return true;	                        

}