#ifndef SAT_H
#define SAT_H

// Prend un nom de fichier au format DIMACS et un tableau de résultats en paramètre
// envoit le fichier a picosat et remplit le tableau de résultats avec les valeurs des variables donné par le SAT solver.
int  rellaiSat(const char *nom_fichier, int *resultats);

#endif // SAT_H