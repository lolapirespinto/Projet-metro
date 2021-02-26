#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define TAILLE 375 //nombre de sommets du graphe
#define infini 1000 //pour définir la distance infini entre deux sommets
#define TAILLE_MAX 100 //taille pour la lecture dans le fichier metro.txt

int d[TAILLE]; //tableau des distances du sommet de départ à chaque sommet
int predecesseur[TAILLE];//tableau des prédécesseurs de chaque sommet
int poids[TAILLE][TAILLE]; //tableau correspondant aux poids des arrêtes entre chaque sommet

typedef struct {
	char nom_station[TAILLE_MAX]; //nom de la station
	int num_sommet; //numéro du sommet
	int num_ligne; //ligne de la station
}SOMMET;

typedef struct{
	int nb_sommets; 
	int *E; // Un tableau monodimensionnel de taille nb_sommets qui indique pour chaque sommet si il est exploré ou non
	SOMMET *S; //Un tableau de SOMMET de taille du nombre de sommet du graphe
}GRAPHE;

///////LISTE DES FONCTIONS//////////

GRAPHE init_graphe(int debut); ////initialisation du graphe
GRAPHE init_sommet(GRAPHE G,char *nomfichier); //initialisation des sommets 
GRAPHE init_poids(GRAPHE G,char *nomfichier); //initialisation du tableau des poids des arrêtes 
//soit le temps en secondes entre chaque station
GRAPHE init_debut(GRAPHE G,int debut); //mis à jour des données initial avec les informations
//du sommet de départ
int trouve_min(GRAPHE G,int debut); //recherche distance min
void maj_dvoisins(GRAPHE G,int num1,int num2); //mise à jour des voisins 
void le_plus_court_chemin(GRAPHE G,int debut,int fin); //renvoie le plus court chemin
void ecrire_chemin(GRAPHE G,int tab[],int taille); //affiche l'itinéraire trouvé
void dijkstra(GRAPHE G,int debut,int fin); //fonction principale
//implémentation de l'algorithme de Dijkstra
