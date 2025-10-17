#include <stdio.h>
#include <stdlib.h>
#define TAILLE1 10
#define TAILLE2 10

// A REFAIRE

int* conca(int t1[],int t2[],int taille1,int taille2){
    int *tint = (int*)malloc(sizeof(int)*(taille1+taille2));
    for(int i = 0;i<taille1+taille2;i++){
        if (i < taille1){
            tint[i]=t1[i];
        }
        else {
            tint[i]=t2[i - taille1];
        }
        
    }
    int TF[taille1+taille2];
    for(int i = 0;i<taille1+taille2;i++){
        TF[i]=tint[i];
    }
    free(tint);
    return TF;
}

int main(){
    int t1[TAILLE1];
    for(int i = 0;i<TAILLE1;i++){
        printf("la valeur de t[%d] = ",i);
        scanf("%d",&t1[i]);
    }
    int t2[TAILLE2];
    for(int i = 0;i<TAILLE2;i++){
        printf("la valeur de t[%d] = ",i);
        scanf("%d",&t2[i]);
    }
    int T = conca(t1,t2,TAILLE1,TAILLE2);
    for(int i = 0;i<TAILLE1+TAILLE2;i++){
        printf("T[%d] = %d",i,T[i]);
    }
    return 0;
}