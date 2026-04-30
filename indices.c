#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "indices.h"
#include "grille.h"

typedef struct Noeud
{
    int *tab; // tableau de 8 entiers (0=blanc, 1=noir)
    struct Noeud *suivant;
} Noeud;

typedef struct
{
    Noeud *tete;
    int taille;
} Liste_arr;

void initialiser_liste(Liste_arr *liste);
int arrangement_existe(Liste_arr *liste, int *arr);
void inserer_liste(Liste_arr *liste, int *arr);
void detruire_liste(Liste_arr *liste);
void afficher_liste(Liste_arr *liste, int logic);


void initialiser_liste(Liste_arr *liste)
{
    liste->tete = NULL;
    liste->taille = 0;
}

// Vérifier si un arrangement existe déjà
int arrangement_existe(Liste_arr *liste, int *arr)
{
    Noeud *courant = liste->tete;
    while (courant != NULL)
    {
        int id = 1;
        for (int i = 0; i < 8; i++)
        {
            if (courant->tab[i] != arr[i])
            {
                id = 0;
                break;
            }
        }
        if (id)
        {
            return 1; // existe déjà
        }
        courant = courant->suivant;
    }
    return 0; // n'existe pas
}

Noeud *creer_noeud(int *arr)
{
    Noeud *nouveau = (Noeud *)malloc(sizeof(Noeud));
    nouveau->tab = (int *)malloc(8 * sizeof(int));
    for (int i = 0; i < 8; i++)
    {
        nouveau->tab[i] = arr[i];
    }
    nouveau->suivant = NULL;
    return nouveau;
}

/*
void afficher_arrangement(int *arr)
{
    printf("Arrangement: ");
    for (int i = 0; i < 8; i++)
    {
        
        printf("%d ", arr[i]);
    }
    printf("\n");
}
*/

// Insérer dans la liste (sans doublons, avec normalisation)
void inserer_liste(Liste_arr *liste, int *arr)
{
    int a[8];
    //afficher_arrangement(arr);
    for(int i = 0; i < 9; i++){ //n'ajoute pas  8*-1
        if (i == 8)
            return;
        if(arr[i] != -1)
            break;
    }

    // Normaliser l'arrangement (2,3 deviennent 1)
    for (int i = 0; i < 8; i++)
    {
        if (arr[i] > 0)
            a[i] = 1;
        else
            a[i] = arr[i];
    }


    Noeud *courant = liste->tete;
    
    if(courant == NULL){
        liste->tete = creer_noeud(a);
        liste->taille++;
        return;
    }
    
    Noeud *precedent = NULL;
    int i = 0;


    while (courant != NULL){
        if(courant->tab[i] == a[i]){
            i++;
            if(i == 8){
                //printf("arrangement deja dans la liste\n");
                return;
            }
            continue;
        } else if(courant->tab[i] < a[i]){
            precedent = courant;
            courant = courant->suivant;
            i = 0;
        } else {
            Noeud *nouveau = creer_noeud(a);
            if(precedent == NULL){
                nouveau->suivant = liste->tete;
                liste->tete = nouveau;
            } else {
                precedent->suivant = nouveau;
                nouveau->suivant = courant;
            }
            liste->taille++;
            return;
        }
    }
    Noeud *nouveau = creer_noeud(a); //si il n'a pas été inséré avant, il est inséré en fin de liste
    precedent->suivant = nouveau;
    liste->taille++;
    return;
    
    
}

/*
void afficher_liste(Liste_arr *liste, int logic)
{
    Noeud *courant = liste->tete;

    //printf("Liste des arrangements\n");
    while (courant != NULL)
    {
        printf("Arrangement: ");
        for (int i = 0; i < 8; i++)
        {
            if(logic){
                switch (courant->tab[i])
                {
                case -1:
                    continue;
                case 0:
                    printf("-");
                default:
                    printf("%d ", i+1);
                }
            }
            else{
                printf("%d ", courant->tab[i]);
            }
        }
        printf("\n");
        courant = courant->suivant;
    }
    printf("Total : %d lignes\n\n", liste->taille);
}
*/

