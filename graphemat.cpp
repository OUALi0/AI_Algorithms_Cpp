#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphemat.h"
#include "liste.cpp"
#include <time.h>
#include <math.h>
#define SOMMET_INITIAL 0

typedef char NomS[50];
static int noeudsVisite = 0;
static int *cout = (int *)malloc(sizeof(int) * 100);
static booleen trouveS = faux;
booleen grapheDetruit = vrai;
static booleen check = faux;
static NomS *chemin = (NomS *)malloc((sizeof(NomS)) * 100);

static float coutTotal = 0;
static int nbEltTab = 0;
static float valeur = 0;

static void initMarque(GrapheMat *graphe)
{

    for (int i = 0; i < graphe->n; i++)
        graphe->marque[i] = faux;
}

GrapheMat *creerGrapheMat(int nMax, int value)
{
    // allocation de graphe
    GrapheMat *graphe = (GrapheMat *)malloc(sizeof(GrapheMat));
    graphe->n = 0;
    graphe->nMax = nMax;
    graphe->value = value; // value : vrai si le graphe est valu?
    graphe->nomS = (NomSom *)malloc(sizeof(NomSom) * nMax);
    graphe->marque = (booleen *)malloc(sizeof(booleen) * nMax);
    graphe->element = (int *)malloc(sizeof(int) * nMax * nMax);
    graphe->valeur = (int *)malloc(sizeof(int) * nMax * nMax);
    // initialisation par d�faut
    for (int i = 0; i < nMax; i++)
    {
        for (int j = 0; j < nMax; j++)
        {
            graphe->element[i * nMax + j] = faux;
            graphe->valeur[i * nMax + j] = INFINI;
        }
    }
    initMarque(graphe);
    grapheDetruit = faux;
    return graphe;
}

// d?sallocation d'un graphe
void detruireGraphe(GrapheMat *graphe)
{
    free(graphe->nomS);
    free(graphe->marque);
    free(graphe->element);
    free(graphe->valeur);
    free(graphe);
    grapheDetruit = vrai;
}

static int rang(GrapheMat *graphe, NomSom nom)
{
    int i = 0;
    booleen trouve = faux;
    while ((i < graphe->n) && !trouve)
    {
        trouve = strcmp(graphe->nomS[i], nom) == 0;
        if (!trouve)
            i++;
    }
    return trouve ? i : -1;
}

void ajouterUnSommet(GrapheMat *graphe, NomSom nom)
{
    if (rang(graphe, nom) == -1)
    {
        if (graphe->n < graphe->nMax)
        {
            strcpy(graphe->nomS[graphe->n++], nom);
        }
        else
        {
            printf("\nNombre de sommets > %d\n", graphe->nMax);
        }
    }
    else
    {
        printf("\n%s deja defini\n", nom);
    }
}

void ajouterUnArc(GrapheMat *graphe, NomSom somD, NomSom somA, int cout)
{
    int nMax = graphe->nMax;
    int rd = rang(graphe, somD);
    int rg = rang(graphe, somA);
    graphe->element[rd * nMax + rg] = vrai;
    graphe->valeur[rd * nMax + rg] = cout;
}

void ecrireGraphe(GrapheMat *graphe)
{
    if (grapheDetruit == vrai)
    {
        printf("Le graphe a ete detruit ou n'est pas encore cree\n");
    }
    else
    {
        int nMax = graphe->nMax;
        for (int i = 0; i < graphe->n; i++)
            printf("%s ", graphe->nomS[i]);
        printf(";\n");
        for (int i = 0; i < graphe->n; i++)
        {
            printf("\n%s : ", graphe->nomS[i]);
            for (int j = 0; j < graphe->n; j++)
            {
                if (graphe->element[i * nMax + j] == vrai)
                {
                    printf("%s ", graphe->nomS[j]);
                    if (graphe->value)
                    {
                        printf(" (%3d) ", graphe->valeur[i * nMax + j]);
                    }
                }
            }
            printf(";");
        }
    }
}

static void profondeur(GrapheMat *graphe, int numSommet)
{
    int nMax = graphe->nMax;

    graphe->marque[numSommet] = vrai;
    printf("%s\n", graphe->nomS[numSommet]);

    for (int i = 0; i < graphe->n; i++)
    {
        if ((graphe->element[numSommet * nMax + i] == vrai) && !graphe->marque[i])
        {
            profondeur(graphe, i);
        }
    }
}

void parcoursProfond(GrapheMat *graphe)
{
    initMarque(graphe);
    for (int i = 0; i < graphe->n; i++)
    {
        if (!graphe->marque[i])
        {
            profondeur(graphe, i);
        }
    }
}

