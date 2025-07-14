/******************************************************************************/     
 /* M_ENSEMBLES_NOMS.H  
    ETE 2025  TP2 ET-HS
 

   interface du module de gestion des noms de villes  dans le TP2 

   les noms de ville  sont  case sensitive 
   saint-julien et Saint-Julien seront distingués
   on pourrait l'éviter mais c'est considéré comme 
   une perte de temps inutile dans notre programme
   
   la longeur du nom d'une ville est limitée par 
   la constante TAILLE_MAX_NOM

   les positions des noms de ville vont de 0
   au (nombre de villes présentes -1)

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
/*                    DÉCLARATION DES FONCTIONS                               */
/******************************************************************************/

//**********************************
/* CONSTRUCTEUR
	Créer un ensemble dynamique de noms de villes vide
	selon  la taille maximale  voulue

	RETOUR 1 pour succès et 0 sinon

	SPECS C'est une erreur de vouloir créer un ensemble de villes
	       si l'ensemble existe déjà
		   avant de pouvoir reutiliser cet init
	      il faudra  détruire explicitement l'ensemble existant
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
/* obtenir le nom de la ville à la position reçue 
   ( position à partir de 0)
   Si la position est fautive, retour NULL  */
const char* get_nom_ville(unsigned int position);

//**********************************
/* obtenir la position du nom d'une ville dans l'ensemble 
   ou NOM_ABSENT  */
unsigned int get_position_ville(const char* nom);

//**********************************
/*  comparaison de 2 noms de ville , Retourner 1 si les noms sont égaux
	0 sinon (une variation du retour ordinaire du "strcmp" utilisé).
*/
int comparer_noms_villes(const char* villeA, const char* villeB);

//**********************************

//**********************************
/* Ajouter un nouveau nom de ville à l'ensemble
	vérifier qu'il reste de la place 
	      que le nom n'est pas trop long
	      que le nom n'y est pas déjà

   RETOUR de 1 si le nom a été ajouté, 0 sinon
*/
int ajouter_nom_ville(const char* nom_ville);

//**********************************
/*  vider l'ensemble des noms présents
    l'ensemble de noms  est redevenu vide

	ne libère pas de mémoire
	l'ensemble pourra encore être utilisé
*/
void vider_ensemble_noms_villes(void);

//**********************************
/* libérer toute la mémoire utilisée 
    par l'ensemble de noms
	cet ensemble ne peut plus être utilisé
	mais peut être réinitialisé
*/
void detruire_ensemble_noms_villes(void);

/******************************************************************************/
#endif
/******************************************************************************/