void detruire_liste(Liste_arr *liste)
{
    Noeud *courant = liste->tete;
    while (courant != NULL)
    {
        Noeud *temp = courant;
        courant = courant->suivant;
        free(temp->tab);
        free(temp);
    }
    liste->tete = NULL;
    liste->taille = 0;
}

void get_arrangement(int k1, int k2, int k3, int k4, int *arr, Liste_arr *E_arr)
{
    //le cas particulier 1 1 1 1 pour eviter de calculer tout les arrangements.
    if (k4){
        inserer_liste(E_arr, (int[]){0, 1, 0, 1, 0, 1, 0, 1}), inserer_liste(E_arr, (int[]){1, 0, 1, 0, 1, 0, 1, 0});
        return;
    }

    for (int i = 0; i < 8; i++)
    {

        int flag = 0;
        for (int i2 = 0; i2 < k1; i2++)
        { // verifie que les cases ou on veut mettre k1 sont vide
            if (arr[(i + i2) % 8] != 0)
            {
                flag = 1;
                break;
            }
        }
        if (flag)
            continue;

        // met tout les arrangements possibles de k1 dans le tableau
        for (int fill = 0; fill < k1; fill++)
        {
            arr[(i + fill) % 8] = 1;
        }
        if (k2 == 0) // si il y a qu'un indice.
            inserer_liste(E_arr, arr);

        else
        {
            for (int j = 0; j < 8; j++)
            { // met tout les arrangements possibles de k2 dans le tableau sachant k1
                
                int flag = 0;
                if (arr[(j + 7) % 8] > 0 || arr[(j + k2) % 8] > 0) {
                    continue;
                }

                for (int j2 = 0; j2 < k2; j2++)
                { // verifie que les cases ou on veut mettre k2 sont vide
                    if (arr[(j + j2) % 8] != 0)
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag)
                    continue;
                
                for (int fill = 0; fill < k2; fill++)
                {
                    arr[(j + fill) % 8] = 2;
                }


                if (k3 == 0) // si il n'y a que 2 indices.
                    inserer_liste(E_arr, arr);
                
                else
                {
                    for (int k = 0; k < 8; k++)
                    { // met tout les arrangements possibles de k3 dans le tableau sachant k1 et k2
                        int flag = 0;
                        if (arr[(k + 7) % 8] > 0 || arr[(k + k3) % 8] > 0) {
                            continue;
                        }

                        for (int j2 = 0; j2 < k3; j2++)
                        { // verifie que les cases ou on veut mettre k3 sont vide
                            if (arr[(k + j2) % 8] != 0)
                            {
                                flag = 1;
                                break;
                            }
                        }
                        if (flag)
                            continue;
                        
                        for (int fill = 0; fill < k3; fill++)
                        {
                            arr[(k + fill) % 8] = 3;
                        }

                        inserer_liste(E_arr, arr);

                        for (int fill = 0; fill < k3; fill++)
                        {
                            if (arr[(k + fill) % 8] == 3)
                                arr[(k + fill) % 8] = 0;
                        }
                    }
                }

                for (int fill = 0; fill < k2; fill++)
                {
                    if (arr[(j + fill) % 8] == 2)
                        arr[(j + fill) % 8] = 0;
                }
            }
        }

        for (int fill = 0; fill < k1; fill++)
        {
            if (arr[(i + fill) % 8] == 1)
                arr[(i + fill) % 8] = 0;
        }
    }
}