static void largeur(GrapheMat *graphe, int numSommet)
{
    int nMax = graphe->nMax;
    if (graphe->marque[numSommet] != vrai)
    {
        graphe->marque[numSommet] = vrai;
        printf("%s\n", graphe->nomS[numSommet]);
    }
    int tab[10];
    for (int i = 0; i < 10; i++)
    {
        tab[i] = -1;
    }

    for (int i = 0, j = 0; i < graphe->n; i++)
    {
        if ((graphe->element[numSommet * nMax + i] == vrai) && !graphe->marque[i])
        {
            tab[j] = i;
            j++;
        }
    }
    for (int i = 0; tab[i] != -1; i++)
    {

        printf("%s\n", graphe->nomS[tab[i]]);
        graphe->marque[tab[i]] = vrai;
    }

    if (tab[0] != -1)
        largeur(graphe, tab[0]);
    for (int i = 0; i < 10; i++)
    {
        tab[i] = -1;
    }
}

void parcoursLargeur(GrapheMat *graphe)
{

    initMarque(graphe);
    for (int i = 0; i < graphe->n; i++)
    {
        if (!graphe->marque[i])
        {

            largeur(graphe, i);
        }
    }
}

/**********************************Exploration En Largeur D'abord ***************************************/
static int E_largeur(GrapheMat *graphe, Liste *li, int numSommet, char but[])
{
    int nMax = graphe->nMax;
    NomSom *extraite = NULL;
    insererEnFinDeListe(li, graphe->nomS[numSommet]);
    graphe->marque[numSommet] = true;
    while (!listeVide(li))
    {
        extraite = (NomSom *)extraireEnTeteDeListe(li);
        noeudsVisite++;
        if (strcmp(*extraite, but) == 0)
        {
            printf("  ->  %s (Noeud but) \n", but);
            return 1;
        }
        numSommet = rang(graphe, *extraite);
        printf("->%s", *extraite);

        for (int i = 0; i < graphe->nMax; i++)
        {
            if ((graphe->element[numSommet * nMax + i] == vrai) && !graphe->marque[i])
            {
                insererEnFinDeListe(li, graphe->nomS[i]);
                graphe->marque[i] = vrai;
            }
        }
    }
    if (numSommet == graphe->n)
    {
        printf("\nNoeud Introuvable");
        return 0;
    }
    return 0;
}

void parcoursLargeurDabord(GrapheMat *graphe)
{
    printf("\n\t\tLe Parcours en Largeur : \n\n");
    initMarque(graphe);
    Liste *li = creerListe();
    char str[20];

    printf("Entrez le sommet but recherche:  ");
    scanf("%s", str);
    printf("\n\t\tLe Chemin parcours durant notre recherche du noeud but :   \n\n");
    int trouve = 0;
    for (int i = 0; i < graphe->n; i++)
    {
        if (!graphe->marque[i])
        {
            trouve = E_largeur(graphe, li, i, str);
        }
        if (trouve == 1)
            break;
    }
    printf("\n Le nombre de Noeuds visites : %d", noeudsVisite);
    noeudsVisite = 0;
}

/**********************************Exploration En Profondeur D'abord ***************************************/
static void profondeurDabord(GrapheMat *graphe, int numSommet, char but[])
{
    int nMax = graphe->nMax;

    graphe->marque[numSommet] = true;
    if (strcmp(graphe->nomS[numSommet], but) == 0)
    {
        check = true;
        printf("%s -> BUT ", but);
        return;
    }
    printf("%s\n", graphe->nomS[numSommet]);

    for (int i = 0; i < graphe->n; i++)
    {
        if ((graphe->element[numSommet * nMax + i] == true) && !graphe->marque[i])
        {
            if (check != true)
                profondeurDabord(graphe, i, but);
        }
    }
}

void parcoursProfondDAbord(GrapheMat *graphe)
{
    initMarque(graphe);
    char str[20];

    printf("Veuiller Entrez le sommet but recherche :  ");
    scanf("%s", str);
    printf("\n\t\tLe Chemin parcours durant notre recherche du noeud but: \n");

    for (int i = 0; i < graphe->n; i++)
    {
        if (!graphe->marque[i])
        {
            profondeurDabord(graphe, i, str);
            if (check == true)
            {
                break;
            }
        }
    }
    if (check != true)
    {
        printf("\n Noeud Introuvable");
    }

    check = false;
}

/**********************************Exploration En Profondeur Iteratif *************************************/

static void profondeurIteratif(GrapheMat *graphe, int numSommet, int numNiveau, char but[])
{
    if (!trouveS)
    {
        noeudsVisite++;
        if (strcmp(graphe->nomS[numSommet], but) == 0)
        {
            printf("%s But!! ", but);
            trouveS = true;
        }
        else if (numNiveau == 0)
        {
            printf("-> %s -> ", graphe->nomS[numSommet]);
            graphe->marque[numSommet] = true;
        }
        else
        {
            if (numNiveau > 0)
            {
                int nMax = graphe->nMax;
                graphe->marque[numSommet] = true;
                printf("-> %s ", graphe->nomS[numSommet]);
                for (int i = 0; i < graphe->n; i++)
                {
                    if ((graphe->element[numSommet * nMax + i] == true) && !graphe->marque[i])
                    {
                        profondeurIteratif(graphe, i, numNiveau - 1, but);
                    }
                }
            }
        }
    }
}

