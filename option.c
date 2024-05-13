#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "fonctions.h"   
    
    
void initoption(){
    
    
	afficherTexte(renderer, font, "Options", 300, 50);
	char text[9]="3 Lettres";
	SDL_SetRenderDrawColor(renderer, 150, 50, 100, 40); // Couleur gris clair
	for (int i = 3; i < 8; i++) {    
		text[0]='0'+i;	
		SDL_Rect cadre = {250, 120+(i-3)*60, 300, 50};
		
		SDL_RenderFillRect(renderer, &cadre);
		
		afficherTexte(renderer, font, text, cadre.x + cadre.w / 2 - 40, cadre.y + cadre.h / 2 - 12);
	}
}


void option(SDL_Event event){

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			int x = event.button.x;
			int y = event.button.y;
			
			for (int i = 3; i < 8; i++) {  
				if (x >= 250 && x <= 550 && y >= (120+(i-3)*60) && y <= (170+(i-3)*60)) {
					difficulte[0]='0'+i;
					scene=0;
					break;
				}	
 			}                       
		}
	}	
}


