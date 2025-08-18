/******************************************************************************/     
/* m_matrice_transition                        

    module de gestion d'une matrice de probabilit�s de passage 
	entre un groupe de points de d�part et un groupe
	de destinations

	�tant donn� un point de d�part du groupe
	la matrice est capable de donner
	une destination selon une distribution 
	sp�cifique � ce point de d�part

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



//indicateur d'une destination impossible � d�terminer
#define DESTINATION_ABSENTE  -1   

//indicateur d'une probabilit� impossible � d�terminer
#define PROB_ABSENTE  -1  

//indicateur d'une DISTRIBUTION acceptable
#define EPSILON_DISTRIBUTION  0.02 

/******************************************************************************/
/*                    D�CLARATION DES FONCTIONS                               */
/******************************************************************************/

/*  
   Unique m�thode pour obtenir une  matrice de probabilit�s 
   d'�changes  entre deux points 
   � la lecture d'un fichier � format d�termin�
   */
int lire_mat_transition(FILE* fichier);
/******************************************************************************/


/******************************************************************************/
/*  deux informatrices
	Obtenir le nombre de points de d�part */
int  get_nb_depart_transition(void);

/* Obtenir le nombre de points de destination */
int  get_nb_destination_transition(void);

/******************************************************************************/
/*  informatrice
	tester si la somme des probabitit�s sur une ligne
	de la matrice est suffisamment proche de 1
	� EPSILON_DISTRIBUTION pr�s 
    depart doit varier entre 0 et (nombre_depart-1)
*/
int tester_une_distribution(int depart);

/******************************************************************************/
/*  informatrice
	Obtenir la probabilit� de migration  d'un point de d�part 
	vers un point de destination 
    si un param�tre est hors matrice retour automatique de 0
*/
double  get_prob_transition(int depart, int destination);

/******************************************************************************/
/*  informatrice LA PLUS IMPORTANTE 
	Obtenir � partir d'un point de d�part une destination al�atoire
    repr�sent�e par une valeur entre 0 et (nb_destination -1)

	 depart doit varier entre 0 et (nombre_depart-1)
	 et retour de DESTINATION_ABSENTE sinon
*/
int get_destination_transition(int point_depart);

/******************************************************************************/
/* Lib�rer toute la m�moire dynamique de la matrice */
void detruire_mat_transition(void);

/******************************************************************************/
#endif
/******************************************************************************/
/******************************************************************************/