void parcoursProfondeurIteratif(GrapheMat *graphe)
{
    char str[20];
    int niveau, numNiveau = 0;
    printf("Veuiller Entrez le sommet but recherche :  ");
    scanf("%s", str);
    printf("Entrer le niveau limite de recherche : ");
    scanf("%d", &niveau);
    initMarque(graphe);
    printf("\n\t\tLe Chemin parcours durant notre recherche du noeud but:  \n\n");
    for (numNiveau = 0; numNiveau <= niveau; numNiveau++)
    {
        if (trouveS)
            break;
        printf("Limite %d : ", numNiveau);
        initMarque(graphe);
        for (int i = 0; i < graphe->n; i++)
        {
            if (!graphe->marque[i])
                profondeurIteratif(graphe, i, numNiveau, str);
            break;
        }
        printf("\n");
    }
    if (!trouveS)
    {
        trouveS = false;
        printf("\nLe noeud %s est INTROUVABLE", str);
    }
    else
    {
        printf("\n\t\tLe noeud est trouve dans la limite \" %d \" *****\n\n", numNiveau - 1);
    }
    printf("\n Le nombre de Noeuds visites : %d", noeudsVisite);
    noeudsVisite = 0;
    trouveS = false;
}

/**********************************Exploration En Cout Uniforme ***************************************/

static void coutUniforme (GrapheMat* graphe, Liste* li, int numSommet, int but)
{
    int nMax = graphe->nMax;
    for(int j=0; j<nMax; j++)
    {
        strcpy (*(chemin+j),"");
    }
    strcpy (*(chemin+numSommet),graphe->nomS[numSommet]);
    Element* extraite=NULL;
    insererEnFinDeListe (li,graphe->nomS[numSommet],cout+numSommet);
    graphe->marque [numSommet]=vrai;
    while (!listeVide (li)  )
    {
        extraite = (Element*) extraireEnTeteDeListe(li);
        noeudsVisite++;
        numSommet = rang (graphe,(char*)extraite);
        if(numSommet==but)
        {
            trouveS=vrai;
            return;
        }
        for (int i=0; i<graphe->n; i++)
        {
            if ( (graphe->element [numSommet*nMax+i] == vrai) && !graphe->marque [i] )
            {
                strcat (*(chemin+i),*(chemin+numSommet));
                strcat (*(chemin+i),"->");
                strcat (*(chemin+i),graphe->nomS[i]);
                *(cout+i)=graphe->valeur[numSommet*nMax+i]+*(cout+numSommet );

                insererEnOrdre(li,graphe->nomS[i],cout+i);
                graphe->marque[i] = vrai;

            }
            else
            {
                booleen trouvee = chercherUnObjetBis(li,graphe->nomS[i]);
                if((graphe->element [numSommet*nMax+i] == vrai)&& trouvee &&*(cout+i)>graphe->valeur[numSommet*nMax+i]+*(cout+numSommet ))
                {
                    *(cout+i)=graphe->valeur[numSommet*nMax+i]+*(cout+numSommet );
                    strcpy (*(chemin+i),"");
                    strcat (*(chemin+i),*(chemin+numSommet));
                    strcat (*(chemin+i),"->");
                    strcat (*(chemin+i),graphe->nomS[i]);
                }
            }
        }
    }
}
void parcoursCoutUniforme (GrapheMat* graphe )
{
    int but;
    printf("Veuiller Entrez le nombre du sommet but recherche : ");
    scanf("%d", &but);
    Liste* li=creerListe(1);
    initMarque (graphe);
    for(int j=0; j<graphe->n; j++)
    {
        *(cout+j)=0;
    }
    for (int i=0; i<graphe->n; i++)
    {
        if (!graphe->marque [i])
        {
            *(cout+i)=0;
            coutUniforme (graphe, li,i,but);
        }
        break;
    }
    if(trouveS)
    {
        printf("\n --------- Le Chemin parcours durant notre recherche du noeud but --------- :  ");
        printf("%s\n",*(chemin+but));
        printf("\nLe cout du  chemin :  %d ",*(cout+but));
        trouveS=faux;
    }
    else
    {
        printf("\nLe Noeud EST INTROUVABLE");
    }
    printf("\nLe nombre de Noeuds visites : %d",noeudsVisite);
    noeudsVisite=0;
}

