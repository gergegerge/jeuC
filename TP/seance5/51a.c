#include <stdio.h>

void init(int *n1,int *n2,float *f){
    *n1 = 0;
    *n2 = 0;
    *f = 0;
}

int main(){
    int a,b;
    float f;
    init(&a,&b,& f);
    printf("%d %d %f",a,b,f);
    return 0;
}