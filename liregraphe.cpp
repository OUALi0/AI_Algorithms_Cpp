
/* cr?er un graphe ? partir d'une description
   du graphe faite dans un fichier
   */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>




typedef char NomSom [20];
int c;   // un caract?re lu en avance dans lireUnMot

// ignorer les blancs
void lireBlancs (FILE* fe) {
  while ( ( (c==' ')  || (c=='\n') || (c==13) )  && !feof(fe) ) {
    c = getc(fe);
  }
}

// lire un nom de sommet en ignorant les espaces
void lireUnMot (FILE* fe, char* chaine) {
  char* pCh = chaine;
  //printf ("Debut lireUnMot %c %d\n", c, c);
  lireBlancs (fe); // blancs avant le mot
  while ( isalpha(c) || isdigit(c) ) {
    *pCh++ = (char) c;
    //printf ("-- %c %d\n", c, c);
    c = getc(fe);
  }
  *pCh = 0;
  lireBlancs (fe);  // blancs apr?s le mot
  //printf ("Fin lireUnMot %s\n", chaine);  getchar();
}

// fournir un pointeur sur un graphe construit
// ? partir d'un fichier fe de donn?es
// value = vrai si le Graphe est valu?
GrapheMat* lireGraphe (FILE* fe, int nMaxSom) {
  booleen value = faux;

  GrapheMat* graphe = creerGrapheMat (nMaxSom, faux);

  // lire les noms des sommets
  c = getc(fe);  // c global
  while (c != ';') {
    char* somD = (char*) malloc (20);
    lireUnMot (fe, somD);
    ajouterUnSommet (graphe, somD);
  }
  while (c != EOF) {
    c = getc(fe);
    NomSom somD;
    lireUnMot (fe, somD); // lit le sommet de d?part
    if (c != ':') {
      if (c != EOF) printf ("Manque : %c (%d)\n", c,c);
      graphe->value = value;
      return graphe;
    }
    c = getc(fe);
    while (c != ';') {
      NomSom somA;
      lireUnMot (fe, somA); // lit les sommets d'arriv?e
      int cout;
      if (c == '(') {
        value = vrai; // si sommet suivi de ( : S1(25)
        fscanf (fe, "%d", &cout);
        c = getc (fe); // passer )
        if (c != ')') printf ("Manque )\n");
        c = getc (fe);
        lireBlancs (fe); // prochain ? analyser
        //printf ("cout %d\n", cout);
      } else {
        cout = 0;
      }
      ajouterUnArc (graphe, somD, somA, cout);
    }
  }
  graphe->value = value;
  return graphe;
}

