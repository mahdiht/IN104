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

//initialisation de la fenetre
void init(){

// initialisation de SDL
	CHECK_ERROR(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0, SDL_GetError());
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	
// creation d'une fenetre 
	window = SDL_CreateWindow("HANGMAN TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	CHECK_ERROR(window == NULL, SDL_GetError());

// creation d'un renderer 
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);    
	CHECK_ERROR(renderer == NULL, SDL_GetError());
	
// parametre initial de la fentre
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	icon = IMG_Load("icon.png");
	SDL_SetWindowIcon(window, icon);
	
//arriere plan
	backgroundTexture= ImporterImage("ap.png", renderer);
	

}


// destruction de la fenetre
void destroy(){

	SDL_DestroyTexture(backgroundTexture);
	SDL_FreeSurface(icon);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


