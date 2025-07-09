/*=========================================================*/
/*	par hs du SEG  */
/*=========================================================*/
/*
  la cr�ation d'un type bien r�el : les vecteurs et les points de l'espace
  ordinaire qu'on appelle traditionnellement t_R3

  UN POINT EST ASSIMIL� � SON VECTEUR POSITION

  c'est le type de base � toute impl�mentation
  de la g�om�trie vectorielle �l�mentaire


  NOTE P�DAGOGIQUE :
  je n'y laisse que le minimum fonctionnel

  une impl�mentation compl�te serait bien plus g�n�reuse
  en fonctions offertes

  Vous rencontrerez bien des formes MACRO offertes
  elles sont pas toujours �videntes 
  d'autres impl�mentations sont possibles

  mais vous savez que c'est � partir de la forme fonctionnelle
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
// D�FINITION DU TYPE
/* un t_R3 a trois composantes r�elles en rectangulaire */
/* c'est pareil � ce qu'on trouve en maths et en physique */

typedef struct {
	double x;
	double y;
	double z;
}t_R3;

/*=========================================================*/

/*
NOTE P�DAGOGIQUE
la structure reste ici accesible au client
mais toutes les op�rations  sur ces objets sont offertes 
sous formes fonctionnelles

il serait assez facile d'impl�menter simultan�ment les formes cylindrique et
spherique d'un point -- certains calculs seraient fortement acc�l�r�s --
mais la forme rectangulaire suffira au d�part....

mettre les trois repr�sentations OBLIGE pratiquement � la  
repr�sentation cach�e pour �viter qu'un client irresponsable de 
corrompe l'�tat de ses propres variables(horreur)

vous avez toute latitude � me poser des questions
*/

/*=========================================================*/
/* OBTENIR DES OBJETS DU NOUVEAU TYPE */

/* UNIQUEMENT � LA D�CLARATION
   UN t_R3 POURRA �TRE INITIALIS�
   avec la syntaxe
	   t_R3 machin = {a,b,c} ;

	sans initialisation, les valeurs des composantes restent impr�visibles
	avec uniquement  = {a}; seul la composante x  est sp�cifi�e, les deux autres 
	                        seront mises � 0
	avec = {a, b} ;  x et y sont explicites et la composante z sera mise � 0

	On se servira naturellement de l'op�rateur  =  pour
	l'assignation des t_R3, elle agira pareillement � ce
	qu'on obtient d�j� d'un type de base( comme int ou double)
*/
/*=========================================================*/


/*=========================================================*/
// maintenant les fonctions du module

// d'abord  DEUX  CONSTRUCTEURS
/*=========================================================*/
/* Voici un premier CONSTRUCTEUR qui recoit les trois composantes
du nouveau point en parametres

	ENTR�E  :  les trois composantes r�elles
	SORTIE  : : un t_R3 avec ces composantes
	SP�CIFICATIONS : Aucune
*/
t_R3 R3_nouveau(double x, double y, double z);
/*=========================================================*/
/*  Voici un  autre CONSTRUCTEUR  connu pour �tre super utile, 
    il est capable de  donner un t_R3 al�atoire mais de longeur 
	donn�e en param�tre

	ENTR�E  :  la distance du point de l'origine
	SORTIE  : : un t_R3 de la norme donn�e obtenu al�atoirement
	SP�CIFICATIONS : si le param�tre est n�gatif, la fonction
			 retourne automatiquement l'origine
*/
t_R3 R3_rand(double norme);
/*=========================================================*/

/*=========================================================*/
/*
	D�CLARATIONS DES FONCTIONS ASSOCI�ES AU MANIEMENT
	NORMAL DES VECTEURS ET DES POINTS DE L'ESPACE

	NOTE P�DAGOGIQUE
	j'y offre quelques fonctions �l�mentaires dont vous devrez
	rapidement tester et contr�ler le comportement.
	pratiquez les tests unitaires (ils sont faciles � r�aliser )
	utilisez fr�quemment assert(....) pour tout tester 
	autant les fonctions que les Macros que vous saurez lire ou d�velopper
*/
/*=========================================================*/

/*=========================================================*/
// les fonctions �l�mentaires
// obtenir la norme d'un vecteur
// normer un vecteur
// obtenir la distance entre 2 points de l'espace
// tester d'�galit�
// tester la proximite suffisante
/*=========================================================*/
/* obtenir la longueur du vecteur (la norme)

	ENTR�E  :  le vecteur � traiter
	SORTIE  : : un r�el positif
	SP�CIFICATIONS :aucune
*/
double R3_norme(t_R3   p);

//en forme Macro
#define R3_NORME(p) (sqrt((p).x * (p).x + (p).y * (p).y + (p).z * (p).z))
/*=========================================================*/


/*=========================================================*/
/* obtenir un vecteur unitaire ( de longeur 1) de m�me direction que le parametre

	ENTR�E  :  le vecteur original
	SORTIE  : : le vecteur unitaire correspondant
	SP�CIFICATIONS : avec l'origine en param�tre, elle retourne
			automatiquement l'origine
*/
t_R3 R3_normer(t_R3  p);

//en forme Macro
#define R3_NORMER(p) ( (R3_NORME(p) > 0)? R3_nouveau((p).x/R3_NORME(p), (p).y/R3_NORME(p),(p).z/R3_NORME(p)): (p) )
/*=========================================================*/

