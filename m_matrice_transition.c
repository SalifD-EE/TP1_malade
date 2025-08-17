/******************************************************************************/
/* m_matrice_transition.c     (*** À COMPLÉTER ***)                              */
/* module de gestion d'une matrice de probabilités de passage               */
/******************************************************************************/

#include"m_matrice_transition.h"

/******************************************************************************/
//je vous les offre sous forme MACRO si vous en avez besoin

#define RANDF() (rand()/(RAND_MAX+1.000000000001))
#define RANDI(n) ((int)(1+RANDF()*(n))) 
/******************************************************************************/


/******************************************************************************/
/* La structure privée qui maintient la matrice de probabilités */
typedef struct {
	double** probs;      // matrice des probs de transition

	int nb_dep;             // nombre  de points de depart
	int nb_dest;            //  nombre  de destinations

} t_mat_probs;

/******************************************************************************/
/*  La matrice  PRIVÉE du module, 
   son usage est disponible pour tous les modules
    une fois la lecture établie 

*/
static t_mat_probs  near_markov;

/******************************************************************************/
/* obtenir  la mémoire nécessaire à la matrice privée du module
   c'est elle qui fait l'allocation dynamique de la matrice 
   les valeurs seront alors toutes à 0 
   chaque allocation est soumise à un assert
 */

static void init_mat_transition(int nb_depart, int nb_destination);
/******************************************************************************/

/******************************************************************************/
/*                    DÉFINITIONS DES FONCTIONS                               */
/******************************************************************************/


int lire_mat_transition(FILE* fichier) {

	int taille;
	int i, j;
	double x;
	
	// lire la taille de la matrice carrée
	fscanf(fichier, "%d", &taille);

	// provoquer les allocations dans near_markov
	init_mat_transition(taille, taille);

	// lire toutes les nombres réels du fichier
	// pour les mettre dans la matrice near_markov.probs
	for (i = 0; i < taille; ++i) {
		//lire toute la ligne i
		for (j = 0; j < taille; ++j) {
			fscanf(fichier, "%lf", &x );
			near_markov.probs[i][j] = x;
		}
		assert(tester_une_distribution(i));
	}

	return 1;
}
/****************************************************************************/


/****************************************************************************/
void init_mat_transition(int nb_depart, int nb_destination) {
	int i;

	// initialiser nb_dest et nb_dep
	near_markov.nb_dest = 0;
	near_markov.nb_dep = 0;

	//allocation des lignes
	assert(near_markov.probs = calloc(nb_depart, sizeof(double*)));

	//allocation des tableaux de probabilitées
	for (i = 0; i < nb_depart; i += 1) {
		assert(near_markov.probs[i] = calloc(nb_destination, sizeof(double)));
	}

	near_markov.nb_dest = nb_destination;
	near_markov.nb_dep = nb_depart;

}
/****************************************************************************/
/*  TESTER_UNE_DISTRIBUTION
	S'assurer que la somme des probabilites sur une ligne
	soit proche de 1 avec la tolérance EPSILON_DISTRIBUTION
	et qu'aucune des valeurs est négative ou supérieure à 1

	PARAMETRES: la ligne  de la matrice de transition qui doit être testée
	RETOUR: 1 si la somme est valide, 0 sinon
	SPEC: aucune
*/
int tester_une_distribution(int depart) {
	int i;
	double sum = 0;

	//boucle sur chaque valeur de cette ligne de la matrice
	for (i = 0; i < near_markov.nb_dest ; ++i) {

		//ajouter cette prob à la somme
			sum += near_markov.probs[depart][i];

		//si cette prob est > 1 ou < 0, return automatique de 0
		if (near_markov.probs[depart][i] > 1 || near_markov.probs[depart][i] < 0) {
			return 0;
		}
	}

	//si la somme des probs est trop loin de 1,return 0
	if (fabs(sum - 1.0) > EPSILON_DISTRIBUTION) {return 0;}
	

}

/****************** INFORMATRICES ***************************/
double  get_prob_transition(int depart, int destination) {
	double temp;

	 // validation du point de depart et de la destination
	if (depart >= 0 && depart < near_markov.nb_dep) {
		if (destination >= 0 && near_markov.nb_dest) {
			temp = near_markov.probs[depart][destination];
			return temp;
		}
	}
	return PROB_ABSENTE;
}
/******************************************************************************/
int get_destination_transition(int depart) {

	// probabilité à dépasser lors du choix de la ville
	double nextCity = RANDF();
	double sum = 0;
	int i = 0;

	// si le point de départ est <0 ou trop grand 
	// return DESTINATION_ABSENTE
	if (depart >= near_markov.nb_dep) return DESTINATION_ABSENTE;
	if (depart < 0) return DESTINATION_ABSENTE;

	i = RANDI(near_markov.nb_dest) - 1;
	assert(nextCity < 1.0);
	

	//boucle qui determine la  ville destination
	while (sum <= nextCity) {
		sum += near_markov.probs[depart][i];
		// si la somme >= nextCity on retourne i
		if (sum >= nextCity) return i;
		
		// on ajuste i en évitant de déborder
		i = (i + 1) % near_markov.nb_dest;
	}
	return i;
}
/******************************************************************************/
int  get_nb_depart_transition(void) {
	return near_markov.nb_dep;

}
/******************************************************************************/
int  get_nb_destination_transition(void) {
	return near_markov.nb_dest;
}
/******************************************************************************/

/****************** DESTRUCTEUR ***************************/
void detruire_mat_transition(void) {
	int i;

	// free de chaque ligne i, near_markov.probs[i]
	for (i = 0; i < near_markov.nb_dep; i++) {
		free(near_markov.probs[i]);
	}

	// free de near_markov.probs
	free(near_markov.probs);
	
	//mettre les deux membres int de near_markov à 0
	near_markov.nb_dest = near_markov.nb_dest = 0;
	
}
/******************************************************************************/