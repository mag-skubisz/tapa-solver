#include "carre2x2.h"
#include "grille.h"
#include "dimacs.h"

int carre2x2_to_dimacs(Grille *G, FILE *fichier)
{
    int nb_clauses = 0;
    fprintf(fichier, "c Clauses pour les carrés 2x2\n");
    for (int i = 0; i < G->hauteur - 1; i++) 
    {
        for (int j = 0; j < G->largeur - 1; j++) //pour chaque carré 2x2 possible dans la grille
        {
            int pos = i * G->largeur + j + 1; // variable pour suivre la position dans la grille

            int a = pos ;
            int b = pos + 1;
            int c = pos + G->largeur;
            int d = pos + G->largeur + 1; // les 4 cases du carré 2x2
            pos++;
            if(G->tab[i][j].nb_indice > 0 || G->tab[i][j+1].nb_indice > 0 || G->tab[i+1][j].nb_indice > 0 || G->tab[i+1][j+1].nb_indice > 0){
                continue; // si une des cases du carré a un indice, on ne peut pas appliquer la règle du carré 2x2
            }
            fprintf(fichier, "-%d -%d -%d -%d 0\n", a, b, c, d); // au moins une des cases doit être blanche
            nb_clauses++;
        }
    }
    printf("Nombre de clauses pour les carrés 2x2: %d\n", nb_clauses);
    return nb_clauses;
}