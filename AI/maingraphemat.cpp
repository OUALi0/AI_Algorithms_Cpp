#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphemat.cpp"  
#include "liregraphe.cpp" 

typedef GrapheMat Graphe; 

int menu()
{
    printf("\n\nEXPLORATION DES GRAPHES\n\n");
    printf("0 - Fin du programme\n");
    printf("1 - Creation a partir d'un fichier\n\n");
    printf("2 - Initialisation d'un graphe vide\n");
    printf("3 - Ajout d'un sommet\n");
    printf("4 - Ajout d'un arc\n\n");
    printf("5 - Liste des sommets et des arcs\n");
    printf("6 - Destruction du graphe\n");
    printf("7 - Parcours en profondeur d'un graphe\n\n");
    printf("8   - Exploration en Largeur d'abord\n");
    printf("9  - Exploration en  profondeur d'abord\n");
    printf("10  - Exploration en  profondeur Iteratif\n");
    printf("11  - Exploration a cout uniforme\n");
    printf("12  - Exploration A*\n");
    printf("13  - Exploration Meilleur Dabord\n");
    printf("14 - Exploration par Escalade\n");
    printf("15  - Exploration par recuit simule \n");
    printf("16  - Exploration par Algorithme Genitique \n");
    printf("17  - Exploration Reseau Neuron monocouche \n");
    printf("18  - Exploration Reseau Neuron multicouche \n");
    printf("Votre choix ? ");
    int cod;
    scanf("%d", &cod);
    getchar(); 
    printf("\n");
    return cod;
}

int main()
{
    Graphe *graphe;      
    booleen fini = faux; 
    
    double initial_solution = 5.0;
    double temperature = 100.0;
    int essais = 25;
    double temperature_stop = 0.01;
    double cooling_factor = 0.9;
    while (!fini)
    {
        switch (menu())
        {       
        case 0: 
            fini = vrai;
            break;
        case 1:
        { 
            printf("Nom du fichier contenant le graphe ? ");
            char nomFe[50];
            scanf("%s", nomFe);
            FILE *fe = fopen(nomFe, "r"); 
            if (fe == NULL)
            {
                perror(nomFe); 
            }
            else
            {
                graphe = lireGraphe(fe, 20); 
                fclose(fe);                  
            }
        }
        break;
        case 2:
        { 
            printf("Nombre maximum de sommets ? ");
            int nMaxSom;
            scanf("%d", &nMaxSom);
            printf("0) non valuee; 1) graphe valuee ? ");
            int value;
            scanf("%d", &value);
            graphe = creerGrapheMat(nMaxSom, value); 
        }
        break;
        case 3:
        {
            printf("Nom du sommet a insirer ? ");
            NomSom somD;
            scanf("%s", somD);
            ajouterUnSommet(graphe, somD); 
        }
        break;
        case 4:
        { 
            printf("Nom du sommet de depart ? ");
            NomSom somD;
            scanf("%s", somD);
            printf("Nom du sommet d'arrivee ? ");
            NomSom somA;
            scanf("%s", somA);
            int cout;
            if (graphe->value)
            {
                printf("Cout de la relation ? ");
                scanf("%d", &cout);
            }
            else
            {
                cout = 0;
            }
            ajouterUnArc(graphe, somD, somA, cout); 
        }
        break;
        case 5: 
            ecrireGraphe(graphe);
            break;
        case 6: 
            detruireGraphe(graphe);
            break;
        case 7: 
            parcoursProfond(graphe);
            break;
        case 8: 
            parcoursLargeurDabord(graphe);
            break;
        case 9: 
            parcoursProfondDAbord(graphe);
            break;
        case 10: 
            parcoursProfondeurIteratif(graphe);
            break;
        case 11: 
            parcoursCoutUniforme(graphe);
            break;
        case 12: 
            parcoursAEtoile(graphe);
            break;
        case 13:
        	parcoursMeilleurDabord(graphe);
            break;
        case 14:
        	parcoursEscalade(graphe);
            break;
       	case 15:
            Exploration_En_Recuit_Simule();
            break;
        case 16:
            Algorithme_Genitique();
            break;
        case 17:
            RN_monocoche();
            break;
        case 18:
            RN_Multicouche();
            break;
        }
        if (!fini)
        {
            printf("\n\nTaper Return pour continuer\n");
            getchar();
        }
    } 

    return 0;
}
