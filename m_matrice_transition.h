/******************************************************************************/     
/* m_matrice_transition                        

    module de gestion d'une matrice de probabilités de passage 
	entre un groupe de points de départ et un groupe
	de destinations

	étant donné un point de départ du groupe
	la matrice est capable de donner
	une destination selon une distribution 
	spécifique à ce point de départ

*/
/******************************************************************************/

#ifndef  __MAT_PROBS_2020E86__
#define  __MAT_PROBS_2020E86__

#define _CRT_SECURE_NO_WARNINGS   
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include<assert.h>



//indicateur d'une destination impossible à déterminer
#define DESTINATION_ABSENTE  -1   

//indicateur d'une probabilité impossible à déterminer
#define PROB_ABSENTE  -1  

//indicateur d'une DISTRIBUTION acceptable
#define EPSILON_DISTRIBUTION  0.02 

/******************************************************************************/
/*                    DÉCLARATION DES FONCTIONS                               */
/******************************************************************************/

/*  
   Unique méthode pour obtenir une  matrice de probabilités 
   d'échanges  entre deux points 
   à la lecture d'un fichier à format déterminé
   */
int lire_mat_transition(FILE* fichier);
/******************************************************************************/


/******************************************************************************/
/*  deux informatrices
	Obtenir le nombre de points de départ */
int  get_nb_depart_transition(void);

/* Obtenir le nombre de points de destination */
int  get_nb_destination_transition(void);

/******************************************************************************/
/*  informatrice
	tester si la somme des probabitités sur une ligne
	de la matrice est suffisamment proche de 1
	à EPSILON_DISTRIBUTION près 
    depart doit varier entre 0 et (nombre_depart-1)
*/
int tester_une_distribution(int depart);

/******************************************************************************/
/*  informatrice
	Obtenir la probabilité de migration  d'un point de départ 
	vers un point de destination 
    si un paramètre est hors matrice retour automatique de 0
*/
double  get_prob_transition(int depart, int destination);

/******************************************************************************/
/*  informatrice LA PLUS IMPORTANTE 
	Obtenir à partir d'un point de départ une destination aléatoire
    représentée par une valeur entre 0 et (nb_destination -1)

	 depart doit varier entre 0 et (nombre_depart-1)
	 et retour de DESTINATION_ABSENTE sinon
*/
int get_destination_transition(int point_depart);

/******************************************************************************/
/* Libérer toute la mémoire dynamique de la matrice */
void detruire_mat_transition(void);

/******************************************************************************/
#endif
/******************************************************************************/
/******************************************************************************/
