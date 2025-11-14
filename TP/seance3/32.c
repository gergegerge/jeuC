#include <stdio.h>
#define C 20
int F(int n){
    if(n==0){
        return 0;
    }
    int p =3*n*n+n+1;
    return p;
}

int somme(int n){
    int S = 0;
    for(int i =0;i<n;i++){
        S = S + F(n-i);
    }
    return S;
}

int Tn(int n){
    int tn;
    for(int i=1;i<n;i++){
        if(n/i*i==0){
            return tn;
        }
        tn = tn + F(n/i*i);
    }
}

int Un(int n){
    int un;
    for(int i=1;i<n/2+2;i++){
        if(n/i*2==0){
            return un;
        }
        if(i%2==0){
            un = un + F(n/i*2);
        }
    }
}

int main(){
    int f = F(C);
    int s = somme(C);
    int t = Tn(C);
    int u = Un(C);
    printf("f = %d\n s = %d\n t = %d\n u = %d\n",f,s,t,u);
    return 0;
}