#include <stdio.h>

void tri_fusion(int tab[], int i, int j) {
    int n = j - i + 1;
    if (n > 1) {
        int v = n / 2;
        
        // Trier récursivement les deux moitiés
        tri_fusion(tab, i, i + v - 1);
        tri_fusion(tab, i + v, j);
        
        // Fusion
        int R[n];
        
        // Copier la première moitié normalement
        for (int p = 0; p < v; p++) {
            R[p] = tab[i + p];
        }
        
        // Copier la seconde moitié en ordre inverse
        for (int p = 0; p < n - v; p++) {
            R[v + p] = tab[j - p];
        }
        
        // Fusionner avec deux pointeurs aux extrémités
        int d = 0;
        int f = n - 1;
        for (int k = i; k <= j; k++) {
            if (R[d] <= R[f]) {
                tab[k] = R[d];
                d++;
            } else {
                tab[k] = R[f];
                f--;
            }
        }
    }
}

int main() {
    int t[10]; 
    printf("Entrez 10 valeurs :\n");
    for (int i = 0; i < 10; i++) {
        printf("t[%d] = ", i);
        scanf("%d", &t[i]);
    }
    
    tri_fusion(t, 0, 9);
    
    printf("\nTableau trié :\n");
    for (int i = 0; i < 10; i++) {
        printf("t[%d] = %d\n", i, t[i]);
    }
    
    return 0;
}