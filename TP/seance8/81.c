#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int nb_personne = 0;
typedef struct{
    char nom[50],prenom[50];
    long int numero;
}personne;

void ajout_personne(personne *p){
    char s1[50],s2[50];
    long int num;
    printf("saisir nom prenom numéro : \n");
    scanf(" %s %s %ld",s1,s2,&num);
    getchar();
    strcpy(p->nom,s1);
    strcpy(p->prenom,s2);
    p->numero=num;
    FILE *fichier = fopen("repertoire.bin","ab");
    /* int v =  */fwrite(p,sizeof(personne),1,fichier);
    fclose(fichier);
    nb_personne++;
}

void affichage(void){
    FILE *fichier = fopen("repertoire.bin","rb");
    for(int i = 0;i<nb_personne;i++){
        personne p;
        fseek ( fichier ,i*sizeof(personne), SEEK_SET );
        fread ( &p, sizeof(personne) , 1 , fichier ); 
        printf ( "nom : %s\nprenom : %s\nnumero : %ld\n" ,p.nom,p.prenom,p.numero);
    }
    fclose(fichier);
}


void obtenirNumero(void){
    char nom[20];
    FILE *fichier =fopen("repertoire.bin","rb");
    printf("nom : ");
    scanf("%s", nom);
    for(int i = 0;i<nb_personne;i++){
        personne p;
        fseek ( fichier ,i*sizeof(personne), SEEK_SET );
        fread(&p, sizeof(personne), 1, fichier);
        if(strcmp(p.nom, nom) == 0){
            printf("numero : %ld\n",p.numero);
        }
    }
}

void changer_numero(void){
    char nom[20];
    FILE *fichier =fopen("repertoire.bin","r+b");
    printf("nom : ");
    scanf("%s", nom);
    for(int i = 0;i<nb_personne;i++){
        personne p;
        fseek ( fichier ,i*sizeof(personne), SEEK_SET );
        fread(&p, sizeof(personne), 1, fichier);
        if(strcmp(p.nom, nom) == 0){
            printf("saisir nouveau numéro : \n");
            scanf("%ld",&p.numero);
            fwrite(&p, sizeof(personne), 1, fichier);
        }
    }
}// NE FONCTIONNE PAS TOUT A FAIT, A REVOIR

void menu(void){
    bool quitter = false;
    int choix;
    while (!quitter){
    printf("1 : ajout personne\n2 : obtenir numero\n3 : afficher repertoire\n4 : changer numero\n5 : quiter\n");
    scanf("%d",&choix);
    switch(choix){
        case 1:
        personne p;
        ajout_personne(&p);
        break;
        case 2:
        obtenirNumero();
        break;
        case 3:
        affichage();
        break;
        case 4:
        changer_numero();
        break;
        case 5:
        quitter = true;
        break;
    }
    }
}

int main(){
    FILE *fichier = fopen("repertoire.bin","wb");
    fclose(fichier);
    menu();
    return 0;
}