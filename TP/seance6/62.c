#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define TAILLE 10

bool est_palindrome(char *s){
    int len=0;
    for(int i = 0;s[i]!='\0';i++){
        len++;
    }
    for(int i =0;i<len/2;i++){
        if (s[i]!=s[len - 1 - i]){
            return false;
        }
    }
    return true;
}

int main(){
    char s[TAILLE];
    printf("saisir la chaine\n");
    scanf("%s",s);
    bool v = est_palindrome(s);
    if (v){
        printf("palindrome!");
        return 0;
    }
    printf("non");
    return 0;
}