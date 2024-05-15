#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define width 800
#define height 600

// Utility macros ntestiw beha el code
#define CHECK_ERROR(test, message) \
    do { \
        if((test)) { \
            fprintf(stderr, "%s\n", (message)); \
            exit(1); \
        } \
    } while(0)

// fonction pour créer des rectangles
SDL_Rect rectangle(int x,int y,int w ,int h){
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
    
	return rect;
}

// fonction qui va charger l'image en une texture lisible par SDL
SDL_Texture *ImporterImage(const char* file_name, SDL_Renderer *renderer){
	SDL_Surface *image = IMG_Load(file_name);
	SDL_Texture * image_texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);
	return image_texture;
}

int main(int argc, char **argv){


// initialisation de SDL
	CHECK_ERROR(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0, SDL_GetError());
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();	
	
// creation d'une fenetre 
	SDL_Window *window = SDL_CreateWindow("CLAVIER", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	CHECK_ERROR(window == NULL, SDL_GetError());

// creation d'un renderer 
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);    
	CHECK_ERROR(renderer == NULL, SDL_GetError());

// chargement de la lettre
	char current[15] = "keyboard/A1.png";
	SDL_Texture * lettre = ImporterImage(current, renderer);
	int lettre_width, lettre_height;
	SDL_QueryTexture(lettre, NULL, NULL, &lettre_width, &lettre_height);
	SDL_Rect lettre_destination = rectangle(350,200,lettre_width,lettre_height);

// parametre initial de la fentre
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Surface* icon = IMG_Load("A2.png");
	SDL_SetWindowIcon(window, icon);
	//dessin
	SDL_RenderClear(renderer); //clear
		
	SDL_RenderCopy(renderer, lettre, NULL, &lettre_destination);

	SDL_RenderPresent(renderer); //present

	
// gestion des evenements 
	SDL_Event event;	//pile pour gerer les evenement 
	bool hold=false;	//verifie si la souris est enfoncée
	bool fin = false; 	//verifie si le programme est en marche


	while (!fin){
		SDL_WaitEvent(&event);	//attendre indefiniment un evenement	

		bool check = (event.motion.x<=(lettre_destination.x+lettre_destination.w))&&(event.motion.x>=lettre_destination.x)&&(event.motion.y<=(lettre_destination.y+lettre_destination.h))&&(event.motion.y>=lettre_destination.y) ;

		if (event.type == SDL_QUIT){	//fermeture
			fin = true;
		}

		else if (event.type == SDL_MOUSEMOTION){
			if ((check&&(current[10] == '2'))||(!check&&(current[10] == '1'))||hold){
				continue;
			}
			else if (check){
				current[10] = '2';
				lettre = ImporterImage(current, renderer);	
			}
			else if (!check){
				current[10] = '1';
				lettre = ImporterImage(current, renderer);
			}

		}	

		else if ((event.type == SDL_MOUSEBUTTONDOWN)&&check){
			current[10] = '3';
			lettre = ImporterImage(current, renderer);
			hold=true;
		}
		
		else if ((event.type == SDL_MOUSEBUTTONUP)&&hold){
			current[10] = check ? '2':'1';
			lettre = ImporterImage(current, renderer);
			hold=false;
		}
		else{
			continue;
		}
		
		
		//dessin
//		SDL_RenderClear(renderer); //clear
		
		SDL_RenderCopy(renderer, lettre, NULL, &lettre_destination);

		SDL_RenderPresent(renderer); //present
				
		
	}
	
// liberation et destruction
	SDL_DestroyTexture(lettre);
	SDL_FreeSurface(icon);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
