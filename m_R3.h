/*=========================================================*/
/*	par hs du SEG  */
/*=========================================================*/
/*
  la création d'un type bien réel : les vecteurs et les points de l'espace
  ordinaire qu'on appelle traditionnellement t_R3

  UN POINT EST ASSIMILÉ À SON VECTEUR POSITION

  c'est le type de base à toute implémentation
  de la géométrie vectorielle élémentaire


  NOTE PÉDAGOGIQUE :
  je n'y laisse que le minimum fonctionnel

  une implémentation complète serait bien plus généreuse
  en fonctions offertes

  Vous rencontrerez bien des formes MACRO offertes
  elles sont pas toujours évidentes 
  d'autres implémentations sont possibles

  mais vous savez que c'est à partir de la forme fonctionnelle
  qu'on developper la  Macro correspondante

*/

/*=========================================================*/
#ifndef __MODULE_POINTS_R3_H_65804_
#define __MODULE_POINTS_R3_H_65804_ 
/*=========================================================*/
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "m_alea_pop.h"
#include "Constante.h"

/*=========================================================*/
// erreur admise lors de l'utilisation de la fonction R3_presque_eq

/*=========================================================*/

/*=========================================================*/
// DÉFINITION DU TYPE
/* un t_R3 a trois composantes réelles en rectangulaire */
/* c'est pareil à ce qu'on trouve en maths et en physique */

typedef struct {
	double x;
	double y;
	double z;
}t_R3;

/*=========================================================*/

/*
NOTE PÉDAGOGIQUE
la structure reste ici accesible au client
mais toutes les opérations  sur ces objets sont offertes 
sous formes fonctionnelles

il serait assez facile d'implémenter simultanément les formes cylindrique et
spherique d'un point -- certains calculs seraient fortement accélérés --
mais la forme rectangulaire suffira au départ....

mettre les trois représentations OBLIGE pratiquement à la  
représentation cachée pour éviter qu'un client irresponsable de 
corrompe l'état de ses propres variables(horreur)

vous avez toute latitude à me poser des questions
*/

/*=========================================================*/
/* OBTENIR DES OBJETS DU NOUVEAU TYPE */

/* UNIQUEMENT À LA DÉCLARATION
   UN t_R3 POURRA ÊTRE INITIALISÉ
   avec la syntaxe
	   t_R3 machin = {a,b,c} ;

	sans initialisation, les valeurs des composantes restent imprévisibles
	avec uniquement  = {a}; seul la composante x  est spécifiée, les deux autres 
	                        seront mises à 0
	avec = {a, b} ;  x et y sont explicites et la composante z sera mise à 0

	On se servira naturellement de l'opérateur  =  pour
	l'assignation des t_R3, elle agira pareillement à ce
	qu'on obtient déjà d'un type de base( comme int ou double)
*/
/*=========================================================*/


/*=========================================================*/
// maintenant les fonctions du module

// d'abord  DEUX  CONSTRUCTEURS
/*=========================================================*/
/* Voici un premier CONSTRUCTEUR qui recoit les trois composantes
du nouveau point en parametres

	ENTRÉE  :  les trois composantes réelles
	SORTIE  : : un t_R3 avec ces composantes
	SPÉCIFICATIONS : Aucune
*/
t_R3 R3_nouveau(double x, double y, double z);
/*=========================================================*/
/*  Voici un  autre CONSTRUCTEUR  connu pour être super utile, 
    il est capable de  donner un t_R3 aléatoire mais de longeur 
	donnée en paramètre

	ENTRÉE  :  la distance du point de l'origine
	SORTIE  : : un t_R3 de la norme donnée obtenu aléatoirement
	SPÉCIFICATIONS : si le paramètre est négatif, la fonction
			 retourne automatiquement l'origine
*/
t_R3 R3_rand(double norme);
/*=========================================================*/

/*=========================================================*/
/*
	DÉCLARATIONS DES FONCTIONS ASSOCIÉES AU MANIEMENT
	NORMAL DES VECTEURS ET DES POINTS DE L'ESPACE

	NOTE PÉDAGOGIQUE
	j'y offre quelques fonctions élémentaires dont vous devrez
	rapidement tester et contrôler le comportement.
	pratiquez les tests unitaires (ils sont faciles à réaliser )
	utilisez fréquemment assert(....) pour tout tester 
	autant les fonctions que les Macros que vous saurez lire ou développer
*/
/*=========================================================*/

/*=========================================================*/
// les fonctions élémentaires
// obtenir la norme d'un vecteur
// normer un vecteur
// obtenir la distance entre 2 points de l'espace
// tester d'égalité
// tester la proximite suffisante
/*=========================================================*/
/* obtenir la longueur du vecteur (la norme)

	ENTRÉE  :  le vecteur à traiter
	SORTIE  : : un réel positif
	SPÉCIFICATIONS :aucune
*/
double R3_norme(t_R3   p);

//en forme Macro
#define R3_NORME(p) (sqrt((p).x * (p).x + (p).y * (p).y + (p).z * (p).z))
/*=========================================================*/


/*=========================================================*/
/* obtenir un vecteur unitaire ( de longeur 1) de même direction que le parametre

	ENTRÉE  :  le vecteur original
	SORTIE  : : le vecteur unitaire correspondant
	SPÉCIFICATIONS : avec l'origine en paramètre, elle retourne
			automatiquement l'origine
*/
t_R3 R3_normer(t_R3  p);

