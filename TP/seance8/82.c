//Directement pris la correction pas de nouvelle chose ici
#include <stdio.h>
#define NOM_FIC         "notes.txt"
#define NB_MAX_ELEVES   100

typedef struct
{
  char nom[20];
  float note;
} Eleve;

// lit le fichier et range les eleves dans le tableau en parametre
// retourne le nb d'eleves lus
int lireFichier(Eleve *eleves);

// analyse les notes du taleau d'eleves en parametre
void analyserNotes(Eleve *eleves, int nbEleves);

int lireFichier(Eleve *eleves)
{
  FILE *fic;
  int i = 0;

  fic = fopen(NOM_FIC, "r");

  while(fscanf(fic, "%s%f", eleves[i].nom, &eleves[i].note) == 2)
    i++;

  fclose(fic);

  return i;
}

void analyserNotes(Eleve *eleves, int nbEleves)
{
  float noteMax = eleves[0].note;
  float noteMin = eleves[0].note;
  float totalNotes = 0;
  int nbRattrapages = 0;
  int rattrapages[NB_MAX_ELEVES]; // liste des indices des eleves en rattrapage
  int i;
  int numEleveEnRattrapage;

  for (i = 0; i < nbEleves; i++)
  {
    float note = eleves[i].note;

    if(note > noteMax)
      noteMax = note;
    else if(note < noteMin)
      noteMin = note;

    totalNotes += note;

    if (note < 10)
    {
      rattrapages[nbRattrapages] = i;
      nbRattrapages++;
    }
  }

  printf("note min : %.1f, note max : %.1f, moyenne : %.1f\n", noteMax, noteMin,
            totalNotes/nbEleves);
  printf("nb rattrapages : %d\n", nbRattrapages);
  for (i = 0; i < nbRattrapages; i++)
  {
    numEleveEnRattrapage = rattrapages[i]; 
    printf("%s\n", eleves[numEleveEnRattrapage].nom);
  }
}

int main(void)
{
  Eleve eleves[NB_MAX_ELEVES];
  int nbEleves;

  nbEleves = lireFichier(eleves);
  analyserNotes(eleves, nbEleves);
  
  return 0;
}