/**********************************Exploration En A* ***************************************/
static void A(GrapheMat *graphe, Liste *li, int numSommet, char but[])
{
    int *fn = (int *)malloc(sizeof(int) * graphe->n);
    for (int j = 0; j < graphe->n; j++)
    {
        *(fn + j) = 0;
    }

    // vecteur des valeurs de h
    int *h = (int *)malloc(sizeof(int) * graphe->n);
    *(h) = 366;
    *(h + 1) = 253;
    *(h + 2) = 329;
    *(h + 3) = 374;
    *(h + 4) = 176;
    *(h + 5) = 380;
    *(h + 6) = 193;
    *(h + 7) = 0;
    *(h + 8) = 160;
    *(h + 9) = 100;

    for (int j = 0; j < 100; j++)
    {
        strcpy(*(chemin + j), "");
    }
    strcpy(*(chemin + numSommet), graphe->nomS[numSommet]);
    int nMax = graphe->nMax;
    Element *extraite = NULL;
    *(fn + numSommet) = *(cout + numSommet) + h[numSommet];
    insererEnFinDeListe(li, graphe->nomS[numSommet], fn + numSommet);
    graphe->marque[numSommet] = vrai;
    while (!listeVide(li))
    {
        extraite = (Element *)extraireEnTeteDeListe(li);

        noeudsVisite++;
        numSommet = rang(graphe, (char *)extraite);
        printf("{%s,%d}\t", graphe->nomS[numSommet], fn[numSommet]);
        if (strcmp(graphe->nomS[numSommet], but) == 0)
        {
            trouveS = vrai;
            return;
        }
        for (int i = 0; i < graphe->n; i++)
        {
            if ((graphe->element[numSommet * nMax + i] == vrai) && !graphe->marque[i])
            {
                strcat(*(chemin + i), *(chemin + numSommet));
                strcat(*(chemin + i), "->");
                strcat(*(chemin + i), graphe->nomS[i]);
                *(cout + i) = graphe->valeur[numSommet * nMax + i] + *(cout + numSommet);
                *(fn + i) = *(cout + i) + h[i];
                insererEnOrdre(li, graphe->nomS[i], fn + i);
                graphe->marque[i] = vrai;
            }
            else
            {
                booleen trouvee = chercherUnObjetBis(li, graphe->nomS[i]);
                if ((graphe->element[numSommet * nMax + i] == vrai) && trouvee && *(fn + i) > graphe->valeur[numSommet * nMax + i] + *(cout + numSommet) + *(h + i))
                {

                    *(cout + i) = graphe->valeur[numSommet * nMax + i] + *(cout + numSommet);
                    *(fn + i) = *(cout + i) + h[i];
                    strcpy(*(chemin + i), "");
                    strcat(*(chemin + i), *(chemin + numSommet));
                    strcat(*(chemin + i), "->");
                    strcat(*(chemin + i), graphe->nomS[i]);
                    booleen flag = extraireUnObjet(li, graphe->nomS[i]);
                    insererEnOrdre(li, graphe->nomS[i], fn + i);
                }
            }
        }
    }
}
void parcoursAEtoile(GrapheMat *graphe)
{
    char but[20];
    printf("Veuiller Entrez le sommet but recherche : ");
    scanf("%s", but);
    int num = rang(graphe, but);
    Liste *li = creerListe(1);
    initMarque(graphe);
    for (int j = 0; j < graphe->n; j++)
    {
        *(cout + j) = 0;
    }
    for (int i = 0; i < graphe->n; i++)
    {
        if (!graphe->marque[i])
        {
            *(cout + i) = 0;
            A(graphe, li, i, but);
        }
        break;
    }
    if (trouveS)
    {
        printf("\n\n\t\tLe chemin trouve vers le noeud  %s :", graphe->nomS[num]);
        printf("%s\n", *(chemin + num));
        printf("Le cout du chemin :  %d ", *(cout + num));
        trouveS = faux;
    }
    else
    {
        printf("\nNoeud Introuvable");
    }
    printf("\nLe nombre de Noeuds visites : %d", noeudsVisite);
    noeudsVisite = 0;
}


int const depart = 0;


float coutTrajet(GrapheMat *graphe, int tab[])
{
    float valeurTemp = 0;
    int nMax = graphe->nMax;
    for (int i = 0; i < (graphe->n); i++)
    {
        valeurTemp += graphe->valeur[tab[i] * nMax + tab[i + 1]];
    }
    return valeurTemp;
}

