#include <limits.h>
typedef int booleen;
#define faux 0
#define vrai 1
typedef char NomSom[20]; // nom d'un sommet
#define INFINI INT_MAX

typedef int *Matrice;

typedef struct
{
  int n;          
  int nMax;        
  booleen value;   
  NomSom *nomS;    
  Matrice element; 
  Matrice valeur; 
  booleen *marque; 
} GrapheMat;

GrapheMat *creerGrapheMat(int nMax, int value);
void detruireGraphe(GrapheMat *graphe);
void ajouterUnSommet(GrapheMat *graphe, NomSom nom);
void ajouterUnArc(GrapheMat *graphe, NomSom somD, NomSom somA, int cout);
void ecrireGraphe(GrapheMat *graphe);

void parcoursProfond(GrapheMat *graphe);
void parcoursLargeur(GrapheMat *graphe);
void parcoursLargeurDabord(GrapheMat *graphe);

void parcoursCoutUniforme(GrapheMat* graphe);
void parcoursProfondDAbord(GrapheMat *graphe);
void parcoursProfendeurLimite(GrapheMat *graphe);
void parcoursProfondeurIteratif(GrapheMat *graphe);

void parcoursAEtoile(GrapheMat *graphe);
void IDAEtoile(GrapheMat *graphe);
void parcoursMeilleurDabord(GrapheMat* graphe);
void parcoursEscalade(GrapheMat *graphe);

void parcoursRecuitSimule(void *initial_solution, double temperature, int essais, double temperature_stop, double cooling_factor);

void Exploration_En_Recuit_Simule();
void Algorithme_Genitique();
void RN_monocoche();
void RN_Multicouche();




