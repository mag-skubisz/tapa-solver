#include <stdio.h>
#include "grille.h"


/*
 * Met à jour l'état de chaque case de la grille selon les résultats du SAT-solveur.
 * Les variables positives correspondent à des cases noires, négatives à des cases blanches.
 */
void updateGrille(Grille *G, int * resultats)
{
    for (int i = 0; i < G->hauteur; i++)
    {
        for (int j = 0; j < G->largeur; j++)
        {
            int index = i * G->largeur + j;
            if (resultats[index] > 0)
                G->tab[i][j].type = NOIR;
            else
                G->tab[i][j].type = BLANC;
        }
    }
}


/*
 * Initialise une grille de dimensions l x h avec toutes les cases vides et sans indice.
 */
void initGrille (Grille* G, int l, int h)
{
    G->largeur = l;
    G->hauteur = h;
    for (int i = 0; i < G->hauteur; i++)
    {
        for (int j = 0; j < G->largeur; j++)
        {
            G->tab[i][j].type = 0; 
            G->tab[i][j].nb_indice = 0;
        }
    }
}


/*
 * Lit une grille depuis un fichier texte et remplit la structure Grille.
 *
 * Format du fichier :
 *   - Première ligne : largeur hauteur
 *   - Lignes suivantes : x y [index1 [index2 [index3 [index4]]]]
 *     où (x,y) est la position (1-indexée) de la case à indice.
 *     Valeurs spéciales pour index1 :
 *       -1 : case nécessairement blanche
 *       -2 : case nécessairement noire
 */

void lectureGrille(Grille* G, FILE* fichierGrille)
{
    if (!fichierGrille)
    {
        printf("Erreur dans l'ouverture de fichier.\n");
        return;
    }
    char buffer[256];
    int l, h;
    fscanf(fichierGrille,"%d", &l);
    fscanf(fichierGrille,"%d", &h);
    initGrille (G, l, h);
    if (!fgets(buffer, sizeof(buffer), fichierGrille)) { }
    while (fgets(buffer, sizeof(buffer), fichierGrille))
    {
        int x=0, y=0;
        int index1=0, index2 = 0, index3 = 0, index4 = 0;
        int n = sscanf(buffer, "%d %d %d %d %d %d",
                       &x, &y, &index1, &index2, &index3, &index4);

        if(index1 == -1){
            G->tab[y-1][x-1].type = BLANC;
            G->tab[y-1][x-1].nb_indice = -1;
            continue;
        } else if(index1 == -2){
            G->tab[y-1][x-1].type = NOIR;
            G->tab[y-1][x-1].nb_indice = -2;
            continue;
        }

        int i = y - 1;  
        int j = x - 1;  

        /* Lecture des indices (jusqu'à 4) selon le nombre de valeurs lues */
        if (n >= 3)
            G->tab[i][j].indices[0] = index1;
        else
            G->tab[i][j].indices[0] = 0;

        if (n >= 4)
            G->tab[i][j].indices[1] = index2;
        else
            G->tab[i][j].indices[1] = 0;

        if (n >= 5)
            G->tab[i][j].indices[2] = index3;
        else
            G->tab[i][j].indices[2] = 0;

        if (n >= 6)
            G->tab[i][j].indices[3] = index4;
        else
            G->tab[i][j].indices[3] = 0;

        
        G->tab[i][j].nb_indice = 0;
        for (int k = 0; k < 4; k++)
        {
            if (G->tab[i][j].indices[k] != 0)
                G->tab[i][j].nb_indice++;
    }
    
}
    fclose(fichierGrille);
}


/*
 * Affiche la grille dans le terminal.
 * Chaque case est affichée sur 2 lignes :
 *   - ligne 1 : indices 1 et 2 (ou type de case si pas d'indice)
 *   - ligne 2 : indices 3 et 4
 * Les cases noires sont représentées par "$$$", les blanches forcées par "---".
 */

void afficheGrille(Grille* G)
{
    //printf("Grille de largeur : %d\n", G->largeur);
    //printf("Grille de hauteur : %d\n", G->hauteur);

    /* Séparateur horizontal */
    for (int j = 0; j < G->largeur; j++)
            {
                printf("|-----");
            }
            printf("|\n");

    /* Chaque ligne logique est affichée sur 2 passes (pour les indices sur 2 lignes) */
    for (int pass = 0; pass < G->hauteur * 2; pass++)
    {
        int i = pass / 2; // to print on 2 lines
        for (int j = 0; j < G->largeur; j++)
        {
            printf("| ");

            if (G->tab[i][j].nb_indice > 0)
            {
                /* Affichage des indices : pass pair -> indices 0,1 ; pass impair -> indices 2,3 */
                for (int k = 0; k < 2; k++) 
                {
                    int x = G->tab[i][j].indices[k + (pass % 2) * 2];
                    if(x != 0){
                        printf("%d", x);
                        if (k < 1 && G->tab[i][j].indices[k + (pass % 2) * 2+1] != 0)
                            printf(",");
                        else if (k < 1) 
                            printf(" ");                         
                    }
                    else {
                        printf(" ");
                        if (k < 1)
                            printf(" "); 
                    }
                }
                printf(" ");  
            }
            else
            {
                /* Affichage selon le type de case sans indice */
                switch (G->tab[i][j].type)
                {
                    case 0:
                        if(G->tab[i][j].nb_indice == -1)
                            printf("--- "); /* Case nécessairement blanche */
                        else
                            printf("    "); /* Case vide ordinaire */
                        break;
                    case 1:  
                        printf("$$$ "); /* Case noire */
                        break;
                    default:
                        perror("type de case inconnu.\n");
                        break;
                }
            }
        }
        printf("|\n");
        if (pass % 2 == 1)  /* Séparateur horizontal après chaque ligne logique complète (2 passes) */
        {
            for (int j = 0; j < G->largeur; j++)
            {
                printf("|-----");
            }
            printf("|\n");
        }
    }
}



