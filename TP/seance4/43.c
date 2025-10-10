#include <stdio.h>


int fibonnacci_rec(int n){
    if(n==1){
        return 1;
    }
    if(n==0){
        return 0;
    }
    else {
        return fibonnacci_rec(n-1)+fibonnacci_rec(n-2);
    }
}

int fibonnacci_ite(int n){
    if(n==0){
        return 0;
    }
    int v = 1;
    int g = 0;
    int f;
    for (int i = 1;i<n;i++){
        f = g + v;
        g = v;
        v = f; 
    }
    return f;

}

int main(){
    int n;
    printf("on va calculer fibonnacci de : ");
    scanf("%d",&n);
    int f1 = fibonnacci_rec(n);
    int f2 = fibonnacci_ite(n);
    printf("résultats :\n - avec rec : %d\n - avec ite : %d",f1,f2);
    return 0;
}