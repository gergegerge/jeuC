#include <stdio.h>
#include <string.h>

void afficher(char*s1,char*s2){
    int c1 = 0;
    int c2 = 0;
    for(int i = 0;s1[i]!='\0';i++){
        c1++;
    }
    for(int i = 0;s2[i]!='\0';i++){
        c2++;
    }
    printf("taille de s1 : %d\ntaille de s2 : %d\n",c1,c2);
}

void comparer(char*s1,char*s2){
    for(int i = 0;i<10;i++){
        if ((int)s1[i]<(int)s2[i]){
            printf("s1 est prems\n");
            return;
        }
        else if ((int)s1[i]>(int)s2[i]){
            printf("s2 est prems\n");
            return;
        }
    }
    printf("c'est les meme listes\n");
    
}

void new(char*s1,char*s2){
    int c1 = 0;
    int c2 = 0;
    for(int i = 0;s1[i]!='\0';i++){
        c1++;
    }
    for(int i = 0;s2[i]!='\0';i++){
        c2++;
    }
    char s3[c1+c2];
    strcpy ( s3 , s2 );
    strcat ( s3 , ":" );
    strcat ( s3 , s1 );
    printf("la nouvelle chaine est %s",s3);
}

int main(){
    char *s1,*s2;
    printf("entre deux chaines de characteres \n");
    scanf("%s %s",s1,s2);
    afficher(s1,s2);
    comparer(s1,s2);
    new(s1,s2);
    return 0;
}