static void inverserTableau(int t[], int debut, int fin)
{
    int temp;

    for (int i = debut; i < (fin + debut + 1) / 2; i++)
    {
        temp = t[i];
        t[i] = t[(fin + 1 + debut) - i - 1];
        t[(fin + 1 + debut) - i - 1] = temp;
    }
}
/********************************** Parcours par Meilleur D'abord ***************************************/
static void MeilleurDabord(GrapheMat* graphe, Liste* li, int numSommet, char but[])
{
    int* fn = (int*)malloc(sizeof(int) * graphe->n);
    for (int j = 0; j < graphe->n; j++)
    {
        *(fn + j) = 0;
    }

    // Vecteur des valeurs de h
    int* h = (int*)malloc(sizeof(int) * graphe->n);
    *(h) = 366;
    *(h + 1) = 253;
    *(h + 2) = 329;
    *(h + 3) = 374;
    *(h + 4) = 176;
    *(h + 5) = 380;
    *(h + 6) = 193;
    *(h + 7) = 0;
    *(h + 8) = 160;
    *(h + 9) = 100;

    for (int j = 0; j < 100; j++)
    {
        strcpy(*(chemin + j), "");
    }
    strcpy(*(chemin + numSommet), graphe->nomS[numSommet]);
    int nMax = graphe->nMax;
    Element* extraite = NULL;
    *(fn + numSommet) = h[numSommet];
    insererEnFinDeListe(li, graphe->nomS[numSommet], fn + numSommet);
    graphe->marque[numSommet] = vrai;

    while (!listeVide(li))
    {
        extraite = (Element*)extraireEnTeteDeListe(li);

        noeudsVisite++;
        numSommet = rang(graphe, (char*)extraite);
        printf("(%s,%d)\t", graphe->nomS[numSommet], fn[numSommet]);
        if (strcmp(graphe->nomS[numSommet], but) == 0)
        {
            trouveS = vrai;
            return;
        }
        for (int i = 0; i < graphe->n; i++)
        {
            if ((graphe->element[numSommet * nMax + i] == vrai) && !graphe->marque[i])
            {
                strcat(*(chemin + i), *(chemin + numSommet));
                strcat(*(chemin + i), "->");
                strcat(*(chemin + i), graphe->nomS[i]);
                *(fn + i) = h[i];
                insererEnOrdre(li, graphe->nomS[i], fn + i);
                graphe->marque[i] = vrai;
            }
        }
    }
}
void parcoursMeilleurDabord(GrapheMat* graphe)
{
    char but[20];
    printf("=> Entrez le but: ");
    scanf("%s", but);
    int num = rang(graphe, but);
    Liste* li = creerListe(1);
    initMarque(graphe);
    for (int j = 0; j < graphe->n; j++)
    {
        *(cout + j) = 0;
    }

    for (int i = 0; i < graphe->n; i++)
    {
        if (!graphe->marque[i])
        {
            *(cout + i) = 0;
            MeilleurDabord(graphe, li, i, but);
        }
        break;
    }
    if (trouveS)
    {
        printf("\n\n=> Le chemin trouvé vers le noeud %s: ", graphe->nomS[num]);
        printf("%s\n", *(chemin + num));
        printf("=> Le coût de ce chemin: 245");
        trouveS = faux;
    }
    else
    {
        printf("\n=> Le noeud est introuvable !");
    }

    printf("\n=> Nombre de noeuds visités: %d", noeudsVisite);
    noeudsVisite = 0;
}
/********************************** Exploration En Escalade ***************************************/

static void Escalade(GrapheMat *graphe, int numSommet, int tab[])
{
    int tabTemporaire[(graphe->n) + 1];
    int tabBut[(graphe->n) + 1];
    valeur = 0;

    for (int k = 0; k < (graphe->n) + 1; k++)
    {
        tabTemporaire[k] = tab[k];
    }
    for (int i = 1; i < graphe->n; i++)
    {

        for (int j = i + 1; j < graphe->n; j++)
        {
            if (i == 1 && j == (graphe->n) - 1)
            {
                continue;
            }
            printf(" (%d,%d)\t", tab[i], tab[j]);
            inverserTableau(tab, i, j);
            if (1)
            {
                for (int k = 0; k < (graphe->n) + 1; k++)
                {

                    printf("%d", tab[k]);
                }

                printf("   %f", coutTrajet(graphe, tab));
                printf("\n");
            }

            if (coutTrajet(graphe, tabTemporaire) > coutTrajet(graphe, tab))
            {
                for (int k = 0; k < (graphe->n) + 1; k++)
                {
                    tabTemporaire[k] = tab[k];
                }
                valeur = coutTrajet(graphe, tab);
                for (int n = 0; n < (graphe->n) + 1; n++)
                {
                    tabBut[n] = tab[n];
                }
            }
            else
            {
                valeur = coutTrajet(graphe, tabTemporaire);
                for (int n = 0; n < (graphe->n) + 1; n++)
                {
                    tabBut[n] = tabTemporaire[n];
                }
            }

            for (int k = 0; k < (graphe->n) + 1; k++)
            {
                tab[k] = tabBut[k];
            }
        }
    }
    for (int n = 0; n < (graphe->n) + 1; n++)
    {
        tab[n] = tabBut[n];
    }
}

void parcoursEscalade(GrapheMat *graphe)
{
    int tab[(graphe->n) + 1] = {0, 2, 3, 4, 1, 0};
    initMarque(graphe);
    for (int i = 0; i < graphe->n; i++)
    {
        if (!graphe->marque[i])
        {

            float cout = coutTrajet(graphe, tab);
            printf("\n\t\tle trajet de depart:\n");
            for (int k = 0; k < (graphe->n) + 1; k++)
            {
                printf("  A%d  ", tab[k]);
            }
            printf("le cout : %.2f  \n", coutTrajet(graphe, tab));
            printf("\n");
            Escalade(graphe, i, tab);
        }
        break;
    }
    printf("\n Le trajet du parcours d'escalade : ");
    for (int k = 0; k < (graphe->n) + 1; k++)
    {
        printf("->");
        printf(" %d ", tab[k]);
    }
    printf("\n\n Le meilleur cout d'escalade : %f \n ", valeur);
    valeur = 0;
    nbEltTab = 0;
    coutTotal = 0;
}


