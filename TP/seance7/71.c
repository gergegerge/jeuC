#include <stdio.h>

struct panneau{
    float longueur, largeur, epaisseur;
    int codebois;
};

struct panneau saisie(){
    struct panneau p;
    float a,b,c;
    int d;
    printf("entrer les valeurs de longueur,largeur,epaisseur puis le code\n");
    scanf("%f\n %f\n %f\n %d\n",&a,&b,&c,&d);
    p.longueur = a;
    p.largeur = b;
    p.epaisseur = c;
    p.codebois = d;
    return p;
}

void affichage(struct panneau p){
    printf("%f\n%f\n%f\n%d\n",p.longueur,p.largeur,p.epaisseur,p.codebois);
}

float volume(struct panneau p){
    float V = p.longueur*p.largeur*p.epaisseur;
    return V;
}

int main(){
    struct panneau p = saisie();
    affichage(p);
    float Volume = volume(p);
    printf("le volume est de %f\n",Volume);
    return 0;
}