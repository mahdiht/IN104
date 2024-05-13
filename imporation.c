#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "fonctions.h"

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


// Fonction pour afficher un texte à l'écran
void afficherTexte(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y) {
	SDL_Color couleur = {0, 0, 0}; // Couleur du texte (noir)
	SDL_Surface *surface = TTF_RenderText_Blended(font, texte, couleur);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    
	SDL_Rect destRect = {x, y, surface->w, surface->h};
	SDL_RenderCopy(renderer, texture, NULL, &destRect);
    
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

// Fonction pour afficher l'arrière-plan avec une texture
void afficherArrierePlan(SDL_Texture *backgroundTexture) {
    // Obtenir les dimensions de la fenêtre
	int largeurFenetre, hauteurFenetre;
	SDL_GetRendererOutputSize(renderer, &largeurFenetre, &hauteurFenetre);

    // Obtenir les dimensions de la texture d'arrière-plan
	int largeurTexture, hauteurTexture;
	SDL_QueryTexture(backgroundTexture, NULL, NULL, &largeurTexture, &hauteurTexture);

    // Calculer les dimensions et la position de l'arrière-plan pour le centrer dans la fenêtre
	int x = (largeurFenetre - largeurTexture) / 2;
	int y = (hauteurFenetre - hauteurTexture) / 2;

    // Afficher l'arrière-plan
	SDL_Rect destRect = {x, y, largeurTexture, hauteurTexture};
	SDL_RenderCopy(renderer, backgroundTexture, NULL, &destRect);
}