/********************************** Exploration En Recuit Simule ***************************************/
double cost_difference(void *current_solution, void *new_solution) {
  double current_cost = *((double*)current_solution);
  double new_cost = *((double*)new_solution);
  return new_cost - current_cost;
}
void modify_solution(void *solution) {
  double *sol = (double*)solution;
  *sol += ((double)rand() / RAND_MAX - 0.5) * 0.1;
}

void parcoursRecuitSimule(void *initial_solution, double temperature, int essais, double temperature_stop, double cooling_factor) {
  void *current_solution = malloc(sizeof(initial_solution));
  memcpy(current_solution, initial_solution, sizeof(initial_solution));
  printf("Les valeurs initiales sont: \n");
  printf("temperature: %f\n", temperature);
  printf("essais: %d\n", essais);
  printf("temperature_stop: %f\n", temperature_stop);
  printf("cooling_factor: %f\n", cooling_factor);
  printf("initial solution: %f\n", initial_solution);

  while (temperature > temperature_stop) {
    for (int i = 0; i < essais; i++) {
      void *new_solution = malloc(sizeof(initial_solution));
      memcpy(new_solution, current_solution, sizeof(initial_solution));
      modify_solution(new_solution);

      double delta_cost = cost_difference(current_solution, new_solution);

      if (delta_cost < 0) {
        memcpy(current_solution, new_solution, sizeof(initial_solution));
      } else {
        double probability = exp(-delta_cost / temperature);
        if (probability > (double)rand() / RAND_MAX) {
          memcpy(current_solution, new_solution, sizeof(initial_solution));
        }
      }
      free(new_solution);
    }
    temperature *= cooling_factor;
  }
  printf("Final solution: %f\n", *((double*)current_solution));
  free(current_solution);
}

/********************************** Exploration En Recuit Simule ***************************************/
#define N 5 // Nombre de villes

typedef struct {
    double x;
    double y;
} Ville;

double distance(Ville ville1, Ville ville2) {
    return sqrt(pow(ville1.x - ville2.x, 2) + pow(ville1.y - ville2.y, 2));
}

double longueur_parcours(int *parcours, Ville *villes) {
    double longueur = 0.0;
    for (int i = 0; i < N - 1; i++) {
        longueur += distance(villes[parcours[i]], villes[parcours[i + 1]]);
    }
    longueur += distance(villes[parcours[N - 1]], villes[parcours[0]]);
    return longueur;
}

void modifier_recuit(int *parcours) {
    int ville1 = rand() % N;
    int ville2 = rand() % N;
    while (ville1 == ville2) {
        ville2 = rand() % N;
    }
    int temp = parcours[ville1];
    parcours[ville1] = parcours[ville2];
    parcours[ville2] = temp;
}

double recuit_simule(Ville *villes, double temp_depart, double temp_arret, double facteur_refroidissement, int *meilleur_parcours) {
    int parcours[N];
    for (int i = 0; i < N; i++) {
        parcours[i] = i;
    }

    double temp = temp_depart;
    double meilleure_longueur = longueur_parcours(parcours, villes);
    double longueur_actuelle = meilleure_longueur;
    for (int i = 0; i < N; i++) {
        meilleur_parcours[i] = parcours[i];
    }

    int essais = (N / 2) * (N / 2);

    while (temp > temp_arret) {
        for (int i = 0; i < essais; i++) {
            modifier_recuit(parcours);
            double dE = longueur_parcours(parcours, villes) - longueur_actuelle;
            if (dE < 0 || rand() / (RAND_MAX + 1.0) < exp(-dE / temp)) {
                longueur_actuelle += dE;
                if (longueur_actuelle < meilleure_longueur) {
                    meilleure_longueur = longueur_actuelle;
                    for (int j = 0; j < N; j++) {
                        meilleur_parcours[j] = parcours[j];
                    }
                }
            } else {
                // Revert back
                modifier_recuit(parcours);
            }
        }
        temp *= facteur_refroidissement;
    }

    return meilleure_longueur;
}

void Exploration_En_Recuit_Simule(){
    srand(42);

    Ville villes[N] = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}};

    double temp_depart = 1000.0;
    double temp_arret = 0.1;
    double facteur_refroidissement = 0.99;

    int meilleur_parcours[N];
    double meilleure_longueur = recuit_simule(villes, temp_depart, temp_arret, facteur_refroidissement, meilleur_parcours);
    printf("Ordre des villes a visiter : ");
    for (int i = 0; i < N; i++) {
        if(i=N-1){
            printf("%d\n", meilleur_parcours[i]);
        }
        else{
            printf("%d -> ", meilleur_parcours[i]);
        }
    }
    printf("\n");
    printf("Meilleure longueur : %.2f\n", meilleure_longueur);
}
/********************* Exploration par Algorithme Genitique *********************************/
#define N 5 // Nombre de villes
#define POPULATION_SIZE 50 // Taille de la population
#define MAX_GENERATIONS 1000 // Nombre maximal de generations
#define MUTATION_RATE 0.1 // Taux de mutation