//en forme Macro
#define R3_NORMER(p) ( (R3_NORME(p) > 0)? R3_nouveau((p).x/R3_NORME(p), (p).y/R3_NORME(p),(p).z/R3_NORME(p)): (p) )
/*=========================================================*/

/*=========================================================*/
/* obtenir la distance entre deux points de l'espace

	ENTRÉE  :  les deux points à traiter
	SORTIE  : : un réel positif
	SPÉCIFICATIONS :aucune
*/
double R3_distance(t_R3  p1, t_R3  p2);

//en forme Macro
#define R3_DISTANCE(p1,p2) ( R3_NORME(R3_DIFF(p1, p2)))
/*=========================================================*/

/*=========================================================*/
/*  tester l'égalité
	les deux points ont-ils les mêmes composantes?

	ENTRÉE  :  les deux points à traiter
	SORTIE  : : 0 ou 1
	SPÉCIFICATIONS :aucune
*/
int R3_eq(t_R3  p1, t_R3 p2);

//en forme Macro
#define R3_EQ(p1,p2)  (((p1).x == (p2).x ) && ( (p1).y == (p2).y) && ((p1).z == (p2).z))
/*=========================================================*/

/*=========================================================*/
/*  tester la proximité de deux points de l'espace
// leur distance est-elle inférieure à un  epsilon constant


ENTRÉE  :  les deux points à traiter 
SORTIE  : : 0 ou 1
SPÉCIFICATIONS : aucune
*/
int R3_presque_eq(t_R3  p1, t_R3 p2);

//en forme Macro
#define R3_PRESQUE_EQ(p1,p2, epsilon) ( R3_DISTANCE(p1, p2) < EPSILON)? 1:0) 
/*=========================================================*/


/*=========================================================*/
// LES OPERATIONS 
// dilater un vecteur et les OPS BINAIRES les plus usuelles
// la somme, la différence, le produit scalaire et le produit vectoriel

/*=========================================================*/
/* obtenir la somme de deux vecteurs
	la somme se fait composante à composante

	ENTRÉE  :   deux vecteurs
	SORTIE  : : la somme, un t_R3
	SPÉCIFICATIONS :aucune
*/
t_R3  R3_sum(t_R3  p1, t_R3 p2);

//en forme Macro
#define R3_SUM(p1,p2) ( R3_nouveau((p1).x + (p2).x, (p1).y + (p2).y, (p1).z + (p2).z))
/*=========================================================*/

/*=========================================================*/
/* obtenir  la différence de deux vecteurs
		la différence se fait composante à composante

	ENTRÉE  :   deux vecteurs
	SORTIE  : : la différence, un t_R3
	SPÉCIFICATIONS :aucune
*/
t_R3  R3_diff(t_R3  p1, t_R3  p2);

//en forme Macro
#define R3_DIFF(p1,p2) ( R3_nouveau((p1).x - (p2).x, (p1).y - (p2).y, (p1).z - (p2).z))
/*=========================================================*/

/*=========================================================*/
/* obtenir le produit d'un vecteur par un scalaire réel (*) 
   qui dilate le  vecteur      ie(allonge le vecteur) 

	ENTRÉE  :  le vecteur et son facteur de dilatation
	SORTIE  : : le vecteurs résultat de cette élongation
	SPÉCIFICATIONS :aucune

*/
t_R3 R3_dilater(t_R3  p, double facteur);

//en forme Macro
#define R3_DILATER(p, facteur) (R3_nouveau((facteur) * (p).x,  (facteur) * (p).y , (facteur) *  (p).z))
/*=========================================================*/

/*=========================================================*/
/* obtenir le produit scalaire
	QUI DONNE LE PRODUIT DES LONGUEURS MULTIPLIÉ PAR LE COSINUS
	DE L'ANGLE ENTRE LES DEUX VECTEURS (rappel)

	ENTRÉE  :   deux vecteurs
	SORTIE  : : la valeur  réelle calculée
	SPÉCIFICATIONS :aucune
*/
double R3_scalaire(t_R3  p1, t_R3  p2);

//en forme Macro
#define R3_SCALAIRE(p1, p2) (((p1).x * (p2).x) + ((p1).y * (p2).y) + ((p1).z * (p2).z))
/*=========================================================*/

/*=========================================================*/
/* obtenir le produit vectoriel

	ENTRÉE  :   deux vecteurs
	SORTIE  : : le vecteur calculé
	SPÉCIFICATIONS : aucune
*/
t_R3  R3_vectoriel(t_R3  p1, t_R3 p2);
//en forme Macro
#define R3_VECTORIEL(p1,p2) (R3_nouveau(((p1).y * (p2).z) - ((p1).z * (p2).y),  -1 * (((p1).x * (p2).z) - ((p1).z * (p2).x)),  ((p1).x * (p2).y) - ((p1).y * (p2).x)))       

/*=========================================================*/

/*=========================================================*/
/* 	 Si on veut utiliser le format d'affichage
	offert (x,y,z)  à l'écran

	ENTRÉE  :  le vecteur
	SORTIE  : : rien
	SPÉCIFICATIONS : aucune
*/

void R3_afficher(t_R3  p);

//en forme Macro
#define R3_AFFICHER(p)  (fprintf(stdout, "\n( %f, %f, %f )", (p).x, (p).y, (p).z))
/*=========================================================*/

#endif // fin de l'interface
/*=========================================================*/

