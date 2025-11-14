#include <stdio.h>
#include <math.h>

struct point{
    float x,y;
};

struct point saisie(){
    struct point p;
    float a,b;
    printf("entrer la position\n");
    scanf("%f %f",&a,&b);
    p.x = a;
    p.y = b;
    return p;
}

void affichage(struct point p){
    printf("(%f,%f)\n",p.x,p.y);
}

float calcul_distance(struct point p1,struct point p2){
    float distance = sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
    return distance;
}

struct cercle{
    struct point p;
    float rayon;
};

struct cercle saisiec(){
    struct cercle c;
    struct point p = saisie();
    float rayon;
    printf("entrer le rayon\n");
    scanf("%f",&rayon);
    c.p = p;
    c.rayon = rayon;
    return c;
}

void affichagec(struct cercle c){
    printf("centre : (%f,%f)\nrayon : %f\n",c.p.x,c.p.y,c.rayon);
}

void est_dedans(struct cercle c,struct point p){
    float d = calcul_distance(c.p,p);
    if(d<c.rayon){
        printf("le point est dans le cercle!\n");
        return;
    }
    printf("le point est hors du cercle!\n");
    
}

int main(){
    struct point p1 = saisie();
    affichage(p1);
    struct point p2 = saisie();
    float d = calcul_distance(p1,p2);
    printf("la distance entre p1 et p2 est de : %f\n",d);
    struct cercle c = saisiec();
    affichagec(c);
    est_dedans(c,p1);
    return 0;
}