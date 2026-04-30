#include <stdio.h>
#include <stdlib.h>
#include "sat.h"
#include "picosat.h"

int rellaiSat(const char *nom_fichier, int *resultats) {
    FILE *f = fopen(nom_fichier, "r");
    if (!f) return 1;

    //printf("Lecture du fichier DIMACS...\n");

    /* Initialisation de l'instance PicoSAT */
    PicoSAT *p = picosat_init();

    char line[8192];
    int nb_vars = 0;

    /* Lecture du fichier ligne par ligne */
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == 'c') continue;

        if (line[0] == 'p') {
            /* On récupère uniquement le nombre de variables */
            sscanf(line, "p cnf %d", &nb_vars);
            continue;
        }

         /* Ligne de clause : lecture des littéraux un par un */
        char *ptr = line;
        int lit;
        while (sscanf(ptr, "%d", &lit) == 1) {
            picosat_add(p, lit);

           
            while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') ptr++;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
        }
    }

    fclose(f);

    int res = picosat_sat(p, -1);

    if (res == PICOSAT_SATISFIABLE) {
        printf("SAT\n");

        for (int i = 1; i <= nb_vars; i++) {
            int val = picosat_deref(p, i);
            resultats[i - 1] = val;  
        }

    } else {
        printf("UNSAT\n");
        return 1;
    }

    picosat_reset(p);
    return 0;
}