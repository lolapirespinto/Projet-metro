#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <uvsqgraphics.h>
#include "metro.h"

//A executer avec le make

GRAPHE init_graphe(int debut){
	GRAPHE G;
	G.nb_sommets=TAILLE;
	G.S = calloc(TAILLE,sizeof(SOMMET)); //initalisation des sommets 
	G.E = calloc(TAILLE,sizeof(int)); //aucun sommet exploré
	return G;
}

GRAPHE init_sommet(GRAPHE G,char *nomfichier){
	FILE *f = fopen(nomfichier, "r");
	char nom_s;
	int num_s,num_l;
	int i = 0;
	int j = 0;
	//on passe les trois premières lignes
	while(i<3){
		nom_s=fgetc(f);
		if(nom_s == '\n')
		i++;
	}
	//on initialise le nom et numéro de chaque sommet
	for(i=0;i<G.nb_sommets;i++) {
		nom_s = fgetc(f);
		fscanf(f,"%d %d",&num_s,&num_l);
		j=0;
		while(nom_s != '\n') { //lecture du nom de la station
			nom_s=fgetc(f);
			G.S[i].nom_station[j]=nom_s;
			j++;
		}
		G.S[i].nom_station[j-1] = '\0';
		G.S[i].num_sommet=num_s;
		G.S[i].num_ligne=num_l;
	}
	fclose(f);
	return G;
}

GRAPHE init_poids(GRAPHE G,char *nomfichier){
	int taille=G.nb_sommets;
	//on initilise le poids des arrêtes à zéro
	for(int i=0;i<taille;i++){
		for(int j=0;j<taille;j++){
			poids[i][j]=0;
		}
	}
	FILE* fichier = NULL;
	fichier = fopen(nomfichier,"r");
	int dep=9218; //placement du curseur au bon endroit du fichier
	int num1,num2,sec;
	if(fichier != NULL){
		fseek(fichier,dep,SEEK_SET);
		for(int i=0;i<472;i++){
			fscanf(fichier,"%d",&num1); //lit le numéro du premier sommet
			fscanf(fichier,"%d",&num2); //lit le numéro du second sommet
			fscanf(fichier,"%d",&sec); //lit le poids en secondes
			fseek(fichier,2,SEEK_CUR);
			poids[num1][num2]=sec; 
		}
	}
	else{
		printf("Erreur dans l'ouverture du fichier \n");
	}
	return G;
}

GRAPHE init_debut(GRAPHE G,int debut){
	predecesseur[debut]=-1;
	d[debut]=0; G.E[debut]=1; //met le premier sommet exploré et la distance à zéro
	for(int i=0;i<TAILLE;i++){
		if(i!=debut){
			d[i]=infini; //met la distance avec les autres sommets à l'infini
		}
	}
	//met à jour les sommets voisins
	for(int i=0;i<TAILLE;i++){
		//si le poids n'est pas nul alors il y a une arrête entre les deux sommets
		if(poids[debut][i]!=0){ 
			d[i]=poids[debut][i];  //mis à jour des la distance
			predecesseur[i]=debut; //mis à jour du prédecesseur
		}
		if(poids[i][debut]!=0){
			d[i]=poids[i][debut];
			predecesseur[i]=debut;
		}
	}
	return G;
}

int trouve_min(GRAPHE G,int debut){
	int min=infini; //minimum initialisé à l'infini
	int smt=-1; 
	//parcours tous les sommets pour trouver la distance d[i] min
	for(int i=0;i<G.nb_sommets;i++){
		if(G.E[i]==0){
			if(d[i]<min){
				min=d[i];
				smt=i;
			}
		}
	}
	return smt; //renvoie le sommet correspondant
}

void maj_dvoisins(GRAPHE G,int num1,int num2){
	//si le sommet min est exploré mais pas son voisin
	if(G.E[num2]==1){
		if(G.E[num1]==0){ 
			//ajouter la nouvelle distance trouvée si est plus petite que l'ancienne
			if (d[num1]>(d[num2]+poids[num1][num2])){ 
				d[num1]=d[num2]+poids[num1][num2]; 
				predecesseur[num1]=num2; //mettre à jour le prédecesseur
			}
		}
	}
	//si son voisin est non exploré
	if(G.E[num2]==0){
		if (d[num2]>(d[num1]+poids[num2][num1])){
			d[num2]=d[num1]+poids[num2][num1];
			predecesseur[num2]=num1;
		}
	}
}

void ecrire_chemin(GRAPHE G,int tab[],int taille){
	int min=d[tab[0]]/60; //calcule le temps en minutes
	int sec=d[tab[0]]-(min*60); //le calcule le temps en secondes
	int cmpt=1; 
	printf("Vous êtes à %s.\n",G.S[tab[taille-1]].nom_station);
	for(int i=taille-2;i>0;i--){
		if(cmpt==1){
			printf("Prenez la ligne %d direction%s.\n",G.S[tab[i]].num_ligne,G.S[tab[i]].nom_station); 
			cmpt=2;
		}
		else{
			printf("A %s, prenez la direction de %s.\n",G.S[tab[i+1]].nom_station,G.S[tab[i]].nom_station); 
			cmpt=1;
		}
	}
	printf("Vous devriez arriver à %s dans %d minutes et %d secondes.\n",G.S[tab[0]].nom_station,min,sec);
}

void le_plus_court_chemin(GRAPHE G,int debut,int fin){
	int tmp=1;
	int final=fin;
	while (fin!=debut){
		if(fin==predecesseur[fin]){ //s'il n'a pas de predecesseur
			tmp=0;
			printf("Il n'y a pas de chemins!\n");
			exit(0); //quitte le programme si pas de chemins
		}
		fin=predecesseur[fin];
		tmp++;
	}
	//ajouter les prédecesseurs au tableau pour faciliter l'affichage
	int tab[tmp];
	tab[0]=final;
	for(int i=1;i<tmp;i++){
		tab[i]=predecesseur[final];
		final=predecesseur[final];
	}
	ecrire_chemin(G,tab,tmp);
}

void dijkstra(GRAPHE G,int debut,int fin){
	G=init_debut(G,debut);
	int min;
	for(int i=0;i<TAILLE;i++){
		//trouver la distance minimum
		min=trouve_min(G,debut);
		G.E[min]=1;
		//mis à jour des distances avec les voisins du sommet min
		for(int j=0;j<TAILLE;j++){
			if(poids[min][j]!=0){
				maj_dvoisins(G,min,j);
			}
			if(poids[j][min]!=0){
				maj_dvoisins(G,min,j);
			}
		}
	}
	//trouver le plus court chemin
	le_plus_court_chemin(G,debut,fin);
}

int main(){
	printf("Veuillez entrer le numéro du sommet de départ.\n");
	int debut=lire_entier_clavier();
	printf("Veuillez entrer le numéro du sommet d'arrivé.\n");
	int fin=lire_entier_clavier();
	//initialisation du graphe, du sommet et des arrêtes
	GRAPHE G=init_graphe(debut);
	G=init_sommet(G,"metro.txt");
	G=init_poids(G,"metro.txt");
	//appel de la fonction principal Djikstra sur les deux sommets choisis
	dijkstra(G,debut,fin);
	exit(0);
}
