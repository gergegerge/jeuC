#include <stdio.h>

void echange(int *a, int *b){
    int p = *a;
    *a = *b;
    *b = p;
}

int main(){
    int a;
    printf("a = ");
    scanf("%d",&a);
    int b;
    printf("b = ");
    scanf("%d",&b);
    echange(&a,&b);
    printf("a = %d\n b = %d\n",a,b);
    return 0;
}