#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>
#include "fonctions.h"

#define width 800
#define height 600
#define MAX_WORD_LENGTH 200
#define LEN_MAX 7 //9adeh el utilisateur inajem yekteb men 7arf nbadloha mba3d nrodoha tetbadel bel difficulteet

// Utility macros ntestiw beha el code
#define CHECK_ERROR(test, message) \
    do { \
        if((test)) { \
            fprintf(stderr, "%s\n", (message)); \
            exit(1); \
        } \
    } while(0)




// initilisation jeu hangman comme variables globals

	int essais=0;
	char brouillon[20];
	char stickman[13] = "Hangman_0.png";
	char nomFichier[15]="mlettres.txt";


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

// fonction pour creer le text et l'espace qui lui sera accordé
void Ecrire(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *destination) {
	int text_width;
	int text_height;
	SDL_Surface *surface;
	
	if (text[0] == '\0'){
		SDL_Color textColor = {255, 255, 255, 255};	// solution mel 7it taw n5amemou fi 7al 5ir mba3d 
		surface = TTF_RenderText_Solid(font, "Hello", textColor);
	}
	else {
		SDL_Color textColor = {0, 0, 0, 255};
		surface = TTF_RenderText_Solid(font, text, textColor);
	}
	
	*texture = SDL_CreateTextureFromSurface(renderer, surface);
	text_width = surface->w;
	text_height = surface->h;
	SDL_FreeSurface(surface);

	*destination = rectangle(x,y,text_width,text_height);
}

