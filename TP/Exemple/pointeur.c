#include <stdio.h>

void incrementer(int *a) {
    *a = *a + 1;  // on modifie la valeur de l'entier pointé
}

int main() {
    //Pointeur vers un entier
    int n = 5;
    printf("La valeur de n avant : %d\n", n);
    incrementer(&n);  // on passe l'adresse de n
    printf("La valeur de n après : %d\n", n);
    
    //Tableaux 
    int tab[5] = {10, 20, 30, 40, 50};
    int *p = tab; 
    
    //Affichage de l'adresse de tab (début de ce tableau dans la mémoire)
    printf("Adresse de tab (&tab[0]) = %p\n", tab);
    printf("Adresse de tab[0]                = %p\n", &tab[0]);
    printf("Adresse de tab[0] pointée par p               = %p\n", p);
    printf("Valeur de tab[0]                 = %d\n", tab[0]);
    printf("Valeur pointee par p             = %d\n", *p);
    
    
    //Affichage des éléments du tableau 
    for (int i = 0; i < 5; i++) {
        printf("tab[%d] = %d, *(p+%d) = %d\n", i, tab[i], i, *(p+i));
    }
    return 0;
}