//modifie les cases innaccessible (hors grille) pour eviter les out of bound
//modifie les cases entourant un indice pour eviter de calculer des arrangements superflux et faire gonfler le nombre de clauses
int remplis_selon_voisins(int *arr, int i, int j, Grille *G) {
    if(i==0){ //sur un bord
        arr[0] = -1;
        arr[1] = -1;
        arr[2] = -1;
    }
    if (i == G->hauteur - 1){
        arr[4] = -1;
        arr[5] = -1;
        arr[6] = -1;
    }
    if(j==0){
        arr[0] = -1;
        arr[6] = -1;
        arr[7] = -1;
    }
    if (j == G->largeur - 1){
        arr[2] = -1;
        arr[3] = -1;
        arr[4] = -1;
    } //fonctionne pour les angles (sur deux bords)

    //chaque cases individuellement
    // >0 et non !=0 pour ne pas prendre en compte les cases forcées noir ou blanches pour les tests
    if(arr[0] != -1 && G->tab[i-1][j-1].nb_indice > 0){
        arr[0] = -1;
    }
    if(arr[1] != -1 && G->tab[i-1][j].nb_indice > 0){
        arr[1] = -1;
    }
    if(arr[2] != -1 && G->tab[i-1][j+1].nb_indice > 0){
        arr[2] = -1;
    }
    if(arr[3] != -1 && G->tab[i][j+1].nb_indice > 0){
        arr[3] = -1;
    }
    if(arr[4] != -1 && G->tab[i+1][j+1].nb_indice > 0){
        arr[4] = -1;
    }
    if(arr[5] != -1 && G->tab[i+1][j].nb_indice > 0){
        arr[5] = -1;
    }
    if(arr[6] != -1 && G->tab[i+1][j-1].nb_indice > 0){
        arr[6] = -1;
    }
    if(arr[7] != -1 && G->tab[i][j-1].nb_indice > 0){
        arr[7] = -1;
    }

    return 0;

}

