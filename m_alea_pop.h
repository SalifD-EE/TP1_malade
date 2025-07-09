/*=========================================================*/
// ete_2025 pour nos etudiants en C
/*=========================================================*/
#ifndef __ALEAPOP_2025_5326_
#define __ALEAPOP_2025_5326_

/*
	Voici sept fonctions offertes pour r�aliser 
	notre simulation num�rique


	Trois fonctions randi, randf et randf_bornes civilisent un peu
	le vieux rand et �mettent dans la distribution uniforme sp�cifi�e

	Deux fonctions randz, randnormal �mettent dans la distribution
	normale sp�cifi�e

	La fonction rand_age_canada donnera un age aleatoire � un canadien
	en respectant une distribution compil� par stats-can

	srand_sys rend plus simple l'initialisation 
	en utilisant l'horloge interne de la machine

*/
/*=========================================================*/

/*==========================================================*/
//permet de d�sactiver certains warnings stupides de visual
#define _CRT_SECURE_NO_WARNINGS   
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<assert.h>

/*==========================================================*/

#define EPSILON (0.0000001)
/*=========================================================*/


/*==========================================================*/
//  D�CLARATIONS DES FONCTIONS
/*==========================================================*/
/*  obtenir un age al�atoire pour un canadien en respectant 
    la distribution de StatsCan 2018

PARAMETRE(s) : aucun

SORTIE : une valeur entiere selon une distribution cr�dible

SPECS : aucune
*/
int rand_age_canada(void);

/*=========================================================*/
/*  srand_sys initialise le g�n�rateur de la valeur de l'horloge
	interne de la machine
	PARAMETRES :  Rien
	RETOUR : Rien

	SPECIFICATIONS : DANS UNE SIMULATION NUM�RIQUE, ON NE DOIT JAMAIS
		INITIALISER PLUS D'UNE FOIS UN G�N�RATEUR
*/
void srand_sys(void);
/*=========================================================*/
/*  randf donne un nombre r�el dans l'intervalle  [0,1]
	PARAMETRES :  aucun
	RETOUR : une valeur r�elle dans cet intervalle

	SPECIFICATIONS : aucune
*/
double randf(void);

#define RANDF (rand()/(1.0 * RAND_MAX+ EPSILON))

/*=========================================================*/
/*  randi donne un entier positif entre 1 et le parametre recu
	PARAMETRES :  nombre , un entier positif
	RETOUR : 0  ou  une valeur correcte entre 1 et n

	SPECIFICATIONS : recevant nombre <= 0 retour automatique de 0
*/
int randi(int nombre);

#define RANDI(n)    (((n) <= 0) ? 0 : ((int)((n) * RANDF) + 1))

/*=========================================================*/
/*  randf_bornes donne un nombre r�el dans l'intervalle sp�cifi�
	PARAMETRES :  deux r�els quelconques
	RETOUR : une valeur r�elle dans l'intervalle d�fini par ses
			ces deux valeurs

	SPECIFICATIONS : l'ordre des param�tres est sans importance
*/
double randf_bornes(double b1, double b2);

#define  RANDF_BORNES(b1, b2)   ((b1) + (((b2) - (b1)) * RANDF ))

/*=========================================================*/

/* La fonction randz

L'�mission d'un nombre r�el dans la distribution normale Z,
la plus classique des normales,   de moyenne 0 et d'�cart type 1

PARAMETRE(s) : Aucun

SORTIE : la valeur r�elle �mise par le g�n�rateur

SPECS : celle-ci a une parit� dans l'�mission
ce qui peut d�concerter si on r�initialise le g�n�rateur
en cours d'ex�cution ( ce qui en r�alit� ne doit PAS �tre fait)
*/
double randz(void);
/*==========================================================*/

/*==========================================================*/
/* rand_normal emet un nombre r�el dans une distribution normale quelconque,
  de moyenne mu  et d'�cart-type sigma donn�

PARAMETRE(s) : mu d�finit la moyenne  et sigma l'�cart-type

SORTIE : la valeur r�elle �mise par le g�n�rateur

SPECS : on traite sigma en valeur absolue n'a pas
*/
double rand_normal(double mu, double sigma);

#define RAND_NORMAL(mu,sigma)  ((mu) + (randz()* (sigma)))

/*=========================================================*/
#endif
/*=========================================================*/