/*=========================================================*/
/* obtenir la distance entre deux points de l'espace

	ENTR�E  :  les deux points � traiter
	SORTIE  : : un r�el positif
	SP�CIFICATIONS :aucune
*/
double R3_distance(t_R3  p1, t_R3  p2);

//en forme Macro
#define R3_DISTANCE(p1,p2) ( R3_NORME(R3_DIFF(p1, p2)))
/*=========================================================*/

/*=========================================================*/
/*  tester l'�galit�
	les deux points ont-ils les m�mes composantes?

	ENTR�E  :  les deux points � traiter
	SORTIE  : : 0 ou 1
	SP�CIFICATIONS :aucune
*/
int R3_eq(t_R3  p1, t_R3 p2);

//en forme Macro
#define R3_EQ(p1,p2)  (((p1).x == (p2).x ) && ( (p1).y == (p2).y) && ((p1).z == (p2).z))
/*=========================================================*/

/*=========================================================*/
/*  tester la proximit� de deux points de l'espace
// leur distance est-elle inf�rieure � un  epsilon constant


ENTR�E  :  les deux points � traiter 
SORTIE  : : 0 ou 1
SP�CIFICATIONS : aucune
*/
int R3_presque_eq(t_R3  p1, t_R3 p2);

//en forme Macro
#define R3_PRESQUE_EQ(p1,p2, epsilon) ( R3_DISTANCE(p1, p2) < EPSILON)? 1:0) 
/*=========================================================*/


/*=========================================================*/
// LES OPERATIONS 
// dilater un vecteur et les OPS BINAIRES les plus usuelles
// la somme, la diff�rence, le produit scalaire et le produit vectoriel

/*=========================================================*/
/* obtenir la somme de deux vecteurs
	la somme se fait composante � composante

	ENTR�E  :   deux vecteurs
	SORTIE  : : la somme, un t_R3
	SP�CIFICATIONS :aucune
*/
t_R3  R3_sum(t_R3  p1, t_R3 p2);

//en forme Macro
#define R3_SUM(p1,p2) ( R3_nouveau((p1).x + (p2).x, (p1).y + (p2).y, (p1).z + (p2).z))
/*=========================================================*/

/*=========================================================*/
/* obtenir  la diff�rence de deux vecteurs
		la diff�rence se fait composante � composante

	ENTR�E  :   deux vecteurs
	SORTIE  : : la diff�rence, un t_R3
	SP�CIFICATIONS :aucune
*/
t_R3  R3_diff(t_R3  p1, t_R3  p2);

//en forme Macro
#define R3_DIFF(p1,p2) ( R3_nouveau((p1).x - (p2).x, (p1).y - (p2).y, (p1).z - (p2).z))
/*=========================================================*/

/*=========================================================*/
/* obtenir le produit d'un vecteur par un scalaire r�el (*) 
   qui dilate le  vecteur      ie(allonge le vecteur) 

	ENTR�E  :  le vecteur et son facteur de dilatation
	SORTIE  : : le vecteurs r�sultat de cette �longation
	SP�CIFICATIONS :aucune

*/
t_R3 R3_dilater(t_R3  p, double facteur);

//en forme Macro
#define R3_DILATER(p, facteur) (R3_nouveau((facteur) * (p).x,  (facteur) * (p).y , (facteur) *  (p).z))
/*=========================================================*/

/*=========================================================*/
/* obtenir le produit scalaire
	QUI DONNE LE PRODUIT DES LONGUEURS MULTIPLI� PAR LE COSINUS
	DE L'ANGLE ENTRE LES DEUX VECTEURS (rappel)

	ENTR�E  :   deux vecteurs
	SORTIE  : : la valeur  r�elle calcul�e
	SP�CIFICATIONS :aucune
*/
double R3_scalaire(t_R3  p1, t_R3  p2);

//en forme Macro
#define R3_SCALAIRE(p1, p2) (((p1).x * (p2).x) + ((p1).y * (p2).y) + ((p1).z * (p2).z))
/*=========================================================*/

/*=========================================================*/
/* obtenir le produit vectoriel

	ENTR�E  :   deux vecteurs
	SORTIE  : : le vecteur calcul�
	SP�CIFICATIONS : aucune
*/
t_R3  R3_vectoriel(t_R3  p1, t_R3 p2);
//en forme Macro
#define R3_VECTORIEL(p1,p2) (R3_nouveau(((p1).y * (p2).z) - ((p1).z * (p2).y),  -1 * (((p1).x * (p2).z) - ((p1).z * (p2).x)),  ((p1).x * (p2).y) - ((p1).y * (p2).x)))       

/*=========================================================*/

/*=========================================================*/
/* 	 Si on veut utiliser le format d'affichage
	offert (x,y,z)  � l'�cran

	ENTR�E  :  le vecteur
	SORTIE  : : rien
	SP�CIFICATIONS : aucune
*/

void R3_afficher(t_R3  p);

//en forme Macro
#define R3_AFFICHER(p)  (fprintf(stdout, "\n( %f, %f, %f )", (p).x, (p).y, (p).z))
/*=========================================================*/

#endif // fin de l'interface
/*=========================================================*/

