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
#define KEYBOARD_WIDTH 400
#define KEYBOARD_HEIGHT 200
#define BUTTON_WIDTH 50
#define BUTTON_HEIGHT 50
#define BUTTON_MARGIN 5

// Structure représentant un bouton du clavier virtuel
typedef struct {
    SDL_Rect rect;   // Rectangle délimitant le bouton
    char label;      // Caractère affiché sur le bouton
} KeyButton;

// Déclaration globale de la police de caractères
TTF_Font *font;

// Fonction pour initialiser la police de caractères
void initFont() {
    font = TTF_OpenFont("arial.ttf", 24); // Charger une police pour le texte
    if (font == NULL) {
        fprintf(stderr, "Erreur : Impossible de charger la police. %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

// Fonction pour libérer la mémoire utilisée par la police de caractères
void closeFont() {
    TTF_CloseFont(font);
}

// Fonction pour initialiser les boutons du clavier virtuel
void initKeyButtons(KeyButton buttons[3][10]) {
    int x, y;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 10; x++) {
            buttons[y][x].rect.x = x * (BUTTON_WIDTH + BUTTON_MARGIN);
            buttons[y][x].rect.y = y * (BUTTON_HEIGHT + BUTTON_MARGIN);
            buttons[y][x].rect.w = BUTTON_WIDTH;
            buttons[y][x].rect.h = BUTTON_HEIGHT;
            buttons[y][x].label = 'A' + y * 10 + x; // Associer chaque bouton à une lettre de l'alphabet
        }
    }
}

// Fonction pour dessiner les boutons du clavier virtuel
void drawKeyButtons(SDL_Renderer *renderer, KeyButton buttons[3][10]) {
    int x, y;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 10; x++) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 100, 100); // Couleur des bordures des boutons (noir)
            SDL_RenderDrawRect(renderer, &buttons[y][x].rect); // Dessiner la bordure du bouton

            // Dessiner la lettre dans un cadre
            SDL_Color bgColor = {100, 100, 100, 255}; // Couleur de fond (blanc)
            SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
            SDL_RenderFillRect(renderer, &buttons[y][x].rect); // Remplir le bouton avec la couleur de fond

            // Dessiner le texte (la lettre) au centre du bouton
            char label[2] = {buttons[y][x].label, '\0'};
            SDL_Surface *surface = TTF_RenderText_Solid(font, label, (SDL_Color){0, 0, 0, 255}); // Texte en noir
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect textRect;
            textRect.x = buttons[y][x].rect.x + (BUTTON_WIDTH - surface->w) / 2; // Centrer horizontalement
            textRect.y = buttons[y][x].rect.y + (BUTTON_HEIGHT - surface->h) / 2; // Centrer verticalement
            textRect.w = surface->w;
            textRect.h = surface->h;
            SDL_RenderCopy(renderer, texture, NULL, &textRect);
            
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }
}
// Fonction pour déterminer si les coordonnées (x, y) sont à l'intérieur du rectangle r
int pointInRect(int x, int y, SDL_Rect r) {
    return x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h;
}

int main(int argc, char *argv[]){
    SDL_SetMainReady();
    // Initialiser la SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur : Impossible d'initialiser SDL. %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Initialiser la bibliothèque SDL_ttf
    if (TTF_Init() != 0) {
        fprintf(stderr, "Erreur : Impossible d'initialiser SDL_ttf. %s\n", TTF_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Initialiser la police de caractères
    initFont();

    // Créer la fenêtre SDL
    SDL_Window *window = SDL_CreateWindow("Clavier virtuel",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL) {
        fprintf(stderr, "Erreur : Impossible de créer la fenêtre. %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Créer le renderer SDL
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Erreur : Impossible de créer le renderer. %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    KeyButton keyButtons[3][10]; // Matrice de boutons pour le clavier virtuel
    initKeyButtons(keyButtons);  // Initialiser les boutons

    SDL_Event event;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;
                        // Vérifier quel bouton a été cliqué
                        int i, j;
                        for (i = 0; i < 3; i++) {
                            for (j = 0; j < 10; j++) {
                                if (pointInRect(x, y, keyButtons[i][j].rect)) {
                                    // Afficher la lettre du bouton cliqué
                                    printf("%c", keyButtons[i][j].label);
                                    break;
                                }
                            }
                        }
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Couleur de fond blanche
        SDL_RenderClear(renderer);

        // Dessiner les boutons du clavier virtuel
        drawKeyButtons(renderer, keyButtons);

        SDL_RenderPresent(renderer);
    }
}

    // Libérer la mémoire utilisée par la police de caractères
