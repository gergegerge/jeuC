#include <stdio.h>
#include <string.h>
#define TAILLE 10

int voyelle(char *s){
    int len=0;
    for(int i = 0;s[i]!='\0';i++){
        len++;
    }
    int compt = 0;
    for(int i =0;i<len;i++){
        if(s[i]=='a'||s[i]=='e'||s[i]=='i'||s[i]=='o'||s[i]=='u'||s[i]=='y'){
            compt++;
        }
    }
    return compt;
}

int occurences(char *s, char c){
    int len = strlen(s);
    int compt = 0;
    for(int i=0;i<len;i++){
        if(s[i]==c){
            compt++;
        }
    }
    return compt;
}

void remplacer(char *s, char c1, char c2){
    int len = strlen(s);
    for(int i=0;i<len;i++){
        if(s[i]==c1){
            s[i]=c2;
        }
    }
}

int main(){
    char s[TAILLE];
    printf("saisir la chaine\n");
    scanf("%s",s);
    int nbv = voyelle(s);
    printf("le nombre de voyelles est : %d\n",nbv);
    char ca;
    printf("calculer le nombre d'occurrence de : \n");
    scanf(" %c",&ca); 
    int occ = occurences(s,ca);
    printf("le charactere a apparait %d fois\n",occ);
    remplacer(s,ca, 'a');
    printf("%s",s);
    return 0;
}