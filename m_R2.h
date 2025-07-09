/*=========================================================*/
/* par hs du SEG  */
/*=========================================================*/

/*=========================================================*/
/* la cr�ation d'un type bien r�el : les vecteurs et les points du plan */
/* qu'on appelle traditionnellement t_R2 */

/* UN POINT EST ASSIMIL� � SON VECTEUR POSITION */

/* c'est le type de base � toute impl�mentation */
/* de la g�om�trie vectorielle �l�mentaire en 2D */

/* NOTE P�DAGOGIQUE : */
/* je n'y laisse que le minimum fonctionnel */
/* une impl�mentation compl�te serait bien plus g�n�reuse */
/* en fonctions offertes */

/* Vous rencontrerez bien des formes MACRO offertes */
/* elles sont pas toujours �videntes */
/* d'autres impl�mentations sont possibles */

/* mais vous savez que c'est � partir de la forme fonctionnelle */
/* qu'on d�veloppe la Macro correspondante */
/*=========================================================*/
#ifndef __MODULE_POINTS_R2_H_65804_
#define __MODULE_POINTS_R2_H_65804_ 
/*=========================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "m_alea_pop.h"
#include "Constante.h"


/*=========================================================*/
/* erreur admise lors de l'utilisation de la fonction R2_presque_eq */
//#define M_PI 3.1416922653589793

/*=========================================================*/

/*=========================================================*/
/* D�FINITION DU TYPE */
/* un t_R2 a deux composantes r�elles en rectangulaire */
/* c'est pareil � ce qu'on trouve en maths et en physique */
typedef struct {
    double x;
    double y;
} t_R2;

/*=========================================================*/

/* NOTE P�DAGOGIQUE */
/* la structure reste ici accessible au client */
/* mais toutes les op�rations sur ces objets sont offertes */
/* sous formes fonctionnelles */

/*=========================================================*/
/* OBTENIR DES OBJETS DU NOUVEAU TYPE */

/* UNIQUEMENT � LA D�CLARATION */
/* UN t_R2 POURRA �TRE INITIALIS� */
/* avec la syntaxe */
/*     t_R2 machin = {a,b} ; */

/* sans initialisation, les valeurs des composantes restent impr�visibles */
/* avec uniquement = {a}; seule la composante x est sp�cifi�e, y sera mise � 0 */

/* On se servira naturellement de l'op�rateur = pour */
/* l'assignation des t_R2, elle agira pareillement � ce */
/* qu'on obtient d�j� d'un type de base (comme int ou double) */
/*=========================================================*/

/*=========================================================*/
/* maintenant les fonctions du module */

/* d'abord DEUX CONSTRUCTEURS */
/*=========================================================*/
/* Voici un premier CONSTRUCTEUR qui re�oit les deux composantes */
/* du nouveau point en param�tres */

/* ENTR�E : les deux composantes r�elles */
/* SORTIE : un t_R2 avec ces composantes */
/* SP�CIFICATIONS : Aucune */
t_R2 R2_nouveau(double x, double y);
/*=========================================================*/
/* Voici un autre CONSTRUCTEUR connu pour �tre super utile, */
/* il est capable de donner un t_R2 al�atoire mais de longueur */
/* donn�e en param�tre */

/* ENTR�E : la distance du point de l'origine */
/* SORTIE : un t_R2 de la norme donn�e obtenu al�atoirement */
/* SP�CIFICATIONS : si le param�tre est n�gatif, la fonction */
/* retourne automatiquement l'origine */
t_R2 R2_rand(double norme);
/*=========================================================*/

/*=========================================================*/
/* D�CLARATIONS DES FONCTIONS ASSOCI�ES AU MANIEMENT */
/* NORMAL DES VECTEURS ET DES POINTS DU PLAN */

/* NOTE P�DAGOGIQUE */
/* j'y offre quelques fonctions �l�mentaires dont vous devrez */
/* rapidement tester et contr�ler le comportement. */
/* pratiquez les tests unitaires (ils sont faciles � r�aliser) */
/* utilisez fr�quemment assert(....) pour tout tester */
/* autant les fonctions que les Macros que vous saurez lire ou d�velopper */
/*=========================================================*/

/*=========================================================*/
/* obtenir la longueur du vecteur (la norme) */

/* ENTR�E : le vecteur � traiter */
/* SORTIE : un r�el positif */
/* SP�CIFICATIONS : aucune */
double R2_norme(t_R2 p);

/* en forme Macro */
#define R2_NORME(p) (sqrt((p).x * (p).x + (p).y * (p).y))
/*=========================================================*/

/*=========================================================*/
/* obtenir un vecteur unitaire (de longueur 1) de m�me direction que le param�tre */

