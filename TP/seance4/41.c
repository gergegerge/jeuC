#include <stdio.h>

int pgcd_rec(int a, int b){
    if (a>=b){
        if (a%b==0){
            return b;
        } else {
            return pgcd_rec(a, a%b);
        }
    } else {
        if (b%a==0){
            return a;
        } else {
            return pgcd_rec(b%a, b);
        }
    }
}

int pgcd_ite(int a , int b){
    int p = 1;
    if (a<=b){
        for(int i = 1;i<=a;i++){
            if (a%i == 0){
                if(b%i==0){
                    p = i;
                }
            }
        }
    }
    else {
        for(int i = 1;i<=b;i++){
            if (b%i == 0){
                if(a%i==0){
                    p = i;
                }
            }
        }
    }
    return p;
}

int main(){
    int a;
    int b;
    printf("les deux entiers dont on calcul le pgcd sont : ");
    scanf("%d",&a);
    printf("et : ");
    scanf("%d",&b);
    int p = pgcd_rec(a,b);
    int v = pgcd_ite(a,b);
    printf("pgcd(%d,%d) = %d avec rec %d avec ite",a,b,p,v);
    return 0;
}