#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define width 800
#define height 600

// Structure représentant un bouton du clavier virtuel
typedef struct {
	char image[15];	// Image actuelle
	SDL_Rect destination;	// Rectangle délimitant le bouton
	SDL_Texture * texture;	// Destination

} bouton;


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

//fonction pour initialiser le bouton
bouton init_bouton(char lettre, SDL_Renderer *renderer, int x, int y) {
	bouton bouton_instance;

	strcpy(bouton_instance.image, "keyboard/A1.png");
	bouton_instance.image[9] = lettre;
	bouton_instance.texture = ImporterImage(bouton_instance.image, renderer);

	SDL_QueryTexture(bouton_instance.texture, NULL, NULL, &bouton_instance.destination.w, &bouton_instance.destination.h);
	bouton_instance.destination.x = x;
	bouton_instance.destination.y = y;


	return bouton_instance;
}

 //variable globale du clavier
bool hold=false;
bouton clavier[26];

//dessin
void dessiner(SDL_Renderer *renderer){
	SDL_RenderClear(renderer); //clear
	
	for (int i=0; i<26; i++){
		SDL_RenderCopy(renderer, clavier[i].texture, NULL, &clavier[i].destination);
	}

	SDL_RenderPresent(renderer); //present
}

// animer bouton
bool bouton_anim(SDL_Event event, SDL_Renderer *renderer,bouton *b){
	bool check = (event.motion.x<=(b->destination.x+b->destination.w))&&(event.motion.x>=(b->destination.x))&&(event.motion.y<=(b->destination.y+b->destination.h))&&(event.motion.y>=(b->destination.y)) ;

	if (event.type == SDL_MOUSEMOTION){
		if ((check && (b->image[10] == '2')) || (!check && (b->image[10] == '1')) || hold){
			return true;
		}
		else if (check){
			for (int i=0; i<26; i++){
				if (clavier[i].image[10] == '2'){
					clavier[i].image[10] = '1';
					clavier[i].texture = ImporterImage(clavier[i].image, renderer);
					printf("uncheck %c\n",clavier[i].image[9]);
					break;
				}
			}		
			b->image[10] = '2';
			b->texture = ImporterImage(b->image, renderer);
			dessiner(renderer);
			printf("check %c\n",b->image[9]);			
			return true;
		}
//		else if (!check){
//			b->image[10] = '1';
//			b->texture = ImporterImage(b->image, renderer);
//			dessiner(renderer);
//			printf("uncheck %c\n",b->image[9]);				
//			return true;
//
//		}
	}
	else if ((event.type == SDL_MOUSEBUTTONDOWN) && check){
			b->image[10] = '3';
			b->texture = ImporterImage(b->image, renderer);
			hold = true;
			printf("PRESS %c\n",b->image[9]);
			
			dessiner(renderer);
		
			while (true){
				SDL_WaitEvent(&event);	//attendre indefiniment un evenement	
				check = (event.motion.x<=(b->destination.x+b->destination.w))&&(event.motion.x>=(b->destination.x))&&(event.motion.y<=(b->destination.y+b->destination.h))&&(event.motion.y>=(b->destination.y)) ;
				if ((event.type == SDL_MOUSEBUTTONUP) && hold){
				b->image[10] = check ? '2' : '1';
				b->texture = ImporterImage(b->image, renderer);
				dessiner(renderer);
				hold = false;
				puts("button up");
				break;
				}
			}		

	}
	return false;
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

// chargement du clavier
	int x=0;
	int y=0;
	for (int i=0; i<26; i++){

		clavier[i]=init_bouton('A'+i,renderer,x,y);
		x= (i == 12) ? 0 : x+clavier[i].destination.h;
		if (i==12){
			y+=clavier[i].destination.w;
		}
	}

// parametre initial de la fentre
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Surface* icon = IMG_Load("A2.png");
	SDL_SetWindowIcon(window, icon);
	dessiner(renderer);


	
// gestion des evenements 
	SDL_Event event;	//pile pour gerer les evenement 
	bool fin = false; 	//verifie si le programme est en marche


	while (!fin){
		SDL_WaitEvent(&event);	//attendre indefiniment un evenement	

		if (event.type == SDL_QUIT){	//fermeture
			fin = true;
		}
		for (int i=0; i<26; i++){
			if (bouton_anim(event, renderer,&clavier[i])) {
				continue;
			}
		}
		
	}
	
// liberation et destruction
	for (int i=0; i<26; i++){
		SDL_DestroyTexture(clavier[i].texture);
	}
	SDL_FreeSurface(icon);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
