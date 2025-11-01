// idem que pour le 2
#include <stdio.h>
#define NOM_FIC   "doubles.dat"

void saisirFichier(void);
void afficherValeursEntre(double borneMin, double borneMax);
void afficherValAIndice(int indice);
void affcherAvantDerniereVal(void);

void saisirFichier(void)
{
  FILE *fic;
  int nbVal;
  int i;
  double val;

  fic = fopen(NOM_FIC, "w");

  printf("nb valeurs : ");
  scanf("%d", &nbVal);
  fwrite(&nbVal, sizeof(int), 1, fic);

  for (i = 0; i < nbVal; i++)
  {
    printf("valeur : ");
    scanf("%lf", &val);
    fwrite(&val, sizeof(double), 1, fic);
  }

  fclose(fic);
}

void afficherValeursEntre(double borneMin, double borneMax)
{
  FILE *fic;
  int nbVal;
  int i;
  double val;

  fic = fopen(NOM_FIC, "r");

  fread(&nbVal, sizeof(int), 1, fic);

  for (i = 0; i < nbVal; i++)
  {
    fread(&val, sizeof(double), 1, fic);
    if (borneMin <= val && val <= borneMax)
      printf("%lf\n", val);
  }

  fclose(fic);
}

void afficherValAIndice(int indice)
{
  FILE *fic;
  int nbVal;
  double val;

  fic = fopen(NOM_FIC, "r");

  fread(&nbVal, sizeof(int), 1, fic);
  if (indice >= nbVal)
    printf("indice trop grand\n");
  else
  {
    fseek(fic, indice * sizeof(double), SEEK_CUR);
    fread(&val, sizeof(double), 1, fic);
    printf("val = %lf\n", val);
  }

  fclose(fic);
}

void afficherAvantDerniereVal(void)
{
  FILE *fic;
  int nbVal;
  double val;

  fic = fopen(NOM_FIC, "r");

  fseek(fic, -(2 * sizeof(double)), SEEK_END);
  fread(&val, sizeof(double), 1, fic);
  printf("avant derniere = %lf\n", val);

  fclose(fic);
}

int main(void)
{
  double val1, val2;
  int i;

  saisirFichier();

  printf("bornes : ");
  scanf("%lf%lf", &val1, &val2);
  afficherValeursEntre(val1, val2);

  printf("indice : ");
  scanf("%d", &i);
  afficherValAIndice(i);

  afficherAvantDerniereVal();

  return 0;
}