void hangman(char* saisi,char* word){
		
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

void init_game(char c){
	nomFichier[0]=c;
	char *word;
	char *hint;

    lireligneAleatoire(nomFichier,&word,&hint);
	printf("%s",word);
	printf("%s",hint);

// initialisation du brouillon
	for (int i = 0; i < strlen(word); i++) {
		brouillon[i] = '_';
	}
	brouillon[strlen(word)] = '\0';	
	


// initialisation de SDL
	CHECK_ERROR(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0, SDL_GetError());
	IMG_Init(IMG_INIT_PNG);
	
// creation d'une fenetre 
	SDL_Window *window = SDL_CreateWindow("HANGMAN TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	CHECK_ERROR(window == NULL, SDL_GetError());

// creation d'un renderer 
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);    
	CHECK_ERROR(renderer == NULL, SDL_GetError());


// chargement de l'image
	SDL_Texture * image_texture = ImporterImage(stickman, renderer);
	int image_width, image_height;
	SDL_QueryTexture(image_texture, NULL, NULL, &image_width, &image_height);
	SDL_Rect image_destination = rectangle(100,50,image_width,image_height);

// parametre initial de la fentre
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
// initialisation TTF
	TTF_Init();
	
// initialisation text input
	SDL_StartTextInput();
	char saisi[LEN_MAX + 1] = {0};
	int len=0;
	
	
// initialisation police 
	char *font_path;
	font_path ="font.ttf"; //lezem nbadloha ama rasi wja3ni donc hay hezi ama mech mezyena
	TTF_Font *font = TTF_OpenFont(font_path, 24);

// initialisation affichage brouillon
	SDL_Rect brouillon_destination;
	SDL_Texture *brouillon_texture;

	Ecrire(renderer, 0, image_destination.y + image_destination.h, brouillon, font, &brouillon_texture, &brouillon_destination);

// initialisation affichage texte de saisie
	SDL_Rect texte_destination;
	SDL_Texture *texte_texture;

	Ecrire(renderer, 0, brouillon_destination.y + brouillon_destination.h, saisi, font, &texte_texture, &texte_destination);
// initialisation affichage brouillon
	SDL_Rect hint_destination;
	SDL_Texture *hint_texture;

	Ecrire(renderer, 0, texte_destination.y + texte_destination.h, hint, font, &hint_texture, &hint_destination);


	
// gestion des evenements 
	SDL_Event event;	//pile pour gerer les evenement 
	bool fin = false; 	//verifie si le programme est en marche
	while (!fin){
		bool asaisi=false; 	//verifie si l'utilisateur a entree du texte
		
		SDL_WaitEvent(&event);	//attendre indefiniment un evenement
		
		if (event.type == SDL_QUIT){	//fermeture
			fin = true;
		}
		
		else if(event.type == SDL_KEYDOWN){	//pression de touche
			
			if (event.key.keysym.sym == SDLK_BACKSPACE && len){	//effacer
				saisi[len - 1] = 0;  
				len--;
				asaisi=true;
			}
			
			else if (event.key.keysym.sym==SDLK_RETURN && len){  //confirmer la saisie
				hangman(saisi,word);
				
				Ecrire(renderer, 0, image_destination.y + image_destination.h, brouillon, font, &brouillon_texture, &brouillon_destination);
			
			
			//verification fin du jeu	
			if (strcmp(word, brouillon) == 0) {
				puts("Bravo, vous avez trouvé le mot !");
			}	
			else if(essais<6){
				stickman[8]='0'+essais;
				image_texture = ImporterImage(stickman, renderer);
				char message[50];
    				sprintf(message, "Il vous reste %d essais\n", 6 - essais);
				puts(message);
			}
			else{
				stickman[8]='0'+6;
				image_texture = ImporterImage(stickman, renderer);
				puts("Game over");
			}
										
				memset(saisi, 0, sizeof(saisi));	//vider saisi completement
				len=0;
				asaisi=true;			
			}
		
		}
		else if (event.type == SDL_TEXTINPUT){	//saisie de texte
			int l = strlen(event.text.text);
			
			int l1;
			if (len + l < LEN_MAX){
				l1=l;
			}
			else {
				l1=LEN_MAX-len;
			}
						
			strncpy(saisi + len, event.text.text, l1);
			len += l1;
			asaisi=true;
		}
			
		if (asaisi){
			puts(saisi);
			Ecrire(renderer, 0, brouillon_destination.y + brouillon_destination.h, saisi, font, &texte_texture, &texte_destination);
		}
			
		//dessin
		SDL_RenderClear(renderer); //clear
		
		SDL_RenderCopy(renderer, image_texture, NULL, &image_destination);
		SDL_RenderCopy(renderer, brouillon_texture, NULL, &brouillon_destination);
		SDL_RenderCopy(renderer, texte_texture, NULL, &texte_destination);
		SDL_RenderCopy(renderer, hint_texture, NULL, &hint_destination);
		SDL_RenderPresent(renderer); //present
		

	}

// liberation et destruction
	SDL_StopTextInput();
	SDL_DestroyTexture(texte_texture);
	SDL_DestroyTexture(brouillon_texture);
	SDL_DestroyTexture(image_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return ;
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

// Fonction pour afficher le menu principal
void afficherMenuPrincipal(SDL_Renderer *renderer, TTF_Font *font) {
    afficherTexte(renderer, font, "Menu Principal", 300, 50);
    afficherTexte(renderer, font, "1. Jouer", 300, 150);
    afficherTexte(renderer, font, "2. Options", 300, 200);
}
void afficherOptions(SDL_Renderer *renderer, TTF_Font *font) {
    afficherTexte(renderer, font, "Options", 300, 50);
    afficherTexte(renderer, font, "3 Lettres", 300, 150);
    afficherTexte(renderer, font, "4 Lettres", 300, 200);
    afficherTexte(renderer, font, "5 Lettres", 300, 250);
    afficherTexte(renderer, font, "6 Lettres", 300, 300);
    afficherTexte(renderer, font, "7 Lettres", 300, 350);
    afficherTexte(renderer, font, "8 Lettres", 300, 400);
}
int main(int argc, char *argv[]) {
    // Initialisation de SDL
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    
    // Création de la fenêtre et du renderer
    SDL_Window *window = SDL_CreateWindow("Menu Principal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charger la police
    TTF_Font *font = TTF_OpenFont("font.ttf", 36);
    
    // Vérifier si la police a bien été chargée
    if (!font) {
        printf("Erreur lors du chargement de la police : %s\n", TTF_GetError());
        return 1;
    }

    // Boucle principale
    int quitter = 0;
    int jouer = 0;
	int optionsActive = 0;
	char c;
    while (!quitter) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitter = 1;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;
                    if (!optionsActive) {
                        if (x >= 300 && x <= 500 && y >= 150 && y <= 180) {
                            jouer = 1;
                        } else if (x >= 300 && x <= 500 && y >= 200 && y <= 230) {
                            optionsActive = 1;
                        }
                    } else {
                        if (x >= 300 && x <= 500 && y >= 150 && y <= 180) {
                            c='3';
                        } else if (x >= 300 && x <= 500 && y >= 200 && y <= 230) {
                            c='4';
                        } else if (x >= 300 && x <= 500 && y >= 250 && y <= 280) {
                            c='5';
                        } else if (x >= 300 && x <= 500 && y >= 300 && y <= 330) {
                            c='6';
                        } else if (x >= 300 && x <= 500 && y >= 350 && y <= 380) {
                            c='7';
                        } else if (x >= 300 && x <= 500 && y >= 400 && y <= 430) {
                            c='8';
                        }
                        optionsActive = 0; // Fermer le sous-menu Options après avoir cliqué sur une option
                    }
                }
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        if (!jouer && !optionsActive) {
            afficherMenuPrincipal(renderer, font);
        } else if (optionsActive) {
            afficherOptions(renderer, font);
        }
         else if (jouer) {
            // Code pour démarrer le jeu
            // Ici, vous pouvez ouvrir une nouvelle fenêtre ou changer le contenu de la fenêtre actuelle pour afficher le jeu.
            // Par souci de simplicité, je vais juste afficher "Jeu en cours..." dans la même fenêtre.
            init_game(c);
        }

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }

    // Libérer la mémoire et quitter SDL
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
