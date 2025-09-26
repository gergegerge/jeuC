#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LARGEUR 50
#define HAUTEUR 25
#define VIVANT '#'
#define MORT ' '

// Structure pour représenter la grille
typedef struct {
    char grille[HAUTEUR][LARGEUR];
} Monde;

// Initialise une grille vide
void initialiser_monde(Monde *monde) {
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            monde->grille[i][j] = MORT;
        }
    }
}

// Affiche la grille
void afficher_monde(Monde *monde) {
    // Efface l'écran
    system("clear");
    
    // Affiche le cadre supérieur
    printf("+");
    for (int j = 0; j < LARGEUR; j++) {
        printf("-");
    }
    printf("+\n");
    
    // Affiche la grille avec les bordures
    for (int i = 0; i < HAUTEUR; i++) {
        printf("|");
        for (int j = 0; j < LARGEUR; j++) {
            printf("%c", monde->grille[i][j]);
        }
        printf("|\n");
    }
    
    // Affiche le cadre inférieur
    printf("+");
    for (int j = 0; j < LARGEUR; j++) {
        printf("-");
    }
    printf("+\n");
}

// Compte les voisins vivants d'une cellule
int compter_voisins(Monde *monde, int x, int y) {
    int count = 0;
    
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // Ne pas compter la cellule elle-même
            
            int nx = x + i;
            int ny = y + j;
            
            // Vérifier les limites avec effet de bord toroïdal (optionnel)
            // Pour des bords fixes, décommentez les lignes suivantes :
            // if (nx < 0 || nx >= HAUTEUR || ny < 0 || ny >= LARGEUR) continue;
            
            // Bords toroïdaux (la grille "s'enroule")
            nx = (nx + HAUTEUR) % HAUTEUR;
            ny = (ny + LARGEUR) % LARGEUR;
            
            if (monde->grille[nx][ny] == VIVANT) {
                count++;
            }
        }
    }
    
    return count;
}

// Applique les règles du jeu de la vie
void nouvelle_generation(Monde *monde_actuel, Monde *nouveau_monde) {
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            int voisins = compter_voisins(monde_actuel, i, j);
            char cellule_actuelle = monde_actuel->grille[i][j];
            
            // Règles du Jeu de la Vie :
            if (cellule_actuelle == VIVANT) {
                // Une cellule vivante avec 2 ou 3 voisins survit
                if (voisins == 2 || voisins == 3) {
                    nouveau_monde->grille[i][j] = VIVANT;
                } else {
                    nouveau_monde->grille[i][j] = MORT;
                }
            } else {
                // Une cellule morte avec exactement 3 voisins naît
                if (voisins == 3) {
                    nouveau_monde->grille[i][j] = VIVANT;
                } else {
                    nouveau_monde->grille[i][j] = MORT;
                }
            }
        }
    }
}

// Place un motif "planeur" dans la grille
void placer_planeur(Monde *monde, int start_x, int start_y) {
    int planeur[3][3] = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 1}
    };
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int x = (start_x + i) % HAUTEUR;
            int y = (start_y + j) % LARGEUR;
            if (planeur[i][j]) {
                monde->grille[x][y] = VIVANT;
            }
        }
    }
}

// Place un motif "clignotant" (oscillateur période 2)
void placer_clignotant(Monde *monde, int start_x, int start_y) {
    monde->grille[start_x][start_y] = VIVANT;
    monde->grille[start_x][start_y + 1] = VIVANT;
    monde->grille[start_x][start_y + 2] = VIVANT;
}

// Place un motif "bloc" (nature morte)
void placer_bloc(Monde *monde, int start_x, int start_y) {
    monde->grille[start_x][start_y] = VIVANT;
    monde->grille[start_x][start_y + 1] = VIVANT;
    monde->grille[start_x + 1][start_y] = VIVANT;
    monde->grille[start_x + 1][start_y + 1] = VIVANT;
}

// Initialise avec quelques motifs connus
void initialiser_avec_motifs(Monde *monde) {
    initialiser_monde(monde);
    
    // Quelques motifs intéressants
    placer_planeur(monde, 2, 2);
    placer_clignotant(monde, 10, 10);
    placer_bloc(monde, 15, 20);
    placer_planeur(monde, 5, 30);
}

int main() {
    Monde monde1, monde2;
    Monde *monde_actuel = &monde1;
    Monde *nouveau_monde = &monde2;
    int generation = 0;
    
    printf("=== JEU DE LA VIE DE CONWAY ===\n");
    printf("Appuyez sur Ctrl+C pour arrêter\n\n");
    sleep(2);
    
    // Initialisation avec des motifs prédéfinis
    initialiser_avec_motifs(monde_actuel);
    
    // Boucle principale
    while (1) {
        printf("Génération: %d\n", generation);
        afficher_monde(monde_actuel);
        
        // Calcule la nouvelle génération
        nouvelle_generation(monde_actuel, nouveau_monde);
        
        // Échange les mondes
        Monde *temp = monde_actuel;
        monde_actuel = nouveau_monde;
        nouveau_monde = temp;
        
        generation++;
        
        // Attendre un peu avant la prochaine génération
        usleep(200000); // 200ms
    }
    
    return 0;
}

/* 
COMPILATION ET EXÉCUTION :
gcc -o jeu_de_la_vie jeu_de_la_vie.c
./jeu_de_la_vie

RÈGLES DU JEU DE LA VIE :
1. Toute cellule vivante avec moins de 2 voisins meurt (sous-population)
2. Toute cellule vivante avec 2 ou 3 voisins survit à la génération suivante
3. Toute cellule vivante avec plus de 3 voisins meurt (surpopulation)
4. Toute cellule morte avec exactement 3 voisins devient vivante (reproduction)

MOTIFS INCLUS :
- Planeur : se déplace en diagonal
- Clignotant : oscille entre deux états
- Bloc : reste stable (nature morte)
*/