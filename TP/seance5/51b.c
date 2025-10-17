#include <stdio.h>

int quot(int a,int b,int *pr){
    int q = a / b;
    *pr = a%b;
    return q;
}




int main(){
    int *pr;
    int a;
    printf("a = ");
    scanf("%d",&a);
    int b;
    printf("b = ");
    scanf("%d",&b);
    int q = quot(a,b,pr);
    printf ("quot = %d\n reste = %d\n",q,*pr);
    return 0;
}