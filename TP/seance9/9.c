#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Compte {
    char nom[50];
    float montant;
    struct Compte* suivant;
} Compte;

typedef struct {
    Compte* tete;
} ListeComptes;

Compte* creerCompte(const char* nom, float montant) {
    Compte* nouveau = (Compte*)malloc(sizeof(Compte));
    strcpy(nouveau->nom, nom);
    nouveau->montant = montant;
    nouveau->suivant = NULL;
    return nouveau;
}


void ajouterCompte(ListeComptes* liste, const char* nom, float montant) {
    Compte* nouveau = creerCompte(nom, montant);    
    if (liste->tete == NULL) {
        liste->tete = nouveau;
    } else {
        Compte* courant = liste->tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouveau;
    }
}

void lireFichier(ListeComptes* liste, const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    char nom[50];
    float montant; 
    while (fscanf(fichier, "%s %f", nom, &montant) == 2) {
        ajouterCompte(liste, nom, montant);
    }
    fclose(fichier);
}

void afficherListe(ListeComptes* liste) {
    if (liste->tete == NULL) {
        printf("La liste est vide\n");
        return;
    }
    Compte* courant = liste->tete;
    int numero = 1;
    
    while (courant != NULL) {
        printf("%d. %s : %f €\n", numero, courant->nom, courant->montant);
        courant = courant->suivant;
        numero++;
    }
    printf("Nombre de comptes : %d\n", numero - 1);
}

void sauvegarderFichier(ListeComptes* liste, char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "w");
    Compte* courant = liste->tete;  
    while (courant != NULL) {
        fprintf(fichier, "%s %f\n", courant->nom, courant->montant);
        courant = courant->suivant;
    }
    fclose(fichier);
    printf("Comptes sauvegardes\n");
}

void deposerRetirerArgent(ListeComptes* liste) {
    char nom[50];
    float montant;
    
    printf("Entrez le nom du compte : ");
    scanf("%s", nom);
    
    Compte* courant = liste->tete;
    
    while (courant != NULL) {
        if (strcmp(courant->nom, nom) == 0) {
            printf("Montant actuel : %f euros\n", courant->montant);
            printf("Entrez le montant : ");
            scanf("%f", &montant);
            courant->montant = courant->montant + montant;
            printf("Nouveau montant : %f euros\n", courant->montant);
            return;
        }
        courant = courant->suivant;
    }
    printf("Compte non trouve\n");
}

void supprimerCompte(ListeComptes* liste) {
    char nom[50];
    printf("Entrez le nom du compte a supprimer : ");
    scanf("%s", nom);
    
    Compte* precedent = NULL;
    Compte* courant = liste->tete;
    
    while (courant != NULL) {
        if (strcmp(courant->nom, nom) == 0) {
            if (precedent == NULL) {
                liste->tete = courant->suivant;
            } else {
                precedent->suivant = courant->suivant;
            }
            printf("Compte supprime\n");
            return;
        }
        precedent = courant;
        courant = courant->suivant;
    }
    printf("Compte non trouve\n");
}

void ajoutercompte(ListeComptes* liste) {
    char nom[50];
    float montant;
    
    printf("Entrez le nom du nouveau compte : ");
    scanf("%s", nom);
    printf("Entrez le montant initial : ");
    scanf("%f", &montant);
    
    Compte* nouveau = (Compte*)malloc(sizeof(Compte));
    strcpy(nouveau->nom, nom);
    nouveau->montant = montant;
    nouveau->suivant = NULL;
    
    if (liste->tete == NULL) {
        liste->tete = nouveau;
    } else {
        Compte* precedent = NULL;
        Compte* courant = liste->tete;
        
        while (courant != NULL && strcmp(courant->nom, nom) < 0) {
            precedent = courant;
            courant = courant->suivant;
        }
        
        if (precedent == NULL) {
            nouveau->suivant = liste->tete;
            liste->tete = nouveau;
        } else {
            nouveau->suivant = courant;
            precedent->suivant = nouveau;
        }
    }
    
    printf("Compte ajoute\n");
}

void afficherUnCompte(ListeComptes* liste) {
    char nom[50];
    printf("Entrez le nom du compte : ");
    scanf("%s", nom);
    
    Compte* courant = liste->tete;
    
    while (courant != NULL) {
        if (strcmp(courant->nom, nom) == 0) {
            printf("%s : %f euros\n", courant->nom, courant->montant);
            return;
        }
        courant = courant->suivant;
    }
    
    printf("Compte non trouve\n");
}

void menu(void){
    ListeComptes liste;
    liste.tete = NULL;
    lireFichier(&liste, "comptes.txt");
    bool quitter = false;
    int choix;
    while (!quitter){
    printf("1 : afficher liste\n2 : afficher compte\n3 : depot ou retrait d'argent\n4 : ajouter compte\n5 : supprimer compte\n6 : quitter\n");
    scanf("%d",&choix);
    switch(choix){
        case 1:
        afficherListe(&liste);
        break;
        case 2:
        afficherUnCompte(&liste);
        break;
        case 3:
        deposerRetirerArgent(&liste);
        break;
        case 4:
        ajoutercompte(&liste);
        sauvegarderFichier(&liste,"comptes.txt");
        break; 
        case 5:
        supprimerCompte(&liste);
        break;
        case 6:
        sauvegarderFichier(&liste,"comptes.txt");
        quitter = true;
        break;
    }
    }
}

int main() {
    menu();
    return 0;
}