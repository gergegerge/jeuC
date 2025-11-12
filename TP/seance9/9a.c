#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fichier;
    int nb_comptes, i;
    char nom[50];
    float montant;
    
    fichier = fopen("comptes.txt", "w");
    
    if (fichier == NULL) {
        printf("Erreur : impossible de créer le fichier.\n");
        return 1;
    }
    
    printf("Combien de comptes voulez-vous enregistrer ? ");
    scanf("%d", &nb_comptes);

    for (i = 0; i < nb_comptes; i++) {
        printf("\n--- Compte %d ---\n", i + 1);
        printf("Nom du titulaire : ");
        scanf("%s", nom);
        printf("Montant : ");
        scanf("%f", &montant);
        fprintf(fichier, "%s %.2f\n", nom, montant);
    }
    
    fclose(fichier);
    printf("%d compte(s) enregistré(s).\n", nb_comptes);
    
    return 0;
}