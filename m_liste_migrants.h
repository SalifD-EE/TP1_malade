/*---------------------------------------------------*/
/* 	par husiro 	*/
/* 	modifié par salif diarra et léo bouamrane 	     */
/*---------------------------------------------------*/

/*-----------------------------------------------------------------*/
#ifndef __LISTEMIGRANTS_9784__
#define __LISTEMIGRANTS_9784__
/*-----------------------------------------------------------------*/

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<assert.h>
#include"m_migrant.h"

/*-----------------------------------------------------------------*/
// type d'objets dans les listes 

typedef t_migrant t_el_liste_migrants;
/*-----------------------------------------------------------------*/
/* TYPE DU HANDLE DU NOEUD */

typedef struct noeud_migrant* t_noeud_migrant;
//la definition du noeud est dans l'implementation (.c) du module

/*-----------------------------------------------------------------*/
/* TYPE DU HANDLE DE CHAQUE LISTE INSTANTIÉE PAR LE CONSTRUCTEUR */
typedef struct liste_migrants* t_liste_migrants;

/*-----------------------------------------------------------------*/
// VOICI L'ARTIFICE une simple constante à 0 ou à 1

#define LISTE_CACHER 1

/*  la definition de la liste  de 5 membres peut être dans
	l'interface -- ce qui est très très scolaire (comme ici)
	ou dans l'implémentation ce qui est nettement mieux

	avec la constante LISTE_CACHER à 1 la définition de la stucture
	est hors d'atteite d'un utilateur de la liste

	sinon la liste est  équivalente mais un utilisateur aura accès aux
	membres de la stucture avec un potentiel d'erreurs tragiques

	essayez et vous verrez
	tout est automatisé par LISTE_CACHER

*/
/*-----------------------------------------------------------------*/
#if (!LISTE_CACHER)
struct listeD00 {

	/* la taille actuelle de la liste */
	int  taille;

	/*le pointeur courant*/
	t_noeudD0  iterateur;

	/* position de l'itérateur*/
	int  position;

	/*pointeur du premier noeud*/
	t_noeudD0 tete;

	/* pointeur du dernier noeud*/
	t_noeudD0  queue;

	/* un  invariant de la liste
	Si elle n'est pas vide,
	l'itérateur actuel, tete et queue  ne seront jamais NULL
	*/
};
#endif
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/* LA GESTION DE LA LISTE */
/*-----------------------------------------------------------------*/
/* CONSTRUCTEUR
	entree : Rien
	Retour : un handle sur la nouvelle liste ou NULL

	Spécifications :

*/
t_liste_migrants init_liste_migrants(void);

/*-----------------------------------------------------------------*/
/* DESTRUCTEUR
	entree : le handle d'une liste existante
	Retour : Rien
	Spécifications :
	Vide la liste et l'élimine du programme .
	c'est une erreur d'utiliser ce handle de liste
	aprÈs cet appel.
*/
void detruire_liste_migrants(t_liste_migrants list);

/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/* INFORMATRICES
	retourne le nombre d'éléments actuellement dans la liste
	entrée : le handle d'une liste existante
	Retour : Sa cardinalité
	Spécifications : le résultat est imprévisible
	si le handle n'est pas valide
*/

int get_dans_liste_migrants(const t_liste_migrants list);

/*prédicat : savoir si la liste est vide
entrée : le handle d'une liste existante
	Retour : 0 ou 1
	Spécifications : le résultat est imprévisible
	si le handle n'est pas valide
*/
int est_vide_liste_migrants(const t_liste_migrants list);

/*	Retourne la position actuelle de l'itérateur
entrée : le handle d'une liste existante
Retour : Entre 0 et taille -1  de la liste
		  -1 si elle est vide
*/
int get_position_liste_migrants(const t_liste_migrants list);

/*	copie  la valeur actuelle du noeud courant dans la référence
   position actuelle de l'itérateur
entrée : le handle d'une liste existante
			la référence ou copier la valeur
Retour : 1 si la liste n'est pas vide 0 sinon
*/
int get_valeur_liste_migrants(const t_liste_migrants list, t_el_liste_migrants* dest);


/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*	TOUTES LES ACTIONS MUTATRICES

		elles ont en commun la spécification :
			le résultat est imprévisible
			si le handle n'est pas valide
*/
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
// CINQ MÉTHODES DE DÉPLACEMENT DE L'ITÉRATEUR

