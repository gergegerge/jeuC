#include <stdio.h>
#define TAILLE_T 10


int recherche_dichotomique(int tab[],int element,int taille){
    int ind_m = taille/2;
    int deb = 0;
    int fin = taille - 1;
    int compteur_op = 0;
    while (tab[ind_m]!=element){
        if(tab[ind_m]<element){
            deb = ind_m;
            ind_m = (fin + deb)/2;
            printf("%d\n",ind_m);
        }
        else if(tab[ind_m]>element){
            fin = ind_m;
            ind_m = (fin + deb)/2;
            printf("%d\n",ind_m);
        }
  
       
    compteur_op++;
    }
    printf("il y a eu %d opérations \n",compteur_op);
    return ind_m;
        
}


int main(){
    /* int n;
    printf("la taille de la liste est de : ");
    scanf("%d\n",&n); */
    int t[TAILLE_T];
    for(int i = 0 ; i < TAILLE_T ; i++){
        printf("la valeur t[%d] est : ",i);
        scanf("%d",&t[i]);
    }
    int element;
    printf("l'element recherche est : ");
    scanf("%d",&element);
    int ind = recherche_dichotomique(t,element,TAILLE_T);
    printf("l'indice de l'element est : %d",ind);
    return 0;
}