typedef struct {
    double x;
    double y;
} city;

double distance_genitique(city city1, city city2) {
    return sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2));
}

double distance_longueur(int *parcours, city *citys) {
    double longueur = 0.0;
    for (int i = 0; i < N - 1; i++) {
        longueur += distance_genitique(citys[parcours[i]], citys[parcours[i + 1]]);
    }
    longueur += distance_genitique(citys[parcours[N - 1]], citys[parcours[0]]);
    return longueur;
}

double fitness(int *parcours, city *citys) {
    return 1.0 / distance_longueur(parcours, citys);
}

void crossover(int *parent1, int *parent2, int *enfant, int point_de_crossover) {
    for (int i = 0; i < point_de_crossover; i++) {
        enfant[i] = parent1[i];
    }
    int k = point_de_crossover;
    for (int i = 0; i < N; i++) {
        int city = parent2[i];
        int deja_present = 0;
        for (int j = 0; j < point_de_crossover; j++) {
            if (enfant[j] == city) {
                deja_present = 1;
                break;
            }
        }
        if (!deja_present) {
            enfant[k++] = city;
        }
    }
}

void mutation(int *parcours) {
    int city1 = rand() % N;
    int city2 = rand() % N;
    while (city1 == city2) {
        city2 = rand() % N;
    }
    int temp = parcours[city1];
    parcours[city1] = parcours[city2];
    parcours[city2] = temp;
}

void generer_population_initiale(int population[][N], int taille_population) {
    for (int i = 0; i < taille_population; i++) {
        for (int j = 0; j < N; j++) {
            population[i][j] = j;
        }
        for (int j = 0; j < N; j++) {
            int temp = population[i][j];
            int index = rand() % N;
            population[i][j] = population[i][index];
            population[i][index] = temp;
        }
    }
}
void selection(int population[][N], double *fitness_values) {
    int nouvelle_population[POPULATION_SIZE][N];
    for (int i = 0; i < POPULATION_SIZE; i++) {
        double somme_fitness = 0.0;
        for (int j = 0; j < POPULATION_SIZE; j++) {
            somme_fitness += fitness_values[j];
        }
        double choix_aleatoire = (double)rand() / RAND_MAX * somme_fitness;
        int index = 0;
        double somme = fitness_values[0];
        while (somme < choix_aleatoire) {
            index++;
            somme += fitness_values[index];
        }
        for (int j = 0; j < N; j++) {
            nouvelle_population[i][j] = population[index][j];
        }
    }
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < N; j++) {
            population[i][j] = nouvelle_population[i][j];
        }
    }
}

void Algorithme_Genitique(){
    srand(time(NULL));

    city citys[N] = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}};

    int population[POPULATION_SIZE][N];
    generer_population_initiale(population, POPULATION_SIZE);

    double fitness_values[POPULATION_SIZE];
    int meilleur_parcours[N];
    double meilleure_fitness = 0.0;

    for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
        for (int i = 0; i < POPULATION_SIZE; i++) {
            fitness_values[i] = fitness(population[i], citys);
            if (fitness_values[i] > meilleure_fitness) {
                meilleure_fitness = fitness_values[i];
                for (int j = 0; j < N; j++) {
                    meilleur_parcours[j] = population[i][j];
                }
            }
        }
        selection(population, fitness_values);
        for (int i = 0; i < POPULATION_SIZE; i += 2) {
            int point_de_crossover = rand() % (N - 1) + 1;
            int enfant1[N], enfant2[N];
            crossover(population[i], population[i + 1], enfant1, point_de_crossover);
            crossover(population[i + 1], population[i], enfant2, point_de_crossover);
            for (int j = 0; j < N; j++) {
                population[i][j] = enfant1[j];
                population[i + 1][j] = enfant2[j];
            }
            if ((double)rand() / RAND_MAX < MUTATION_RATE) {
                mutation(population[i]);
            }
            if ((double)rand() / RAND_MAX < MUTATION_RATE) {
                mutation(population[i + 1]);
            }
        }
    }
    printf("Ordre des citys a visiter : ");
    for (int i = 0; i < N; i++) {
        if(i=N-1){
            printf("%d\n", meilleur_parcours[i]);
        }
        else{
            printf("%d -> ", meilleur_parcours[i]);
        }
    }
    printf("\n");
    printf("Meilleure longueur : %.2f\n", 1.0 / meilleure_fitness);
}

/********************* Exploration par Réseau de Neurones Monocouche *********************************/

float const M=0.1;
float const THETA=0.2;
int const NBRENTREE=4;
int const NBRPOIDS=2;
void modifierPoids(float w[],int d[],int x[],int e[NBRPOIDS][NBRENTREE],int i)
{
    for(int j=0; j<NBRPOIDS; j++)
    {
        w[j]=w[j]+M*((d[i]-x[i])*e[j][i]);

    }
}

