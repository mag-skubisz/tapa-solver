#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grille.h"
#include "dimacs.h"
#include "indices.h"
#include "carre2x2.h"
#include "continuite.h"
#include "sat.h"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: %s <flags> <fichier>\n", argv[0]);
        printf("Flags:\n");
        printf("  2 : activer la règle des carrés 2x2\n");
        printf("  i : activer les indices\n");
        printf("  c : activer la continuité\n");
        printf("  t : lancer les tests (via run_tests.sh)\n");
        return -1;
    }

    char *flags = argv[1];

    if (strchr(flags, 't') != NULL)
    {
        system("bash run_tests.sh");
        return 0;
    }
    
    char *filename = argv[2];

    int use_2x2        = (strchr(flags, '2') != NULL);
    int use_indices    = (strchr(flags, 'i') != NULL);
    int use_continuite = (strchr(flags, 'c') != NULL);
    
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Erreur dans l'ouverture de fichier.\n");
        return 1;
    }

    Grille G;
    lectureGrille(&G, f);
    //fclose(f);
    int nb_clauses = 0;
    afficheGrille(&G);
    
    char *dimac = "grille.dimacs";
    FILE *dimacs_file = fopen(dimac, "w");
    if (dimacs_file == NULL)
    {
        printf("Erreur dans la création du fichier DIMACS.\n");
        return 1;
    }
    int nb_variables = initialiserDimacs(&G, dimacs_file);

    if (use_2x2)
        nb_clauses += carre2x2_to_dimacs(&G, dimacs_file);

    nb_clauses += indices_to_dimacs(&G, dimacs_file, !use_indices);

    if (use_continuite)
        nb_clauses += continuite_to_dimacs(dimacs_file, &G, &nb_variables);


    //nb_clauses += carre2x2_to_dimacs(&G,dimacs_file);
    //nb_clauses += indices_to_dimacs(&G,dimacs_file, 1);
    //nb_clauses += continuite_to_dimacs(dimacs_file, &G, &nb_variables);

    fclose(dimacs_file);
    
    dimacs_file = fopen(dimac, "r+");
    modifierClausesDimacs(nb_clauses, nb_variables, dimacs_file);
    fclose(dimacs_file);

    int *resultats = calloc((size_t)nb_variables,sizeof(int));
    if (resultats==NULL)
    {
        printf("Erreur dans l'allocation mémoire pour le résultat.\n");
        return 1;
    }

    int res = rellaiSat(dimac,resultats);

    if(res)
        return 0;

    updateGrille(&G, resultats);

    afficheGrille(&G);
    free(resultats);
    return 0;
}