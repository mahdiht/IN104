#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "fonctions.h"

void choixmot(){

	//chargement du mot et de l'indice
	lireligneAleatoire(difficulte);
	// initialisation du brouillon
	strcpy(brouillon,word);
	for (int i = 0; i < strlen(word); i++) {
		brouillon[i] = '_';
	}
	
	printf("%s %s %s\n",brouillon,word,hint);
	
}

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
	
			//verification fin du jeu	
	if (strcmp(word, brouillon) == 0) {
		puts("Bravo, vous avez trouvé le mot !");

	}	
	else if(essais<6){
		stickman[8]='0'+essais;
	
		

		image_texture = ImporterImage(stickman, renderer);

		//puts(message);
	}
	else{
		stickman[8]='0'+6;
		image_texture = ImporterImage(stickman, renderer);
		puts("Game over");

		
	}
				
}


void initjeu(){


	// chargement de l'image
	image_texture = ImporterImage(stickman, renderer);	
	int image_width, image_height;
	SDL_QueryTexture(image_texture, NULL, NULL, &image_width, &image_height);	
	image_destination = rectangle(212,50,image_width,image_height);


	SDL_RenderCopy(renderer, image_texture, NULL, &image_destination);

	
	
	// initialisation text input
	SDL_StartTextInput();

	
	// affichage text
	afficherTexte(renderer, font, brouillon, 400-(strlen(brouillon)/2)*23.4, 300);
	afficherTexte(renderer, font, hint, 400-(strlen(hint)/2)*23.4, 350);
//	afficherTexte(renderer, font, saisi, 0, 200);


	newscene=false;
}


void destroyjeu(){
	SDL_StopTextInput();
	SDL_DestroyTexture(image_texture);
}

bool play(SDL_Event event){
	bool asaisi=false; 	//verifie si l'utilisateur a entree du texte
	
	if(event.type == SDL_KEYDOWN){	//pression de touche
			
		if (event.key.keysym.sym == SDLK_BACKSPACE && len_s){	//effacer
			saisi[len_s - 1] = 0;
			len_s--;
			asaisi=true;
		}
			
		else if (event.key.keysym.sym==SDLK_RETURN && len_s){  //confirmer la saisie
			hangman(saisi);				
											
			memset(saisi, 0, sizeof(saisi));	//vider saisi completement
			len_s=0;
			asaisi=true;			
		}
		else 
			return true;
		
	}
	else if (event.type == SDL_TEXTINPUT){	//saisie de texte
		int l = strlen(event.text.text);
			
		int l1;
		if (len_s + l < strlen(word)){
			l1=l;
		}
		else {
			l1=strlen(word)-len_s;
		}
						
		strncpy(saisi + len_s, event.text.text, l1);
		len_s += l1;
		asaisi=true;
	}
	else if(event.type == SDL_MOUSEBUTTONDOWN) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		checkMouseOverButton(x, y);
		if (gMouseOverButton) {
			//convertir en chaine
			char temp[2];
			temp[0]= gLetters[gHoveredButton] + 'a' - 'A';
			temp[1] = '\0';
			
			hangman(temp);
			printf("Letter clicked: %c\n", gLetters[gHoveredButton]);
			gButtonState[gHoveredButton] = !gButtonState[gHoveredButton]; // Inverser l'état du bouton
		}
	}
	else if (event.type == SDL_MOUSEMOTION){
		int x, y;
		SDL_GetMouseState(&x, &y);
		checkMouseOverButton(x, y);
	}
	else
		return true;
			
	if (asaisi){
		puts(saisi);	
	}
			
	//dessin
//	SDL_RenderClear(renderer); //clear
	afficherArrierePlan(backgroundTexture); //arriere plan

	SDL_RenderCopy(renderer, image_texture, NULL, &image_destination);
	
	afficherTexte(renderer, font, brouillon, 400-(strlen(brouillon)/2)*23.4, 300);
	afficherTexte(renderer, font, hint, 400-(strlen(hint)/2)*23.4, 350);
	if ((essais>0)&&(essais<6)){
			char scoreText[50];
    sprintf(scoreText, "Il vous reste %d essais", 6-essais);
	afficherTexte(renderer, font,scoreText ,10, 10);
	}


	if (saisi != NULL && *saisi != '\0')
		afficherTexte(renderer, font, saisi, 0, 300);
		
	//clavier
	drawKeyboard();
	
	return false;
}
	