/* 	Avancer l'itérateur d'une position
	entrée : le handle d'une liste existante
	retour : 0 si la liste est vide ou si l'itérateur est positionné
		sur le dernier élément,  impossible  d'avancer
		1 sinon.
*/
int avancer_liste_migrants(t_liste_migrants list);

///*-----------------------------------------------------------------*/
///* 	Reculer l'itérateur d'une position
//	entrée : le handle d'une liste existante
//	retour :  0 si la liste est vide ou si l'itérateur est positionné
//			  sur le premier élément et ne peut reculer
//			  1 sinon.
//*/
//int reculer_liste_migrants(t_liste_migrants list);

/*-----------------------------------------------------------------*/
/* 	Placer l'itérateur en position voulue
	entrée : le handle d'une liste existante
			 une position entiere  valide
	retour : 0 si la liste est vide ou la position voulue
			 est supérieure OU égale à la taille de la liste
			 1 sinon.
*/
int positionner_liste_migrants(t_liste_migrants list, int position);

/*-----------------------------------------------------------------*/
/* Placer l'itérateur au début de la liste
	entrée : le handle d'une liste existante
	retour de 0 si la liste est vide et	1 sinon.
*/
int aller_debut_liste_migrants(t_liste_migrants list);

/*-----------------------------------------------------------------*/
/* 	Placer l'itérateur en fin de liste
	entrée : le handle d'une liste existante
	retour de 0 si la liste est vide et	1 sinon.
*/
int aller_fin_liste_migrants(t_liste_migrants list);
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/*   INSERER , SUPPRIMER,  CONSULTER OU MODIFIER
	 L'ÉLÉMENT SOUS L'ITÉRATEUR et plus....
*/
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/* 	obtenir  la valeur  sous l'itérateur
	 Retour : 1 si la liste n'est pas vide
			   0 sinon
*/
int consulter_liste_migrants(const t_liste_migrants list, t_el_liste_migrants* dest);

/*-----------------------------------------------------------------*/
/* 	obtenir l'adresse  de la valeur sous l'itérateur
	Retour : adresse non NULL si la liste n'est pas vide
			 NULL sinon
*/
t_el_liste_migrants* modifier_liste_migrants(t_liste_migrants list);

/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/*  4 formes pour inserer un élément à la liste	qui recoivent toutes
	un pointeur source constant
	l'iterateur ira sur le nouveau noeud.

	1- le nouveau noeud occupe la position de l'itérateur
	   Donc impossibilité d'ajouter à la fin

	2 et 3- les deux formes explicites au début et à la fin et déplacent l'iterateur

	4-  inserer à une position donnée correcte
		0 va inserer au debut et taille de la liste à la fin

	Retour : 1 insertion réussie, 0 sinon
*/
/*-----------------------------------------------------------------*/
int inserer_liste_migrants(t_liste_migrants list, const t_el_liste_migrants* src);
/*-----------------------------------------------------------------*/
int inserer_debut_liste_migrants(t_liste_migrants list, const t_el_liste_migrants* src);
/*-----------------------------------------------------------------*/
int inserer_fin_liste_migrants(t_liste_migrants list, const t_el_liste_migrants* src);
/*-----------------------------------------------------------------*/
int inserer_position_liste_migrants(t_liste_migrants list, int position, const t_el_liste_migrants* src);
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/


/*-----------------------------------------------------------------*/
/*  4 formes pour supprimer un élément de la liste

	1- supprimer à la position de l'itérateur.

	2 et 3- les deux formes explicites au début et à la fin

	4-  supprimer à une position donnée correcte

	Retour : 1 c'est réussi , 0 la liste est vide ou la position est erronée

	l'iterateur se positionne au debut à la fin  ou reste à la meme position

*/
/*-----------------------------------------------------------------*/
int supprimer_liste_migrants(t_liste_migrants list);
/*-----------------------------------------------------------------*/
int supprimer_debut_liste_migrants(t_liste_migrants list);
/*-----------------------------------------------------------------*/
int supprimer_fin_liste_migrants(t_liste_migrants list);
/*-----------------------------------------------------------------*/
int supprimer_position_liste_migrants(t_liste_migrants list, int position);
/*-----------------------------------------------------------------*

/*-----------------------------------------------------------------*/
/* La liste est vidée sans récupération des objets présents
	entrée : le handle d'une liste existante
	Retour : Rien

	Spécifications : cette liste pourra encore servir

*/

int vider_liste_migrants(t_liste_migrants list);

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/* 	FIN DE LA LISTE */
/*-----------------------------------------------------------------*/
#endif