void fnd_a_fnc(Liste_arr *fnd, Liste_arr *fnc){
    Noeud *monome_courant = fnd->tete;
    if(monome_courant == NULL)
        return;

    for (int i = 0; i < 8; i++) { //innitialisation de la FNC avec le premier monome de la FND
        int arr[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
        arr[i] = monome_courant->tab[i];
        inserer_liste(fnc, arr);
    }
    
    monome_courant = monome_courant->suivant;
    while (monome_courant != NULL) // iteration sur les monomes de la FND
    {
        Liste_arr nouvelle_fnc; // liste temporaire pour stocker les nouvelles clauses de la FNC
        initialiser_liste(&nouvelle_fnc);
        Noeud *clause_courante = fnc->tete;
        while (clause_courante != NULL) {  // iteration sur les clauses de la FNC
            //pour caque clause de la FNC on ajoute un literal de la FND et on ajoute la nouvelle clause a la nouvelle FNC
            
            for (int i = 0; i < 8; i++) {
                int arr[8];
                for(int i = 0; i < 8; i++){
                    arr[i] = clause_courante->tab[i];
                }

                switch(monome_courant->tab[i]){
                    case -1:
                        continue; // non présent dans le monome, on ne change rien
                    case 0:
                        if(clause_courante->tab[i] == 1)
                            continue; //elimination des clauses valides a ou -a
                        arr[i] = 0; //ajout de -a dans la clause
                        break;
                    case 1:
                        if(clause_courante->tab[i] == 0)
                            continue; //elimination des clauses valides a ou -a
                        arr[i] = 1; //ajout de a dans la clause
                        break;                        
                }
                inserer_liste(&nouvelle_fnc, arr);                    
            }
            clause_courante = clause_courante->suivant;
        }
        detruire_liste(fnc);
        fnc->tete = nouvelle_fnc.tete;
        fnc->taille = nouvelle_fnc.taille;
        nouvelle_fnc.tete = NULL;
        nouvelle_fnc.taille = 0;
        monome_courant = monome_courant->suivant;
    }
    
  

}

//ecrit les clauses de la FNC dans le fichier au format DIMACS en prenant en compte le decalage par rapport a la position i j  de l'indice
//retourne le nombre de clauses écrites
int ecrire_dimacs(Liste_arr *fnc, FILE *f, Grille *G, int i, int j){
    Noeud *courant = fnc->tete;
    int nb_clauses = 1;
    fprintf(f, "\n");
    fprintf(f, "c Clauses pour la case (%d,%d) avec indice", j+1, i+1);
       for(int k = 0; k < G->tab[i][j].nb_indice; k++){
            fprintf(f, " %d", G->tab[i][j].indices[k]);
       }
    fprintf(f, "\n");

    int pos = i*G->largeur + j + 1; // position de la variable correspondant à la case (i,j)
    fprintf(f, "-%d 0\n", pos); // clause pour dire que la case doit être noire

       //le décalage en question
    int decalages[8] = {-G->largeur-1, -G->largeur, -G->largeur+1, 1, G->largeur+1, G->largeur, G->largeur-1, -1};

    while (courant != NULL)
    {
        nb_clauses++;
        for (int k = 0; k < 8; k++)
        {
            if(courant->tab[k] == 1){
                fprintf(f, "%d ", pos + decalages[k]);
            } else if(courant->tab[k] == 0){
                fprintf(f, "-%d ", pos + decalages[k]);
            }
        }
        fprintf(f, "0\n");
        courant = courant->suivant;
    }
    return nb_clauses;
}

//utilisé spécifiquement dans initialise_clauses pour écrire les clauses dans les fichiers .clauses
void ecrire_clauses(Liste_arr *fnc, FILE *f){
    Noeud *courant = fnc->tete;
    while (courant != NULL)
    {
        for (int k = 0; k < 8; k++)
        {
            fprintf(f, "%d ", courant->tab[k]);
        }
        fprintf(f, "\n");
        courant = courant->suivant;
    }
}

//utilisé semblablement a une lambda expression pour qsort dans lire_clause
int comparer_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

//recuprére la FNC correspondante a un indice donné dans les fichiers .clauses préalablement écrits par initialise_clauses
int lire_clause(int k0, int k1, int k2, int k3, Liste_arr * fnc){
    //make ure k0>= k1 >= k2>= k3 if not make is so
    int ks[4] = {k0, k1, k2, k3};
    qsort(ks, 4, sizeof(int), comparer_int); //tri dans l'ordre croissant 
    k0 = ks[3];
    k1 = ks[2];
    k2 = ks[1];
    k3 = ks[0];
    


    int arr[8] = {0};
    char nom_fichier[50];
    sprintf(nom_fichier, "indices/indice_%d%d%d%d.clauses", k0, k1, k2, k3);
    FILE *f = fopen(nom_fichier, "r");
    if(f == NULL){
        printf("Erreur dans l'ouverture de fichier %s.\n", nom_fichier);
        return -1;
    }
    while(!feof(f)){
        for(int i = 0; i < 8; i++){
            if(fscanf(f, "%d", &arr[i]) != 1){
                if(i == 0){
                    return 0;
                } else {
                    printf("Erreur dans la lecture de fichier %s.\n", nom_fichier);
                    return -1;
                }
            }
        }
        inserer_liste(fnc, arr); //ajoute l'arrangement lu dans la liste de FNC
    }
    fclose(f);
    return 0;
}


//A l'attention du/de la professeur : vous pouvez décommenter les premières lignes dans indice_to_dimacs
//pour pouvoir voir l'execution et le temps de calcul de chaque indice, ainsi que le nombre de clauses générées pour chaque indice.




//calcule tout les indices possible dans le cadre des indices de tapa, et les transcrit en clauses dans des fichiers .clauses pour eviter de les recalculer a chaque (diminution de l'ordre de magnitude de la complexité temporelle de l'algorithme)
void initialise_clauses() {
    struct timespec start, end;
    char nom_fichier[50];
    Liste_arr enesmble_fnd;
    
    for(int i = 1; i<=8; i++){
        int arr[8] = {0};
        initialiser_liste(&enesmble_fnd);
        get_arrangement(i, 0, 0, 0, arr, &enesmble_fnd);
        //printf("arrangement pour indice %d: %d\n", i, enesmble_fnd.taille);
        Liste_arr ensemble_fnc;
        initialiser_liste(&ensemble_fnc);
        clock_gettime(CLOCK_MONOTONIC, &start);
        fnd_a_fnc(&enesmble_fnd, &ensemble_fnc);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        //printf("nombre de clauses: %d, %fs\n", ensemble_fnc.taille, time_taken);
        sprintf(nom_fichier, "indices/indice_%d000.clauses", i);
        FILE *fclauses = fopen(nom_fichier, "w");
        ecrire_clauses(&ensemble_fnc, fclauses);
        fclose(fclauses);
        detruire_liste(&ensemble_fnc);
        detruire_liste(&enesmble_fnd);
        for(int j = 1; j <= i  && i+j+2 <= 8; j++)
        {
            int arr[8] = {0};
            get_arrangement(i, j, 0, 0, arr, &enesmble_fnd);
            //printf("arrangement pour indice %d %d: %d\n", i, j, enesmble_fnd.taille);
            Liste_arr ensemble_fnc;
            initialiser_liste(&ensemble_fnc);
            clock_gettime(CLOCK_MONOTONIC, &start);
            fnd_a_fnc(&enesmble_fnd, &ensemble_fnc);
            clock_gettime(CLOCK_MONOTONIC, &end);
            double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            //printf("nombre de clauses: %d, %fs\n", ensemble_fnc.taille, time_taken);
            sprintf(nom_fichier, "indices/indice_%d%d00.clauses", i, j);
            FILE *fclauses = fopen(nom_fichier, "w");
            ecrire_clauses(&ensemble_fnc, fclauses);
            fclose(fclauses);
            detruire_liste(&ensemble_fnc);
            detruire_liste(&enesmble_fnd);
            for(int k = 1; k <= j && i+j+k+3 <= 8; k++)
            {
                int arr[8] = {0};
                get_arrangement(i, j, k, 0, arr, &enesmble_fnd);
                //printf("arrangement pour indice %d %d %d: %d\n", i, j, k, enesmble_fnd.taille);
                Liste_arr ensemble_fnc;
                initialiser_liste(&ensemble_fnc);
                clock_gettime(CLOCK_MONOTONIC, &start);
                fnd_a_fnc(&enesmble_fnd, &ensemble_fnc);
                clock_gettime(CLOCK_MONOTONIC, &end);
                double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
                //printf("nombre de clauses: %d, %fs\n", ensemble_fnc.taille, time_taken);
                sprintf(nom_fichier, "indices/indice_%d%d%d0.clauses", i, j, k);
                FILE *fclauses = fopen(nom_fichier, "w");
                ecrire_clauses(&ensemble_fnc, fclauses);
                fclose(fclauses);
                detruire_liste(&ensemble_fnc);
                detruire_liste(&enesmble_fnd);
                for(int l = 1; l <= k && i+j+k+l+4 <= 8; l++)
                {
                    int arr[8] = {0};
                    get_arrangement(i, j, k, l, arr, &enesmble_fnd);
                    //printf("arrangement pour indice %d %d %d %d: %d\n", i, j, k, l, enesmble_fnd.taille);
                    Liste_arr ensemble_fnc;
                    initialiser_liste(&ensemble_fnc);
                    clock_gettime(CLOCK_MONOTONIC, &start);
                    fnd_a_fnc(&enesmble_fnd, &ensemble_fnc);
                    clock_gettime(CLOCK_MONOTONIC, &end);
                    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
                    //printf("nombre de clauses: %d, %fs\n", ensemble_fnc.taille, time_taken);
                    sprintf(nom_fichier, "indices/indice_%d%d%d%d.clauses", i, j, k, l);
                    FILE *fclauses = fopen(nom_fichier, "w");
                    ecrire_clauses(&ensemble_fnc, fclauses);
                    fclose(fclauses);
                    detruire_liste(&ensemble_fnc);
                    detruire_liste(&enesmble_fnd);
                }
            }

        }
        printf("\n");
    }
}

//trouve la FNC correspondant a un indice de tapa donné en paramètre, et la met dans fnc
int trouver_fnc(Grille *G, int i, int j, Liste_arr *fnc){
    

    //printf("Case (%d,%d) a un indice: ", j + 1, i + 1);
    int indice[4] = {0};
    for (int k = 0; k < G->tab[i][j].nb_indice; k++)
    {
        indice[k] = G->tab[i][j].indices[k];
        //printf("%d ", G->tab[i][j].indices[k]);
    }
    //printf("\n");


    int arr[8] = {0};
    //verifie les cases adjacentes a l'indice
    remplis_selon_voisins(arr, i, j, G);
    if(arr[0] != 0 || arr[1] != 0 || arr[2] != 0 || arr[3] != 0 || arr[4] != 0 || arr[5] != 0 || arr[6] != 0 || arr[7] != 0){//si au moins une est forcement blanche ou hors grille, on calcule la FNC a partir de la FND pour eviter d'avoir des un nombre de clauses qui explose en copiant une FNC qui sera réuite par DPLL
        Liste_arr ensemble_fnd;
        initialiser_liste(&ensemble_fnd);

        get_arrangement(indice[0], indice[1], indice[2], indice[3], arr, &ensemble_fnd); 

        fnd_a_fnc(&ensemble_fnd, fnc);
    } else { //sinon on peut directement copier la FNC correspondante a l'indice 
        lire_clause(indice[0], indice[1], indice[2], indice[3], fnc);
    }
    return 0;

}

int indices_to_dimacs(Grille *G, FILE *f, int ignore_numbers)
{
    /// DECOMENTEZ LES LIGNE SUIVANTE POUR CALCULER LES CLAUSES POUR TOUS LES INDICES DE TAPA
    //executez avec nimporte quel flag sur nimporte quel fichier grille (qui a une grille syntaxiquement correcte)


    //initialise_clauses();
    //exit(1); //pour éviter de continuer l'execution apres le calcul des clauses
    
    
    
    
    int nb_clauses = 0; //compteur de clauses pour les indices. 
    
    struct timespec start, end;
    
    char nom_fichier[100];
    
    

    


    for (int i = 0; i < G->hauteur; i++)
    {
        for (int j = 0; j < G->largeur; j++) // pour toutes les cases de la grille
        {
            if (G->tab[i][j].nb_indice > 0) //si il y a un indice sur la case.
            {
                Liste_arr fnc;
                initialiser_liste(&fnc);
                if(ignore_numbers){ //force quand même blanc, même si les nombres sur l'indices sont ignorés.
                    ecrire_dimacs(&fnc, f, G, i, j);
                    continue;
                }
                //printf("Case (%d,%d) a un indice: ", j + 1, i + 1);
                for (int k = 0; k < G->tab[i][j].nb_indice; k++)
                {
                    //printf("%d ", G->tab[i][j].indices[k]);
                }
                //printf("\n");
                //met la FNC correspondant a l'indice de la case dans fnc
                trouver_fnc(G, i, j, &fnc);
                
                nb_clauses += ecrire_dimacs(&fnc, f, G, i, j);

                detruire_liste(&fnc);


            } else if (G->tab[i][j].nb_indice == -1) { //si la case est forcée blanche
                fprintf(f, "\n");
                fprintf(f, "c Case (%d,%d) necessairement blanche\n", j + 1, i + 1);
                fprintf(f, "-%d 0\n", i*G->largeur + j + 1); // clause pour dire que la case doit être blanche
                nb_clauses++;
            } else if (G->tab[i][j].nb_indice == -2) { //si la case est forcée noire
                fprintf(f, "\n");
                fprintf(f, "c Case (%d,%d) necessairement noire\n", j + 1, i + 1);
                fprintf(f, "%d 0\n", i*G->largeur + j + 1); // clause pour dire que la case doit être noire
                nb_clauses++;
            } 
        }
    }

    printf("Nombre total de clauses pour les indices: %d\n", nb_clauses);
    fprintf(f, "\n");
    return nb_clauses;


    
    
}