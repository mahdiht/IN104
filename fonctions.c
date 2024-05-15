#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "fonctions.h"
#include <string.h>

#define MAX_WORD_LENGTH 200

//fonction qui sépare les mots selon les virgules
void separateWords(char *input_string) {
    char *token;
    const char delimiter[] = ",";

    // Utilisation de strtok pour diviser la chaîne d'entrée en tokens
    token = strtok(input_string, delimiter);

    // Si token est NULL, cela signifie qu'aucun token n'a été trouvé
    if (token != NULL) {
        // Allouer de la mémoire pour la première partie (mot)
        strcpy(word, token);

        // Obtenir le prochain token
        token = strtok(NULL, delimiter);

        // Si token est NULL, il n'y a pas de deuxième partie (indice)
        if (token != NULL) {
            // Allouer de la mémoire pour la deuxième partie (indice)
            strcpy(hint, token);
        } 
    }
}
// fonction qui trouve un mot aléatoirement
void lireligneAleatoire(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", nomFichier);
        exit(EXIT_FAILURE);
    }
    
    // Compter le nombre de mots dans le fichier
    int nombreDeMots = 0;
    char ligne[MAX_WORD_LENGTH];
    while (fgets(ligne, MAX_WORD_LENGTH, fichier) != NULL) {
        nombreDeMots++;
    }
    
    // Choisir un mot aléatoire
    srand(time(NULL));
    int motAleatoireIndex = rand() % nombreDeMots;
    
    // Retourner au début du fichier
    rewind(fichier);

    // Lire le mot à l'indice aléatoire
    int i = 0;
    while (fgets(ligne, MAX_WORD_LENGTH, fichier) != NULL) {
        if (i == motAleatoireIndex) {
            // Allouer de la mémoire pour le mot retourné
            char* motRetourne = malloc(strlen(ligne) + 1);
            if (motRetourne == NULL) {
                printf("Erreur : Échec de l'allocation de mémoire.\n");
                exit(EXIT_FAILURE);
            }
            strcpy(motRetourne, ligne);
            
            // Supprimer le caractère de saut de ligne à la fin
            if (motRetourne[strlen(motRetourne) - 1] == '\n') {
                motRetourne[strlen(motRetourne) - 1] = '\0';
            }
            separateWords(motRetourne);
            free(motRetourne);
            // Retourner après avoir traité le mot
            fclose(fichier);
            return;
        }
        i++;
    }
    
    // Fermer le fichier (au cas où le nombre de mots serait inférieur à motAleatoireIndex)
    fclose(fichier);
    
    // Si quelque chose ne s'est pas passé comme prévu, retourner NULL
    return;
}
