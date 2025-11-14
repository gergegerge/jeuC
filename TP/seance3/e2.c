#include <stdio.h>

int somme_div(int nombre){
    int somme=0;
    for(int i = 1; i<nombre;i++){
        if(nombre%i==0){
            somme = somme + i;
        }
    }
    return somme;
}

int est_parfait(int nombre){
    if(nombre==somme_div(nombre)){
        return 1;
    }
    return 0;
}

int main(){
    int n;
    printf("afficher les nombres parfait jusqu'à : ");
    scanf("%d",&n);
    for(int i = 1;i<n+1;i++){
        if(est_parfait(i)){
            printf("%d est parfait \n",i);
        }
    }
    return 0;
}