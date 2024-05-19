#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liste.h"
#include "personne.h"

int menu () {
  printf ("\n\nGESTION D'UNE LISTE DE PERSONNES\n\n");
  printf ("0 - Fin\n");
  printf ("1 - Insertion   en tête de liste\n");
  printf ("2 - Insertion   en fin  de liste\n");
  printf ("3 - Retrait     en tête de liste\n");
  printf ("4 - Retrait     en fin  de liste\n");
  printf ("5 - Retrait     d'un élément à partir de son nom\n");
  printf ("6 - Parcours    de la liste\n");
  printf ("7 - Recherche   d'un élément à partir de son nom\n");
  printf ("8 - Destruction de la liste\n");
  printf ("\n");
  printf ("Votre choix ? ");
  int cod; scanf ("%d", &cod);
  printf ("\n");
  return cod;
}

int  main () {


  booleen fini = faux;
  Liste* lp = creerListe (0, ecrirePersonne, comparerPersonne);
  while (!fini) {
    switch (menu() ) {
    case 0:
      fini = vrai;
      break;
    case 1 : {
	  printf ("Nom de la personne à créer ? ");
	  ch15 nom; scanf ("%s", nom);
      printf ("Prénom de la personne à créer ? ");
      ch15 prenom; scanf ("%s", prenom);
      Personne* nouveau = creerPersonne(nom, prenom);

      insererEnTeteDeListe (lp, nouveau);
      } break;
    case 2 : {
	  printf ("Nom de la personne à créer ? ");
	  ch15 nom; scanf ("%s", nom);
      printf ("Prénom de la personne à créer ? ");
      ch15 prenom; scanf ("%s", prenom);
      Personne* nouveau = creerPersonne(nom, prenom);

      insererEnFinDeListe (lp, nouveau);
      } break;
    case 3 : {
      Personne* extrait = (Personne*) extraireEnTeteDeListe (lp);
      if (extrait != NULL) {
        printf ("Élément %s %s extrait en tête de liste",
                extrait->nom, extrait->prenom);
      } else {
        printf ("Liste vide");
      }
      } break;
    case 4 : {
      Personne* extrait = (Personne*) extraireEnFinDeListe (lp);
      if (extrait != NULL) {
        printf ("Élément %s %s extrait en fin de liste",
                extrait->nom, extrait->prenom);
      } else {
        printf ("Liste vide");
      }
      } break;
    case 5 : {
      printf ("Nom de la personne à extraire ? ");
      ch15 nom; scanf ("%s", nom);
      Personne* cherche = creerPersonne (nom, "?");
      Personne* pp      = (Personne*) chercherUnObjet (lp, cherche);
      if (extraireUnObjet (lp, pp)) {
        printf ("Élément %s %s extrait de la liste", pp->nom, pp->prenom);
      }
      } break;
    case 6:
      listerListe (lp);
      break;
    case 7 : {
      printf ("Nom de la personne recherchée ? ");
      ch15 nom; scanf ("%s", nom);
      Personne* cherche = creerPersonne (nom, "?");
      Personne* pp      = (Personne*) chercherUnObjet (lp, cherche);
      if (pp != NULL) {
        printf ("%s %s trouvée dans la liste\n", pp->nom, pp->prenom);
      } else {
        printf ("%s inconnue dans la liste\n", nom);
      }
      } break;
    case 8:
      detruireListe (lp);
      break;
    }
  }
    return 0;
}


