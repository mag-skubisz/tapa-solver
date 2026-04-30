#include "carre2x2.h"
#include "grille.h"
#include "dimacs.h"
#include "continuite.h"


/*
 * Calcule l'indice DIMACS de la variable D_{v,d} (variable de distance).
 * Les N premières variables (1..N) sont les variables de couleur X_v.
 * Les variables de distance commencent à N+1 et sont indexées par (v, d) :
 *   D_{v,d} = nombre_cases + (v-1) × nombre_cases + d + 1
 */
int index_distance(int v, int d, int nb_cases) {
    return nb_cases + (v - 1) * nb_cases + d + 1;
}


/*
 * Encode la contrainte de continuité des cases noires en clauses DIMACS,
 * via un encodage par arbre couvrant orienté (spanning tree encoding).
 * Met à jour *nb_variables = N + N×N pour inclure les variables de distance D_{v,d}.
 * Retourne le nombre de clauses générées.
 */
int continuite_to_dimacs(FILE *fichier, Grille *G, int* nb_variables)
{
    int N = G->largeur * G->hauteur;
    int Dmax = N - 1;
    *nb_variables = N + N * N;
    int nb = 0;
    fprintf(fichier, "c Clauses pour la continuité\n");

     /* Contrainte (1a) : X_v => D_{v,0} v ... v D_{v,Dmax} */
    for (int v = 1; v <= N; v++) {
        fprintf(fichier, "-%d", v);
        for (int d = 0; d <= Dmax; d++)
            fprintf(fichier, " %d", index_distance(v, d, N));
        fprintf(fichier, " 0\n");
        nb++;
    }

    /* Contrainte (1b) : D_{v,d} => X_v */
    for (int v = 1; v <= N; v++) {
        for (int d = 0; d <= Dmax; d++) {
            fprintf(fichier, "-%d %d 0\n", index_distance(v, d, N), v);
            nb++;
        }
    }

    /* Contrainte (2) : unicité de la distance — ¬(D_{v,d1} ∧ D_{v,d2}) pour d1 < d2 */
    for (int v = 1; v <= N; v++) {
        for (int d1 = 0; d1 <= Dmax; d1++) {
            for (int d2 = d1 + 1; d2 <= Dmax; d2++) {
                fprintf(fichier, "-%d -%d 0\n",
                    index_distance(v, d1, N),
                    index_distance(v, d2, N));
                nb++;
            }
        }
    }

     /* Contrainte (3) : unicité de la racine — ¬(D_{u,0} ∧ D_{v,0}) pour u < v */
    for (int u = 1; u <= N; u++) {
        for (int v = u + 1; v <= N; v++) {
            fprintf(fichier, "-%d -%d 0\n",
                index_distance(u, 0, N),
                index_distance(v, 0, N));
            nb++;
        }
    }

    /* Contrainte (4) : existence conditionnelle de la racine — X_v => D_{1,0} v ... v D_{N,0} */
    for (int v = 1; v <= N; v++) {
        fprintf(fichier, "-%d", v);
        for (int u = 1; u <= N; u++)
            fprintf(fichier, " %d", index_distance(u, 0, N));
        fprintf(fichier, " 0\n");
        nb++;
    }

     /* Contrainte (5) : propagation de la distance — D_{v,d} => ∨_{u ∈ N_⊥(v)} D_{u,d-1} */
    for (int i = 0; i < G->hauteur; i++) {
        for (int j = 0; j < G->largeur; j++) {
            int v = index_case(i, j, G->largeur);
            for (int d = 1; d <= Dmax; d++) {
                fprintf(fichier, "-%d", index_distance(v, d, N));
                if (i > 0)
                    fprintf(fichier, " %d", index_distance(index_case(i-1, j, G->largeur), d-1, N));
                if (i < G->hauteur - 1)
                    fprintf(fichier, " %d", index_distance(index_case(i+1, j, G->largeur), d-1, N));
                if (j > 0)
                    fprintf(fichier, " %d", index_distance(index_case(i, j-1, G->largeur), d-1, N));
                if (j < G->largeur - 1)
                    fprintf(fichier, " %d", index_distance(index_case(i, j+1, G->largeur), d-1, N));
                fprintf(fichier, " 0\n");
                nb++;
            }
        }
    }
    printf("Nombre de clauses pour la continuite: %d\n", nb);
    return nb;
}