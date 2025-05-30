#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE 10

struct Commande {
    bool quitter;
    int colonne;
    int ligne;
};

int tailles_bateaux[] = {5, 4, 3, 2};
int nb_bateaux = sizeof(tailles_bateaux) / sizeof(tailles_bateaux[0]);

int peut_placer(char grille[][TAILLE], int ligne, int col, int taille, int horizontal) {
    for (int i = 0; i < taille; i++) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        if (l >= TAILLE || c >= TAILLE || grille[l][c] != '~') return 0;
    }
    return 1;
}

void placer_bateau(char grille[][TAILLE], int taille) {
    int placé = 0;
    while (!placé) {
        int horizontal = rand() % 2;
        int ligne = rand() % TAILLE;
        int col = rand() % TAILLE;

        if ((horizontal && col + taille > TAILLE) ||
            (!horizontal && ligne + taille > TAILLE)) {
            continue;
        }

        if (peut_placer(grille, ligne, col, taille, horizontal)) {
            for (int i = 0; i < taille; i++) {
                int l = ligne + (horizontal ? 0 : i);
                int c = col + (horizontal ? i : 0);
                grille[l][c] = 'X';
            }
            placé = 1;
        }
    }
}

void afficher_plateau(char grille[][TAILLE]) {
    printf("  ");
    for (int j = 0; j < TAILLE; j++) {
        printf("%d ", j);
    }
    printf("\n");

    for (int i = 0; i < TAILLE; i++) {
        printf("%c ", 'A' + i);
        for (int j = 0; j < TAILLE; j++) {
            printf("%c ", grille[i][j]);
        }
        printf("\n");
    }
}

int tous_bateaux_coules(char grille[][TAILLE]) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (grille[i][j] == 'X') return 0;
        }
    }
    return 1;
}

void tirer(char (plateau)[][TAILLE], char (plateau_ennemi)[][TAILLE], int ligne, int col_tir){
    if (plateau_ennemi[ligne][col_tir] == 'X') {
        printf("Touché !\n");
        plateau[ligne][col_tir] = 'X';
        plateau_ennemi[ligne][col_tir] = '*';
    }
    else {
        printf("Raté.\n");
        plateau[ligne][col_tir] = 'O';
        }
}

struct Commande saisir_commande(char plateau[][TAILLE]) {

    char ligne_tir;
    int ligne = 0;
    int col_tir = 0;
    _Bool deja_testé = 0;
    int quitter = 0;
    _Bool dans_tableau = 0;

    while (true) {
        printf("Entrez une case pour tirer (ex: B4 ou B puis 4) ou 'Q' pour quitter : ");
        scanf(" %c", &ligne_tir);
        if (ligne_tir == 'Q' || ligne_tir == 'q'){
            quitter = 1;
            break;
        }
        ligne_tir = toupper(ligne_tir);
        ligne = ligne_tir - 'A';

        scanf("%d", &col_tir);
        dans_tableau = ligne >= 0 && ligne < TAILLE && col_tir >= 0 && col_tir < TAILLE;
        if (dans_tableau) {
            deja_testé = plateau[(ligne)][col_tir] == 'X' || plateau[(ligne)][col_tir] == 'O';
            if (deja_testé) {
                printf("Vous avez déjà tiré ici. Choisissez une autre case.\n");
            }
            else {
                break;
            }
        } else {
            printf("Coordonnées invalides. Essayez encore.\n");
        }
    }

    if (quitter) {
        struct Commande commande = { .quitter = 1, .colonne = 0, .ligne = 0 };
        return commande;
    }
    struct Commande commande = { .quitter = 0, .colonne = col_tir, .ligne = ligne };
    return commande;
}

void initialiser_plateau(char plateau[][TAILLE], char plateau_ennemi[][TAILLE]) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            plateau[i][j] = '~';
            plateau_ennemi[i][j] = '~';
        }
    }

    for (int i = 0; i < nb_bateaux; i++) {
        placer_bateau(plateau_ennemi, tailles_bateaux[i]);
    }
}

int main(void) {
    char plateau[TAILLE][TAILLE];
    char plateau_ennemi[TAILLE][TAILLE];
    srand(time(NULL));

    initialiser_plateau(plateau, plateau_ennemi);

    while (!tous_bateaux_coules(plateau_ennemi)) {
        afficher_plateau(plateau);

        struct Commande commande = saisir_commande(plateau);
        if (commande.quitter) {
            printf("\nVous avez quitté la partie.\n");
            exit(0);
        }

        tirer(plateau, plateau_ennemi, commande.ligne, commande.colonne);
    }

    if (tous_bateaux_coules(plateau_ennemi)) {
        printf("\n🎉 Bravo, vous avez coulé tous les bateaux ennemis !\n");
        afficher_plateau(plateau);
    }

    return 0;
}
