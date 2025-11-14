#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void copier(char*s){
    int len = strlen(s);
    char *scpy = (char*)malloc(sizeof(char)*len);
    for (int i = 0;i<len;i++){
        scpy[i] = s[i];
    }
    for (int i = 0;i<len;i++){
        printf("s copy[%d] = %c\n",i,scpy[i]);
    }
    free(scpy);
}

void concatenate(char *s1,char *s2){
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    char *sconc = (char*)malloc(sizeof(char)*(l1+l2));
    for (int i = 0;i<l1;i++){
        sconc[i] = s1[i];
    }
    for (int i = l1;i<l1+l2;i++){
        sconc[i] = s2[i-l1];
    }
    for (int i = 0;i<l1+l2;i++){
        printf("s copy[%d] = %c\n",i,sconc[i]);
    }
    free(sconc);
}

void extract(char *s,int d,int f){
    int len = f - d + 1;
    char *sextr = (char*)malloc(sizeof(char)*len);
    for (int i = d;i<len + d;i++){
        sextr[i-d]=s[i];
        printf("sextr[%d] = %c",i-d,sextr[i-d]);
    }
    free(sextr);
}

int main(){
    char *s1;
    printf("le mot à copier est : ");
    scanf("%s",s1);
    copier(s1);
    char *s2;
    printf("le mot à concat est : ");
    scanf("%s",s2);
    concatenate(s1,s2);
    int d;
    printf("le debut est : ");
    scanf("%d",&d);
    int f;
    printf("la fin est : ");
    scanf("%d",&f);
    extract(s1,d,f);
    return 0;
}
