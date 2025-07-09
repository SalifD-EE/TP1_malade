/*=========================================================*/
// ete_2025 pour nos etudiants en C
/*=========================================================*/
#ifndef __ALEAPOP_2025_5326_
#define __ALEAPOP_2025_5326_

/*
	Voici sept fonctions offertes pour réaliser 
	notre simulation numérique


	Trois fonctions randi, randf et randf_bornes civilisent un peu
	le vieux rand et émettent dans la distribution uniforme spécifiée

	Deux fonctions randz, randnormal émettent dans la distribution
	normale spécifiée

	La fonction rand_age_canada donnera un age aleatoire à un canadien
	en respectant une distribution compilé par stats-can

	srand_sys rend plus simple l'initialisation 
	en utilisant l'horloge interne de la machine

*/
/*=========================================================*/

/*==========================================================*/
//permet de désactiver certains warnings stupides de visual
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
//  DÉCLARATIONS DES FONCTIONS
/*==========================================================*/
/*  obtenir un age aléatoire pour un canadien en respectant 
    la distribution de StatsCan 2018

PARAMETRE(s) : aucun

SORTIE : une valeur entiere selon une distribution crédible

SPECS : aucune
*/
int rand_age_canada(void);

/*=========================================================*/
/*  srand_sys initialise le générateur de la valeur de l'horloge
	interne de la machine
	PARAMETRES :  Rien
	RETOUR : Rien

	SPECIFICATIONS : DANS UNE SIMULATION NUMÉRIQUE, ON NE DOIT JAMAIS
		INITIALISER PLUS D'UNE FOIS UN GÉNÉRATEUR
*/
void srand_sys(void);
/*=========================================================*/
/*  randf donne un nombre réel dans l'intervalle  [0,1]
	PARAMETRES :  aucun
	RETOUR : une valeur réelle dans cet intervalle

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
/*  randf_bornes donne un nombre réel dans l'intervalle spécifié
	PARAMETRES :  deux réels quelconques
	RETOUR : une valeur réelle dans l'intervalle défini par ses
			ces deux valeurs

	SPECIFICATIONS : l'ordre des paramètres est sans importance
*/
double randf_bornes(double b1, double b2);

#define  RANDF_BORNES(b1, b2)   ((b1) + (((b2) - (b1)) * RANDF ))

/*=========================================================*/

/* La fonction randz

L'émission d'un nombre réel dans la distribution normale Z,
la plus classique des normales,   de moyenne 0 et d'écart type 1

PARAMETRE(s) : Aucun

SORTIE : la valeur réelle émise par le générateur

SPECS : celle-ci a une parité dans l'émission
ce qui peut déconcerter si on réinitialise le générateur
en cours d'exécution ( ce qui en réalité ne doit PAS être fait)
*/
double randz(void);
/*==========================================================*/

/*==========================================================*/
/* rand_normal emet un nombre réel dans une distribution normale quelconque,
  de moyenne mu  et d'écart-type sigma donné

PARAMETRE(s) : mu définit la moyenne  et sigma l'écart-type

SORTIE : la valeur réelle émise par le générateur

SPECS : on traite sigma en valeur absolue n'a pas
*/
double rand_normal(double mu, double sigma);

#define RAND_NORMAL(mu,sigma)  ((mu) + (randz()* (sigma)))

/*=========================================================*/
#endif
/*=========================================================*/