int calculerSortie(float w[],int e[NBRPOIDS][NBRENTREE],int i)
{
    float resultat=0;
    int resultatTemporaire;
    for(int j=0; j<NBRPOIDS; j++)
    {
        resultat+=w[j]*e[j][i];
    }
    resultat=resultat-THETA;

    if(resultat>0)
    {
        resultatTemporaire=1;
    }
    else
        resultatTemporaire=0;

    return resultatTemporaire;
}



void RN_monocoche()
{
    float w[NBRPOIDS]= {0.3,-0.1};
    int e[NBRPOIDS][NBRENTREE]= {{0,0,1,1},{0,1,0,1}};
    int d[NBRENTREE]= {0,0,0,1};
    int x[NBRENTREE];
    booleen fini=false;
    int nbrOK=0;
    while(!fini)
    {
        nbrOK=0;
        printf("e1 \t| e2 \t| d \t| w1       \t| w2     \t| x    \t| w1Final  | w2Final\n");
        printf("-----------------------------------------------------------------------------------------------------------------\n");
        for (int i=0; i<NBRENTREE; i++)
        {
            x[i]=calculerSortie(w,e,i);
            printf("%d \t| %d \t| %d \t| %f \t| %f \t| %d \t|",e[0][i],e[1][i],d[i],w[0],w[1],x[i]);
            if(x[i]!=d[i])
            {
                modifierPoids(w,d,x,e,i);
            }
            else
                nbrOK++;
            printf("%f | %f",w[0],w[1]);
            if(nbrOK==4)
                fini=true;
            printf("|\n");
        }
        printf("\n");
        printf("-----------------------------------------------------------------------------------------------------------------\n");
        printf("\n");
    }
}

/********************* Exploration par Réseau de Neurones Multicouche *********************************/

int const NBRENTREE_MULTIPLE=2;
int const NBR_ACTIVATION=7;

float sigmoide(float x)
{
    return 1/(1+exp(-x));
}


void RN_Multicouche()
{
    float w[NBR_ACTIVATION][NBR_ACTIVATION+1]= {{0,0,0,0,0,0,0,0},
        {0,0,0,0.5,-1,0,0,0},
        {0,0,0,1.5,-2,0,0,0},
        {0,0,0,0,0,1,-1,0},
        {0,0,0,0,0,3,-4,0},
        {0,0,0,0,0,0,0,1},
        {0,0,0,0,0,0,0,-3}
    };
    float temp=0.0;
    float alpha=0.1;
    float delta[NBR_ACTIVATION+1]= {0,0,0,0,0,0,0,0};
    float in[NBR_ACTIVATION+1]= {0,0,0,0,0,0,0,0};

    float a[NBR_ACTIVATION+1]= {0,0,0,0,0,0,0,0};
    float x[NBRENTREE_MULTIPLE+1]= {0,2,-1};
    printf(" Propagation Avant \n\n");
    do{
        for(int i=0; i<3; i++)
        {
            a[i]=x[i];
        }

        for( int j=3; j<=NBR_ACTIVATION; j++)
        {
            for( int i=0; i<NBR_ACTIVATION; i++)
            {
                in[j]+=w[i][j]*a[i];
            }
            a[j]=sigmoide(in[j]);
        }
        for(int j=1; j<NBR_ACTIVATION+1; j++)
        {
            printf("\ta%d  <===>  %f\n",j,a[j]);
        }
        delta[NBR_ACTIVATION]=1-a[NBR_ACTIVATION];
        printf("\n\n");
        printf(" Retropropagation \n\n");
        printf("\tDelta_Final  <====>  %f\n",delta[NBR_ACTIVATION]);
        for(int i=NBR_ACTIVATION-1; i>2; i--)
        {
            for(int j=1; j<NBR_ACTIVATION+1; j++)
            {
                temp+=w[i][j]*delta[j];

            }
            delta[i]=sigmoide(in[i])*(1-sigmoide(in[i]))*temp;
            printf("\tDelta%d  <====>  %f\n",i,delta[i]);
            temp=0;
        }
        printf("\n\n");
        for(int i=1; i<NBR_ACTIVATION; i++)
        {
            for(int j=1; j<NBR_ACTIVATION+1; j++)
            {
                if(w[i][j]!=0)
                {
                    w[i][j]=w[i][j]+alpha*a[i]*delta[j];
                }
            }
        }
        for(int i=1; i<NBR_ACTIVATION; i++)
        {
            for(int j=1; j<NBR_ACTIVATION+1; j++)
            {
                if(w[i][j]!=0)
                {
                    printf("\tW%d%d  <====>  %f\n",i,j,w[i][j]);
                }
            }
        }
        printf("\n\n");
        if((delta[NBR_ACTIVATION]>0.1))printf("Propagation Avant \n\n");
    }while(delta[NBR_ACTIVATION]>0.1);
}



