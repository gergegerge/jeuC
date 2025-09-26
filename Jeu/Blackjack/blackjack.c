#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_CARTES 10
#define DECK_SIZE 52

typedef struct {
    int valeur;
    char couleur[10];
    char nom[10];
} Carte;

typedef struct {
    Carte cartes[MAX_CARTES];
    int nb_cartes;
    int total;
    int as_compte; // nombre d'as comptés comme 11
} Main;

// Initialise le deck de cartes
void initialiser_deck(Carte deck[]) {
    char couleurs[4][10] = {"Coeur", "Carreau", "Trefle", "Pique"};
    char noms[13][10] = {"As", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Valet", "Dame", "Roi"};
    int valeurs[13] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
    
    int index = 0;
    for (int c = 0; c < 4; c++) {
        for (int v = 0; v < 13; v++) {
            deck[index].valeur = valeurs[v];
            strcpy(deck[index].couleur, couleurs[c]);
            strcpy(deck[index].nom, noms[v]);
            index++;
        }
    }
}

// Mélange le deck
void melanger_deck(Carte deck[]) {
    for (int i = 0; i < DECK_SIZE; i++) {
        int j = rand() % DECK_SIZE;
        Carte temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// Initialise une main
void initialiser_main(Main *main) {
    main->nb_cartes = 0;
    main->total = 0;
    main->as_compte = 0;
}

// Ajoute une carte à la main
void ajouter_carte(Main *main, Carte carte) {
    main->cartes[main->nb_cartes] = carte;
    main->nb_cartes++;
    
    if (carte.valeur == 11) { // As
        main->as_compte++;
    }
    
    main->total += carte.valeur;
    
    // Ajuste la valeur des As si nécessaire
    while (main->total > 21 && main->as_compte > 0) {
        main->total -= 10; // Change un As de 11 à 1
        main->as_compte--;
    }
}

// Affiche une carte
void afficher_carte(Carte carte) {
    printf("%s de %s", carte.nom, carte.couleur);
}

// Affiche la main
void afficher_main(Main main, const char* nom, int cacher_premiere) {
    printf("\n%s (Total: %d):\n", nom, cacher_premiere ? 0 : main.total);
    
    for (int i = 0; i < main.nb_cartes; i++) {
        if (i == 0 && cacher_premiere) {
            printf("  [Carte cachée]\n");
        } else {
            printf("  ");
            afficher_carte(main.cartes[i]);
            printf("\n");
        }
    }
}

// Vérifie si c'est un blackjack naturel
int est_blackjack(Main main) {
    return (main.nb_cartes == 2 && main.total == 21);
}

// Vérifie si la main est éclatée
int est_eclate(Main main) {
    return main.total > 21;
}

// Demande au joueur s'il veut tirer une carte
int demander_action() {
    char choix;
    printf("\nVoulez-vous tirer une carte? (o/n): ");
    scanf(" %c", &choix);
    return (choix == 'o' || choix == 'O');
}

// Logique du croupier
void jouer_croupier(Main *croupier, Carte deck[], int *index_deck) {
    printf("\nLe croupier révèle sa carte cachée...\n");
    afficher_main(*croupier, "Main du Croupier", 0);
    
    while (croupier->total < 17) {
        printf("\nLe croupier tire une carte...\n");
        ajouter_carte(croupier, deck[(*index_deck)++]);
        afficher_main(*croupier, "Main du Croupier", 0);
        
        if (est_eclate(*croupier)) {
            printf("\nLe croupier a éclaté!\n");
            return;
        }
    }
    
    printf("\nLe croupier s'arrête.\n");
}

// Détermine le gagnant
void determiner_gagnant(Main joueur, Main croupier) {
    printf("\n==================================================\n");
    printf("RÉSULTAT FINAL:\n");
    printf("==================================================\n");
    
    if (est_eclate(joueur)) {
        printf("Vous avez éclaté! Vous perdez.\n");
    } else if (est_eclate(croupier)) {
        printf("Le croupier a éclaté! Vous gagnez!\n");
    } else if (est_blackjack(joueur) && !est_blackjack(croupier)) {
        printf("BLACKJACK! Vous gagnez!\n");
    } else if (est_blackjack(croupier) && !est_blackjack(joueur)) {
        printf("Le croupier a un blackjack! Vous perdez.\n");
    } else if (joueur.total > croupier.total) {
        printf("Vous gagnez! (%d vs %d)\n", joueur.total, croupier.total);
    } else if (joueur.total < croupier.total) {
        printf("Vous perdez! (%d vs %d)\n", joueur.total, croupier.total);
    } else {
        printf("Égalité! (%d vs %d)\n", joueur.total, croupier.total);
    }
}

// Fonction principale du jeu
void jouer_partie() {
    Carte deck[DECK_SIZE];
    Main joueur, croupier;
    int index_deck = 0;
    
    // Initialisation
    initialiser_deck(deck);
    melanger_deck(deck);
    initialiser_main(&joueur);
    initialiser_main(&croupier);
    
    printf("\n==================================================\n");
    printf("    BIENVENUE AU BLACKJACK!\n");
    printf("==================================================\n");
    
    // Distribution initiale
    ajouter_carte(&joueur, deck[index_deck++]);
    ajouter_carte(&croupier, deck[index_deck++]);
    ajouter_carte(&joueur, deck[index_deck++]);
    ajouter_carte(&croupier, deck[index_deck++]);
    
    // Affichage initial
    afficher_main(joueur, "Votre main", 0);
    afficher_main(croupier, "Main du Croupier", 1);
    
    // Vérification blackjack naturel
    if (est_blackjack(joueur)) {
        printf("\nBLACKJACK NATUREL!\n");
        afficher_main(croupier, "Main du Croupier", 0);
        determiner_gagnant(joueur, croupier);
        return;
    }
    
    // Tour du joueur
    while (!est_eclate(joueur)) {
        if (!demander_action()) {
            break;
        }
        
        ajouter_carte(&joueur, deck[index_deck++]);
        afficher_main(joueur, "Votre main", 0);
        
        if (est_eclate(joueur)) {
            printf("\nVous avez éclaté!\n");
            determiner_gagnant(joueur, croupier);
            return;
        }
    }
    
    // Tour du croupier si le joueur n'a pas éclaté
    if (!est_eclate(joueur)) {
        jouer_croupier(&croupier, deck, &index_deck);
    }
    
    // Détermination du gagnant
    determiner_gagnant(joueur, croupier);
}

int main() {
    srand(time(NULL));
    char rejouer;
    
    do {
        jouer_partie();
        printf("\nVoulez-vous rejouer? (o/n): ");
        scanf(" %c", &rejouer);
        printf("\n");
    } while (rejouer == 'o' || rejouer == 'O');
    
    printf("Merci d'avoir joué au Blackjack!\n");
    return 0;
}