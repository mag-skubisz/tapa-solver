#include <stdio.h>
#include <stdlib.h>
#include "dimacs.h"


/*
Retourne l'indice DIMACS (1-indexé) de la case à la ligne et colonne données.
 */
int index_case(int ligne, int colonne, int largeur)
{
    return ligne * largeur + colonne + 1;
}

/*
Retourne le nombre maximal de carrés 2x2 dans la grille.
 */
int nb_clauses_carres(Grille *G)
{
    if (G->hauteur < 2 || G->largeur < 2)
        return 0;

    return (G->hauteur - 1) * (G->largeur - 1);
}

/*
 * Met à jour l'en-tête du fichier DIMACS avec le nombre de variables et de clauses,
 * une fois que toutes les clauses ont été écrites.
 * On écrase les valeurs provisoires écrites par initialiserDimacs (après "p cnf ").
 */
int modifierClausesDimacs(int nb_clauses, int nb_variables, FILE *fichier)
{
    fseek(fichier, 6, SEEK_SET);
    fprintf(fichier, "%d %d", nb_variables, nb_clauses);
    return 0;
}


/*
 * Initialise le fichier DIMACS avec un en-tête provisoire.
 * Le nombre de clauses est mis à 0 et sera corrigé par modifierClausesDimacs
 * une fois toutes les clauses générées.
 * Retourne N = largeur × hauteur, le nombre initial de variables (cases uniquement).
 */
int initialiserDimacs(Grille *G, FILE * fichier)
{
    int N = G->largeur * G->hauteur;
    int nb_variables = N;
    fprintf(fichier, "p cnf %d %d                              \n\n", nb_variables, 0);
    return nb_variables;
}