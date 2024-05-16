#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <time.h>

#include "fonctions.h"

SDL_Rect gButtons[NUM_LETTERS];
char gLetters[NUM_LETTERS] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
TTF_Font* gFont = NULL;
SDL_Color buttonHoverColor = { 0xC0, 0xC0, 0xC0, 0x80 }; // Gris clair semi-transparent
SDL_Color buttonDefaultColor = { 0xFF, 0xFF, 0xFF, 0xFF }; // Blanc
SDL_Color buttonPressedColorAorH = { 0x00, 0xFF, 0x00, 0x80 }; // Vert
SDL_Color buttonPressedColorOther = { 0xFF, 0x00, 0x00, 0x80 }; // Rouge

bool gButtonState[NUM_LETTERS] = {false}; // Initialise tous les boutons à l'état désactivé

bool newstate=true;	//par defaut y a un changement detat


int gHoveredButton = -1;
bool gMouseOverButton = false;

void createButtons() {
    int row1_start_x = (width - (LETTERS_PER_ROW_1 * BUTTON_WIDTH + (LETTERS_PER_ROW_1 - 1) * 5)) / 2;
    int row2_start_x = (width - (LETTERS_PER_ROW_2 * BUTTON_WIDTH + (LETTERS_PER_ROW_2 - 1) * 5)) / 2;
    int row3_start_x = (width - (LETTERS_PER_ROW_3 * BUTTON_WIDTH + (LETTERS_PER_ROW_3 - 1) * 5)) / 2;
    int y = ((height - 3 * (BUTTON_HEIGHT + 5)) / 2)+200;
    int index = 0;
    for(int i = 0; i < LETTERS_PER_ROW_1; i++) {
        gButtons[index].x = row1_start_x + i * (BUTTON_WIDTH + 5);
        gButtons[index].y = y;
        gButtons[index].w = BUTTON_WIDTH;
        gButtons[index].h = BUTTON_HEIGHT;
        index++;
    }
    y += BUTTON_HEIGHT + 5;
    for(int i = 0; i < LETTERS_PER_ROW_2; i++) {
        gButtons[index].x = row2_start_x + i * (BUTTON_WIDTH + 5);
        gButtons[index].y = y;
        gButtons[index].w = BUTTON_WIDTH;
        gButtons[index].h = BUTTON_HEIGHT;
        index++;
    }
    y += BUTTON_HEIGHT + 5;
    for(int i = 0; i < LETTERS_PER_ROW_3; i++) {
        gButtons[index].x = row3_start_x + i * (BUTTON_WIDTH + 5);
        gButtons[index].y = y;
        gButtons[index].w = BUTTON_WIDTH;
        gButtons[index].h = BUTTON_HEIGHT;
        index++;
    }
}

void checkMouseOverButton(int x, int y) {
    gMouseOverButton = false;
    for(int i = 0; i < NUM_LETTERS; i++) {
        if(x >= gButtons[i].x && x <= gButtons[i].x + gButtons[i].w && y >= gButtons[i].y && y <= gButtons[i].y + gButtons[i].h) {
            gMouseOverButton = true;	
            if (gHoveredButton == i)
            	newstate=false;
            else{
            	newstate=true;
            	gHoveredButton = i;
            }
            break;
        }
    }
    if (!gMouseOverButton) {
    	if (gHoveredButton == -1)
    		newstate=false;
    	else{
    		newstate=true;
        	gHoveredButton = -1;
        }
    }
}

void drawKeyboard() {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_Color textColor = { 0, 0, 0 };
    for(int i = 0; i < NUM_LETTERS; i++) {
        if (i == gHoveredButton && gMouseOverButton && !gButtonState[gHoveredButton]) {
            SDL_SetRenderDrawColor(renderer, buttonHoverColor.r, buttonHoverColor.g, buttonHoverColor.b, buttonHoverColor.a);
        } else {
            if (gButtonState[i]) {
                if (strchr(word, gLetters[i]+'a'-'A') != NULL) {
                    SDL_SetRenderDrawColor(renderer, buttonPressedColorAorH.r, buttonPressedColorAorH.g, buttonPressedColorAorH.b, buttonPressedColorAorH.a); // Vert
                } else {
                    SDL_SetRenderDrawColor(renderer, buttonPressedColorOther.r, buttonPressedColorOther.g, buttonPressedColorOther.b, buttonPressedColorOther.a); // Rouge
                }
            } else {
                SDL_SetRenderDrawColor(renderer, buttonDefaultColor.r, buttonDefaultColor.g, buttonDefaultColor.b, buttonDefaultColor.a); // Blanc
            }
        }
        SDL_RenderFillRect(renderer, &gButtons[i]); // Remplir tout le rectangle avec la couleur
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Noir
        SDL_RenderDrawRect(renderer, &gButtons[i]); // Dessiner le contour du rectangle
        char letter = gLetters[i];
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(gFont, &letter, textColor);
        SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        SDL_Rect destRect = {gButtons[i].x + BUTTON_WIDTH / 2 - surfaceMessage->w / 2, gButtons[i].y + BUTTON_HEIGHT / 2 - surfaceMessage->h / 2, surfaceMessage->w, surfaceMessage->h};
        SDL_RenderCopy(renderer, message, NULL, &destRect);
        SDL_DestroyTexture(message);
        SDL_FreeSurface(surfaceMessage);
    }
}

//initalisation du clavier
void initclavier() {
    gFont = TTF_OpenFont("arial.ttf", 28); // Change the path and size as needed
    if(gFont == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    createButtons();
    drawKeyboard();
}

//initalisation du clavier
void destroyclavier() {
	if(gFont != NULL)
		TTF_CloseFont(gFont);
}





