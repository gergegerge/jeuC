#include <stdio.h>

int doublon(float tab[], float element, int taille) {
    int u = 0;
    for (int i = 0; i < taille; i++) {
        if (tab[i] == element) {
            u++;
        }
    }
    if(u>0){
        return 0;
    }
    return 1; 
}

int copy(float tabe[],int taille,float tabs[]){
    int v = 0;
    for(int i = 0 ; i < taille ; i++){
        if(doublon(tabs,tabe[i],v)){
            tabs[v]=tabe[i];
            v++;
        }
    }
    return v;
}

int main(){
    int n;
    printf("la taille du tableau est :\n");
    scanf("%d",&n);
    float te[n];
    float ts[n];
    for(int i = 0 ; i < n ; i++){
        printf("la valeur t[%d] est",i);
        scanf("%f",&te[i]);
    }
    int v = copy(te,n,ts);
    for(int i = 0 ; i < v ; i++){
        printf("la valeur ts[%d] est %f\n",i,ts[i]);
    }
    printf("taille de ts = %d",v);
    return 0;
}