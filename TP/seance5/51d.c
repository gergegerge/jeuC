#include <stdio.h>
#include <stdlib.h>

void interaction(float *adress){
    printf("le float vaudra : ");
    scanf("%f",adress);
}

int main(){
    float *adress = (float*)malloc(sizeof(float));
    interaction(adress);
    return 0;
}