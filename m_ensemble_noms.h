/******************************************************************************/     
 /* M_ENSEMBLES_NOMS.H  
    ETE 2025  TP2 ET-HS
 

   interface du module de gestion des noms de villes  dans le TP2 

   les noms de ville  sont  case sensitive 
   saint-julien et Saint-Julien seront distingu�s
   on pourrait l'�viter mais c'est consid�r� comme 
   une perte de temps inutile dans notre programme
   
   la longeur du nom d'une ville est limit�e par 
   la constante TAILLE_MAX_NOM

   les positions des noms de ville vont de 0
   au (nombre de villes pr�sentes -1)

*/

/******************************************************************************/

#ifndef  __GROUPNOMS_20250054__
#define  __GROUPNOMS_20250054__

/******************************************************************************/

#define _CRT_SECURE_NO_WARNINGS   
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<assert.h>

/******************************************************************************/

/* taille maximale d'un nom de ville dans l'ensemble */
#define TAILLE_MAX_NOM  80   

//indicateur d'un nom de ville absent de l'ensemble 
#define NOM_ABSENT  -1   

/******************************************************************************/
/*                    D�CLARATION DES FONCTIONS                               */
/******************************************************************************/

//**********************************
/* CONSTRUCTEUR
	Cr�er un ensemble dynamique de noms de villes vide
	selon  la taille maximale  voulue

	RETOUR 1 pour succ�s et 0 sinon

	SPECS C'est une erreur de vouloir cr�er un ensemble de villes
	       si l'ensemble existe d�j�
		   avant de pouvoir reutiliser cet init
	      il faudra  d�truire explicitement l'ensemble existant
	      avec detruire_ensemble_noms_villes
*/
int init_ensemble_noms_villes(unsigned int taille);

//**********************************
/* obtenir le nombre actuel de noms de villes dans l'ensemble */
unsigned int get_nombre_villes(void);

//**********************************
/* obtenir le nombre maximum de noms de villes de l'ensemble */
unsigned int get_max_villes(void);

//**********************************
/* obtenir le nom de la ville � la position re�ue 
   ( position � partir de 0)
   Si la position est fautive, retour NULL  */
const char* get_nom_ville(unsigned int position);

//**********************************
/* obtenir la position du nom d'une ville dans l'ensemble 
   ou NOM_ABSENT  */
unsigned int get_position_ville(const char* nom);

//**********************************
/*  comparaison de 2 noms de ville , Retourner 1 si les noms sont �gaux
	0 sinon (une variation du retour ordinaire du "strcmp" utilis�).
*/
int comparer_noms_villes(const char* villeA, const char* villeB);

//**********************************

//**********************************
/* Ajouter un nouveau nom de ville � l'ensemble
	v�rifier qu'il reste de la place 
	      que le nom n'est pas trop long
	      que le nom n'y est pas d�j�

   RETOUR de 1 si le nom a �t� ajout�, 0 sinon
*/
int ajouter_nom_ville(const char* nom_ville);

//**********************************
/*  vider l'ensemble des noms pr�sents
    l'ensemble de noms  est redevenu vide

	ne lib�re pas de m�moire
	l'ensemble pourra encore �tre utilis�
*/
void vider_ensemble_noms_villes(void);

//**********************************
/* lib�rer toute la m�moire utilis�e 
    par l'ensemble de noms
	cet ensemble ne peut plus �tre utilis�
	mais peut �tre r�initialis�
*/
void detruire_ensemble_noms_villes(void);

/******************************************************************************/
#endif
/******************************************************************************/
