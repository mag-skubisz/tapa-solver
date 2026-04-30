#ifndef GRILLE_H
#define GRILLE_H

#include <stdio.h>
#define N_MAX 100
#define BLANC 0
#define NOIR 1


/*
 * Structure représentant une case de la grille.
 * Une case peut être blanche ou noire, et peut contenir jusqu'à 4 indices
 * (les indices Tapa indiquent la taille des groupes de cases noires voisines).
 * Valeurs spéciales de nb_indice :
 *   -1 : case nécessairement blanche
 *   -2 : case nécessairement noire
 *    0 : case ordinaire sans indice
 *   >0 : case à indice (nombre d'indices présents)
 */
typedef struct caseGrille
{
    int type;       
    int indices[4]; 
    int nb_indice; 
} caseGrille;

/*
 * Structure représentant la grille de jeu Tapa.
 * Contient les dimensions et le tableau de cases.
 */
typedef struct Grille
{
    int largeur;
    int hauteur;
    caseGrille tab[N_MAX][N_MAX];
} Grille;

void initGrille(Grille *G, int l, int h);
void updateGrille(Grille *G, int * resultats);
void afficheGrille(Grille *G);
void lectureGrille(Grille *G, FILE *fichierGrille);

#endif