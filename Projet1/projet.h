#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX_ITE 100000
#define PRECISION 0.001
#define NB_MAX_POLYNOMES 10

typedef struct {
    float *liste;  
    int taille;   
} Polynome;

Polynome creerPolynome(float *tableau, int taille);
void afficherliste(float *liste, int taille);
float *exposants(Polynome P);
float *coefficients(Polynome P);
void tri_fusion_polynome(float coeff[], float exp[], int i, int j);
void trierPolynome(Polynome *P);
void afficherPolynome(Polynome P);
Polynome somme(Polynome P1, Polynome P2);
Polynome produit(Polynome P1, Polynome P2);
Polynome derivee(Polynome P);
float evalue(Polynome P, float x);
Polynome developpement_limite(Polynome P, float a, int n);
double racine(Polynome P, float a, float b);
void vider_log();
void log_action(char *msg);
Polynome creer_polynome();
void charger_num_session();
void menu();