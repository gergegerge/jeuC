#include <stdio.h>
#include <stdlib.h>
#define TAILLE 10

void inv(int tab[],int taille){
    int* tind = (int*)malloc(sizeof(int)*taille);
    for(int i =0;i<taille;i++){
        tind[i]=tab[taille - 1 -i];
    }
    for(int i =0;i<taille;i++){
        tab[i]=tind[i];
    }
    //free(tind);
}

int main(){
    int t[TAILLE];
    for(int i = 0;i<TAILLE;i++){
        printf("la valeur de t[%d] = ",i);
        scanf("%d",&t[i]);
    }
    inv(t,TAILLE);
    for(int i = 0;i<TAILLE;i++){
        printf("la nouvelle valeur de t[%d] = %d\n",i,t[i]);
    }
    return 0;
}