#include <stdio.h>
#include <string.h>

int main(){
    char *s;
    printf("le verbe à conjuguer est : ");
    scanf("%s",s);
    int len = strlen(s);
    if (!(s[len-2]=='e'&&s[len-1]=='r')){
        printf("ce verbe n'est pas du premier groupe !\n");
        return 0;
    }
    char s[len-2];

}
// pas fini mais très facile