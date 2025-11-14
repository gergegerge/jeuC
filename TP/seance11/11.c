#include <stdio.h>
#include <stdlib.h>
#define TAILLE_MAX 100

typedef struct
{
int tab[TAILLE_MAX]; // définir une valeur pour TAILLE_MAX (100 par ex.)
int taille; // nb d'éléments rangés dans le tableau (entre 0 et TAILLE_MAX )
} Tas;

void creertasvide(Tas *tas){
    tas->taille =0;
}

int indiceFilsGauche(int i){
    return 2*i +1;
}

int indiceFilsDroit(int i){
    return 2*(i+1);
}

int indicePere(int i){
    int a = (i - 1) / 2;
    return a;
}

int nbFils(Tas *tas, int i){
    if (tas->taille>indiceFilsDroit(i)){
        return 2;
    }
    else if (tas->taille>indiceFilsGauche(i)){
        return 1;
    }
    else {
        return 0;
    }
}

int filsGauche(Tas *tas, int i){
    return tas->tab[indiceFilsGauche(i)];
}

int filsDroit(Tas *tas,int i){
    return tas->tab[indiceFilsDroit(i)];
}

int pere(Tas *tas, int i){
    return tas->tab[indicePere(i)];
}

int nœud(Tas *tas, int i){
    return tas->tab[i];
}

void afficherTas(Tas *tas){
    int i = 0;
    int n = 1;
    int v = 0;
    while (i<tas->taille){
        if (nbFils(tas,i)==2){
            printf("%d (%d %d)   ",nœud(tas,i),filsGauche(tas,i),filsDroit(tas,i));
        }
        if (nbFils(tas,i)==1){
            printf("%d (%d)   ",nœud(tas,i),filsGauche(tas,i));
        }
        if (nbFils(tas,i)==0){
            printf("%d   ",nœud(tas,i));
        }
        v++;
        if (v >= n){
            v = 0;
            n = n * 2;
            printf("\n");
        }
        i++;
    }
}

/* void insererTas(Tas *tas, int e){
    int i = 0;
    int ind;
    while (nbFils(tas,i)==2){
        i++;
    }
    if (nbFils(tas,i)==1){
        tas->tab[indiceFilsDroit(i)]=e;
        ind = indiceFilsDroit(i);
    }
    else if (nbFils(tas,i)==0){
        tas->tab[indiceFilsGauche(i)]=e;
        ind = indiceFilsGauche(i);
    }
    while (nœud(tas,indicePere(ind))<e){
        tas->tab[ind]=tas->tab[indicePere(ind)];
        ind = indicePere(ind);
        tas->tab[ind]=e;
    }
} */

void insererTas(Tas *tas,int e){
    tas->taille++;
    int ind = tas->taille-1;
    tas->tab[ind]=e;
    while (nœud(tas,indicePere(ind))<e&&ind!=0){
        tas->tab[ind]=tas->tab[indicePere(ind)];
        ind = indicePere(ind);
    }
    tas->tab[ind]=e;
}

void creerTas(Tas *tas, int *t, int taille){
    for(int i = 0;i<taille;i++){
        insererTas(tas,t[i]);
    }
}


int main(){
    Tas tas1 = { {18, 16, 12, 10, 9, 11, 5, 4, 6, 3}, 10 };
    afficherTas(&tas1);
    insererTas(&tas1,15);
    printf("\n\n");
    afficherTas(&tas1);
    printf ("\n\n nouveau \n\n");
    int t2[10] = {6, 16, 4, 10, 9, 11, 5, 12, 18, 3};
    Tas tas2;
    tas2.taille =0;
    creerTas(&tas2,t2,10);
    afficherTas(&tas2);
    return 0;
}


