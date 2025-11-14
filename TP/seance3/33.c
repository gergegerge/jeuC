#include <stdio.h>

int premier(int n){
    for(int i=2;i<n;i++){
        if(n%i==0){
            return 0;
        }
    }
    return 1;
}


int main(){
    int n;
    printf("nombre : ");
    scanf("%d",&n);
    if(premier(n)){
        printf("%d est premier!",n);
    }
    else{
        printf("%d n'est pas premier!",n);
    }
    return 0;
}