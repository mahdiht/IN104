#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BUTTON_WIDTH 40
#define BUTTON_HEIGHT 40
#define NUM_LETTERS 26
#define LETTERS_PER_ROW_1 10
#define LETTERS_PER_ROW_2 10
#define LETTERS_PER_ROW_3 6

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Rect gButtons[NUM_LETTERS];
char gLetters[NUM_LETTERS] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
TTF_Font* gFont = NULL;
SDL_Color buttonHoverColor = { 0xC0, 0xC0, 0xC0, 0x80 }; // Gris clair semi-transparent
SDL_Color buttonDefaultColor = { 0xFF, 0xFF, 0xFF, 0xFF }; // Blanc
SDL_Color buttonPressedColorAorH = { 0x00, 0xFF, 0x00, 0x80 }; // Vert
SDL_Color buttonPressedColorOther = { 0xFF, 0x00, 0x00, 0x80 }; // Rouge

bool gButtonState[NUM_LETTERS] = {false}; // Initialise tous les boutons à l'état désactivé


int gHoveredButton = -1;
bool gMouseOverButton = false;

bool initSDL() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    gWindow = SDL_CreateWindow("Virtual Keyboard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    if(TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    gFont = TTF_OpenFont("arial.ttf", 28); // Change the path and size as needed
    if(gFont == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

void closeSDL() {
    TTF_CloseFont(gFont);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    TTF_Quit();
    SDL_Quit();
}

void createButtons() {
    int row1_start_x = (SCREEN_WIDTH - (LETTERS_PER_ROW_1 * BUTTON_WIDTH + (LETTERS_PER_ROW_1 - 1) * 5)) / 2;
    int row2_start_x = (SCREEN_WIDTH - (LETTERS_PER_ROW_2 * BUTTON_WIDTH + (LETTERS_PER_ROW_2 - 1) * 5)) / 2;
    int row3_start_x = (SCREEN_WIDTH - (LETTERS_PER_ROW_3 * BUTTON_WIDTH + (LETTERS_PER_ROW_3 - 1) * 5)) / 2;
    int y = (SCREEN_HEIGHT - 3 * (BUTTON_HEIGHT + 5)) / 2;
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
            gHoveredButton = i;
            gMouseOverButton = true;
            break;
        }
    }
    if (!gMouseOverButton) {
        gHoveredButton = -1;
    }
}

void drawKeyboard() {
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_Color textColor = { 0, 0, 0 };
    for(int i = 0; i < NUM_LETTERS; i++) {
        if (i == gHoveredButton && gMouseOverButton) {
            SDL_SetRenderDrawColor(gRenderer, buttonHoverColor.r, buttonHoverColor.g, buttonHoverColor.b, buttonHoverColor.a);
        } else {
            if (gButtonState[i]) {
                if (gLetters[i] == 'A' || gLetters[i] == 'H') {
                    SDL_SetRenderDrawColor(gRenderer, buttonPressedColorAorH.r, buttonPressedColorAorH.g, buttonPressedColorAorH.b, buttonPressedColorAorH.a); // Vert
                } else {
                    SDL_SetRenderDrawColor(gRenderer, buttonPressedColorOther.r, buttonPressedColorOther.g, buttonPressedColorOther.b, buttonPressedColorOther.a); // Rouge
                }
            } else {
                SDL_SetRenderDrawColor(gRenderer, buttonDefaultColor.r, buttonDefaultColor.g, buttonDefaultColor.b, buttonDefaultColor.a); // Blanc
            }
        }
        SDL_RenderFillRect(gRenderer, &gButtons[i]); // Remplir tout le rectangle avec la couleur
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); // Noir
        SDL_RenderDrawRect(gRenderer, &gButtons[i]); // Dessiner le contour du rectangle
        char letter = gLetters[i];
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(gFont, &letter, textColor);
        SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);
        SDL_Rect destRect = {gButtons[i].x + BUTTON_WIDTH / 2 - surfaceMessage->w / 2, gButtons[i].y + BUTTON_HEIGHT / 2 - surfaceMessage->h / 2, surfaceMessage->w, surfaceMessage->h};
        SDL_RenderCopy(gRenderer, message, NULL, &destRect);
        SDL_DestroyTexture(message);
        SDL_FreeSurface(surfaceMessage);
    }
    SDL_RenderPresent(gRenderer);
}








int main(int argc, char* args[]) {
    if(!initSDL()) {
        printf("Failed to initialize!\n");
        return -1;
    }
    bool quit = false;
    SDL_Event e;
    createButtons();
while(!quit) {
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            quit = true;
        }
        else if(e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            checkMouseOverButton(x, y);
            if (gMouseOverButton) {
                printf("Letter clicked: %c\n", gLetters[gHoveredButton]);
                gButtonState[gHoveredButton] = !gButtonState[gHoveredButton]; // Inverser l'état du bouton
            }
        }
        else if (e.type == SDL_MOUSEMOTION) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            checkMouseOverButton(x, y);
        }
    }
    drawKeyboard();
}


    closeSDL();
    return 0;
}
