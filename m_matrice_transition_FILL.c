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
		//assert(tester_une_distribution(i));
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
//int tester_une_distribution(int depart) {
//	int i;
//	double sum;
//
//	//boucle sur chaque valeur de cette ligne de la matrice
//		for (i = 0; i < near_markov.nb_dest ; ++i) {
//		//ajouter cette prob à la somme
//		sum += 
//		//si cette prob est > 1 ou < 0, return automatique de 0
//		if (                                ) {
//			return 0;
//		}
//	}
//
//	//si la somme des probs est trop loin de 1,return 0
//	if (fabs(sum - 1.0) >        ) {return 0;}
//	
//
//}

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
void liberer_mat_transition(void) {
	int i;

	// free de chaque ligne i, near_markov.probs[i]
	/*if (near_markov.probs[i]) {

	}*/
	// free de near_markov.probs
	
	//mettre les deux membres int de near_markov à 0
	
}
/******************************************************************************/

/******************************************************************************/

/*  une fois toutes les fonctions bien testées
     SCRAPER LE PETIT MAIN DE TEST .... 
*/

#if 0
int main(void) {

	int i, j;
	double prob;

	FILE* lecture = fopen("probs13.txt", "rt");
	assert(lecture);

	// init du générateur
	srand(time(NULL)); rand();


	lire_mat_transition(lecture);

	assert(get_nb_destination_transition() == 13);
	assert(get_nb_depart_transition() == 13);

	assert(get_prob_transition(1, 1) == 0);

	// affichage de la matrice
	for (i = 0; i < get_nb_depart_transition(); i++) {
		printf("\n\n probs sur la ligne %d de la matrice\n ", i);
		for (j = 0; j < get_nb_destination_transition(); j++) {
			prob = get_prob_transition(i, j);
			printf("%5.2f", prob);
		}
	}
	
	// reste les tests unitaires de qualité 
	// de la distribution pour chaque ville
	// la somme sur une ligne est  proche de 1
	for (i = 0; i < get_nb_depart_transition(); i++) {
		assert(tester_une_distribution(i));
	}

	printf("\n\n");
	// obtenir quelques destinations
	for (i = 0; i < get_nb_depart_transition(); i++) {
		int dest;
		dest = get_destination_transition(i);
		printf("\n depart %d destination %d \n",i,dest );
	}

	
	// test de sortie correcte pour une ville inexistante
	assert(get_destination_transition(1003) == DESTINATION_ABSENTE);
	
	printf("\n\n");

	system("pause");
	return EXIT_SUCCESS;

}

#endif


/******************************************************************************/

#if 0
/******************************************************************************/
/*                    DÉFINITIONS DES FONCTIONS                               */
/******************************************************************************/

/*
	allouer la matrice de probs (toutes les valeurs à 0)
	allouer la matrice d'indicateurs à 0
*/

void init_mat_transition(int nb_depart, int nb_destination) {
	int i;
	near_markov.nb_cols = near_markov.nb_ligs = 0;



	// si toutes les allocations fonctionnent 
	// faites des assert
	// terminer avec
	near_markov.nb_ligs = nb_depart;
	near_markov.nb_cols = nb_destination;

}

/******************************************************************************/
// si toutes les probs sont correctes ( 0 <= p <= 1) 
// et que leur somme donne approx 1  (fabs(somme - 1.0) > EPSILON_DISTRIBUTION)
// retour 1 et 0 sinon
static int tester_une_distribution(const double* probs) {



}
/******************************************************************************/
// si le point_depart est valide et tester la distribution donne 1
// copier les probs dans la ligne point_depart de la matrice et retour 1 (et 0 sinon)
int set_distribution_transition(int point_depart, const double* probs) {





}

/******************************************************************************/
 // si point_depart et destination valides retour de la prob à l'intersection de la ligne
// point_depart et de la colonne destination  (sinon PROB_ABSENTE)
double  get_prob_transition(int point_depart, int destination) {




}

/******************************************************************************/
/* si point_depart possède une distribution,  obtenir valeur test de randf et cumuler
	les probs (de i = 0 ...++i) jusqu'à dépasser ou égaler  test, retour i

	juste au cas retour  de near_markov.nb_cols - 1
*/

int get_destination_transition(int point_depart) {
	double test = randf();
	double som = 0;
	int i;




}

/******************************************************************************/
int  get_nb_depart_transition(void) {


}
/******************************************************************************/
int  get_nb_destination_transition(void) {

	return near_markov.nb_cols;
}
/******************************************************************************/


/******************************************************************************/
/*
   dans la structure static,  Libérer tout ce qui est dynamique
   et remettre les nombres à zéro
*/
void liberer_mat_transition(void) {
	int i;

	// la matrice et le tableau d'entiers


	near_markov.nb_ligs = near_markov.nb_cols = 0;
}
/******************************************************************************/
#endif
/******************************************************************************/