/* ENTR�E : le vecteur original */
/* SORTIE : le vecteur unitaire correspondant */
/* SP�CIFICATIONS : avec l'origine en param�tre, elle retourne */
/* automatiquement l'origine */
t_R2 R2_normer(t_R2 p);

/* en forme Macro */
#define R2_NORMER(p) ((R2_NORME(p) > 0) ? R2_nouveau((p).x/R2_NORME(p), (p).y/R2_NORME(p)) : (p))
/*=========================================================*/

/*=========================================================*/
/* obtenir la distance entre deux points du plan */

/* ENTR�E : les deux points � traiter */
/* SORTIE : un r�el positif */
/* SP�CIFICATIONS : aucune */
double R2_distance(t_R2 p1, t_R2 p2);

/* en forme Macro */
#define R2_DISTANCE(p1,p2) (R2_NORME(R2_diff(p1, p2)))
/*=========================================================*/

/*=========================================================*/
/* tester l'�galit� */
/* les deux points ont-ils les m�mes composantes ? */

/* ENTR�E : les deux points � traiter */
/* SORTIE : 0 ou 1 */
/* SP�CIFICATIONS : aucune */
int R2_eq(t_R2 p1, t_R2 p2);

/* en forme Macro */
#define R2_EQ(p1,p2) (((p1).x == (p2).x) && ((p1).y == (p2).y))
/*=========================================================*/

/*=========================================================*/
/* tester la proximit� de deux points du plan */
/* leur distance est-elle inf�rieure � un epsilon constant */

/* ENTR�E : les deux points � traiter */
/* SORTIE : 0 ou 1 */
/* SP�CIFICATIONS : aucune */
int R2_presque_eq(t_R2 p1, t_R2 p2);

/* en forme Macro */
#define R2_PRESQUE_EQ(p1,p2) (R2_DISTANCE(p1, p2) < EPSILON ? 1 : 0)
/*=========================================================*/

/*=========================================================*/
/* obtenir la somme de deux vecteurs */
/* la somme se fait composante � composante */

/* ENTR�E : deux vecteurs */
/* SORTIE : la somme, un t_R2 */
/* SP�CIFICATIONS : aucune */
t_R2 R2_sum(t_R2 p1, t_R2 p2);

/* en forme Macro */
#define R2_SUM(p1,p2) (R2_nouveau((p1).x + (p2).x, (p1).y + (p2).y))
/*=========================================================*/

/*=========================================================*/
/* obtenir la diff�rence de deux vecteurs */
/* la diff�rence se fait composante � composante */

/* ENTR�E : deux vecteurs */
/* SORTIE : la diff�rence, un t_R2 */
/* SP�CIFICATIONS : aucune */
t_R2 R2_diff(t_R2 p1, t_R2 p2);

/* en forme Macro */
#define R2_DIFF(p1,p2) (R2_nouveau((p1).x - (p2).x, (p1).y - (p2).y))
/*=========================================================*/

/*=========================================================*/
/* obtenir le produit d'un vecteur par un scalaire r�el (*) */
/* qui dilate le vecteur ie (allonge le vecteur) */

/* ENTR�E : le vecteur et son facteur de dilatation */
/* SORTIE : le vecteur r�sultat de cette �longation */
/* SP�CIFICATIONS : aucune */
t_R2 R2_dilater(t_R2 p, double facteur);

/* en forme Macro */
#define R2_DILATER(p, facteur) (R2_nouveau((facteur) * (p).x, (facteur) * (p).y))
/*=========================================================*/

/*=========================================================*/
/* obtenir le produit scalaire */
/* QUI DONNE LE PRODUIT DES LONGUEURS MULTIPLI� PAR LE COSINUS */
/* DE L'ANGLE ENTRE LES DEUX VECTEURS (rappel) */

/* ENTR�E : deux vecteurs */
/* SORTIE : la valeur r�elle calcul�e */
/* SP�CIFICATIONS : aucune */
double R2_scalaire(t_R2 p1, t_R2 p2);

/* en forme Macro */
#define R2_SCALAIRE(p1, p2) (((p1).x * (p2).x) + ((p1).y * (p2).y))
/*=========================================================*/

/*=========================================================*/
/* Si on veut utiliser le format d'affichage */
/* offert (x,y) � l'�cran */

/* ENTR�E : le vecteur */
/* SORTIE : rien */
/* SP�CIFICATIONS : aucune */
void R2_afficher(t_R2 p);

/* en forme Macro */
#define R2_AFFICHER(p) (fprintf(stdout, "\n( %f, %f )", (p).x, (p).y))
/*=========================================================*/

#endif // fin de l'interface
/*=========================================================*/