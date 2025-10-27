#include <stdio.h>

struct nombre_rationel{
    int denominateur,numerateur;
};

struct nombre_rationel saisie(){
    struct nombre_rationel n;
    float a,b;
    printf("entrer denominateur puis numerateur\n");
    scanf("%f %f",&a,&b);
    n.denominateur = a;
    n.numerateur = b;
    return n;
}

void affichage(struct nombre_rationel n){
    printf("(%d,%d)\n",n.denominateur,n.numerateur);
}

struct nombre_rationel addition_non_simplifie(struct nombre_rationel n1,struct nombre_rationel n2){
    struct nombre_rationel N;
    N.denominateur = n1.denominateur*n2.denominateur;
    N.numerateur = n1.numerateur*n2.denominateur+n1.denominateur*n2.numerateur;
    return N;
}

struct nombre_rationel multiplication_non_simplifie(struct nombre_rationel n1,struct nombre_rationel n2){
    struct nombre_rationel N;
    N.denominateur = n1.denominateur*n2.denominateur;
    N.numerateur = n1.numerateur*n2.numerateur;
    return N;
}

int pgcd(int a,int b){
    if(b==0){
        return a;
    }
    return pgcd(b,a%b);
}

struct nombre_rationel addition(struct nombre_rationel n1,struct nombre_rationel n2){
    struct nombre_rationel N = addition_non_simplifie(n1,n2);
    int div = pgcd(N.denominateur,N.numerateur);
    N.denominateur = N.denominateur/div;
    N.numerateur = N.numerateur/div;
    return N;
}

struct nombre_rationel multiplication(struct nombre_rationel n1,struct nombre_rationel n2){
    struct nombre_rationel N = multiplication_non_simplifie(n1,n2);
    int div = pgcd(N.denominateur,N.numerateur);
    N.denominateur = N.denominateur/div;
    N.numerateur = N.numerateur/div;
    return N;
}

int main(){
    struct nombre_rationel n1 = saisie();
    affichage(n1);
    struct nombre_rationel n2 = saisie();
    struct nombre_rationel n3 = addition_non_simplifie(n1,n2);
    printf("addition non simplifie : \n");
    affichage(n3) ;
    struct nombre_rationel n4 = addition(n1,n2);
    printf("addition : \n");
    affichage(n4);
    struct nombre_rationel n5 = multiplication_non_simplifie(n1,n2);
    printf("multiplication non simplifie : \n");
    affichage(n5) ;
    struct nombre_rationel n6 = multiplication(n1,n2);
    printf("multiplication : \n");
    affichage(n6) ;
    return 0;
}