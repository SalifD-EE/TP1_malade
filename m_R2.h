/*=========================================================*/
/* par hs du SEG  */
/*=========================================================*/

/*=========================================================*/
/* la création d'un type bien réel : les vecteurs et les points du plan */
/* qu'on appelle traditionnellement t_R2 */

/* UN POINT EST ASSIMILÉ À SON VECTEUR POSITION */

/* c'est le type de base à toute implémentation */
/* de la géométrie vectorielle élémentaire en 2D */

/* NOTE PÉDAGOGIQUE : */
/* je n'y laisse que le minimum fonctionnel */
/* une implémentation complète serait bien plus généreuse */
/* en fonctions offertes */

/* Vous rencontrerez bien des formes MACRO offertes */
/* elles sont pas toujours évidentes */
/* d'autres implémentations sont possibles */

/* mais vous savez que c'est à partir de la forme fonctionnelle */
/* qu'on développe la Macro correspondante */
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
/* DÉFINITION DU TYPE */
/* un t_R2 a deux composantes réelles en rectangulaire */
/* c'est pareil à ce qu'on trouve en maths et en physique */
typedef struct {
    double x;
    double y;
} t_R2;

/*=========================================================*/

/* NOTE PÉDAGOGIQUE */
/* la structure reste ici accessible au client */
/* mais toutes les opérations sur ces objets sont offertes */
/* sous formes fonctionnelles */

/*=========================================================*/
/* OBTENIR DES OBJETS DU NOUVEAU TYPE */

/* UNIQUEMENT À LA DÉCLARATION */
/* UN t_R2 POURRA ÊTRE INITIALISÉ */
/* avec la syntaxe */
/*     t_R2 machin = {a,b} ; */

/* sans initialisation, les valeurs des composantes restent imprévisibles */
/* avec uniquement = {a}; seule la composante x est spécifiée, y sera mise à 0 */

/* On se servira naturellement de l'opérateur = pour */
/* l'assignation des t_R2, elle agira pareillement à ce */
/* qu'on obtient déjà d'un type de base (comme int ou double) */
/*=========================================================*/

/*=========================================================*/
/* maintenant les fonctions du module */

/* d'abord DEUX CONSTRUCTEURS */
/*=========================================================*/
/* Voici un premier CONSTRUCTEUR qui reçoit les deux composantes */
/* du nouveau point en paramètres */

/* ENTRÉE : les deux composantes réelles */
/* SORTIE : un t_R2 avec ces composantes */
/* SPÉCIFICATIONS : Aucune */
t_R2 R2_nouveau(double x, double y);
/*=========================================================*/
/* Voici un autre CONSTRUCTEUR connu pour être super utile, */
/* il est capable de donner un t_R2 aléatoire mais de longueur */
/* donnée en paramètre */

/* ENTRÉE : la distance du point de l'origine */
/* SORTIE : un t_R2 de la norme donnée obtenu aléatoirement */
/* SPÉCIFICATIONS : si le paramètre est négatif, la fonction */
/* retourne automatiquement l'origine */
t_R2 R2_rand(double norme);
/*=========================================================*/

/*=========================================================*/
/* DÉCLARATIONS DES FONCTIONS ASSOCIÉES AU MANIEMENT */
/* NORMAL DES VECTEURS ET DES POINTS DU PLAN */

/* NOTE PÉDAGOGIQUE */
/* j'y offre quelques fonctions élémentaires dont vous devrez */
/* rapidement tester et contrôler le comportement. */
/* pratiquez les tests unitaires (ils sont faciles à réaliser) */
/* utilisez fréquemment assert(....) pour tout tester */
/* autant les fonctions que les Macros que vous saurez lire ou développer */
/*=========================================================*/

/*=========================================================*/
/* obtenir la longueur du vecteur (la norme) */

/* ENTRÉE : le vecteur à traiter */
/* SORTIE : un réel positif */
/* SPÉCIFICATIONS : aucune */
double R2_norme(t_R2 p);

/* en forme Macro */
#define R2_NORME(p) (sqrt((p).x * (p).x + (p).y * (p).y))
/*=========================================================*/

/*=========================================================*/
/* obtenir un vecteur unitaire (de longueur 1) de même direction que le paramètre */

/* ENTRÉE : le vecteur original */
/* SORTIE : le vecteur unitaire correspondant */
/* SPÉCIFICATIONS : avec l'origine en paramètre, elle retourne */
/* automatiquement l'origine */
t_R2 R2_normer(t_R2 p);

/* en forme Macro */
#define R2_NORMER(p) ((R2_NORME(p) > 0) ? R2_nouveau((p).x/R2_NORME(p), (p).y/R2_NORME(p)) : (p))
/*=========================================================*/

/*=========================================================*/
/* obtenir la distance entre deux points du plan */

/* ENTRÉE : les deux points à traiter */
/* SORTIE : un réel positif */
/* SPÉCIFICATIONS : aucune */
double R2_distance(t_R2 p1, t_R2 p2);

/* en forme Macro */
#define R2_DISTANCE(p1,p2) (R2_NORME(R2_diff(p1, p2)))
/*=========================================================*/

/*=========================================================*/
/* tester l'égalité */
/* les deux points ont-ils les mêmes composantes ? */

/* ENTRÉE : les deux points à traiter */
/* SORTIE : 0 ou 1 */
/* SPÉCIFICATIONS : aucune */
int R2_eq(t_R2 p1, t_R2 p2);

/* en forme Macro */
#define R2_EQ(p1,p2) (((p1).x == (p2).x) && ((p1).y == (p2).y))
/*=========================================================*/

/*=========================================================*/
/* tester la proximité de deux points du plan */
/* leur distance est-elle inférieure à un epsilon constant */

/* ENTRÉE : les deux points à traiter */
/* SORTIE : 0 ou 1 */
/* SPÉCIFICATIONS : aucune */
int R2_presque_eq(t_R2 p1, t_R2 p2);

/* en forme Macro */
#define R2_PRESQUE_EQ(p1,p2) (R2_DISTANCE(p1, p2) < EPSILON ? 1 : 0)
/*=========================================================*/

/*=========================================================*/
/* obtenir la somme de deux vecteurs */
/* la somme se fait composante à composante */

/* ENTRÉE : deux vecteurs */
/* SORTIE : la somme, un t_R2 */
/* SPÉCIFICATIONS : aucune */
t_R2 R2_sum(t_R2 p1, t_R2 p2);

/* en forme Macro */
#define R2_SUM(p1,p2) (R2_nouveau((p1).x + (p2).x, (p1).y + (p2).y))
/*=========================================================*/

/*=========================================================*/
/* obtenir la différence de deux vecteurs */
/* la différence se fait composante à composante */

/* ENTRÉE : deux vecteurs */
/* SORTIE : la différence, un t_R2 */
/* SPÉCIFICATIONS : aucune */
t_R2 R2_diff(t_R2 p1, t_R2 p2);

/* en forme Macro */
#define R2_DIFF(p1,p2) (R2_nouveau((p1).x - (p2).x, (p1).y - (p2).y))
/*=========================================================*/

/*=========================================================*/
/* obtenir le produit d'un vecteur par un scalaire réel (*) */
/* qui dilate le vecteur ie (allonge le vecteur) */

/* ENTRÉE : le vecteur et son facteur de dilatation */
/* SORTIE : le vecteur résultat de cette élongation */
/* SPÉCIFICATIONS : aucune */
t_R2 R2_dilater(t_R2 p, double facteur);

/* en forme Macro */
#define R2_DILATER(p, facteur) (R2_nouveau((facteur) * (p).x, (facteur) * (p).y))
/*=========================================================*/

/*=========================================================*/
/* obtenir le produit scalaire */
/* QUI DONNE LE PRODUIT DES LONGUEURS MULTIPLIÉ PAR LE COSINUS */
/* DE L'ANGLE ENTRE LES DEUX VECTEURS (rappel) */

/* ENTRÉE : deux vecteurs */
/* SORTIE : la valeur réelle calculée */
/* SPÉCIFICATIONS : aucune */
double R2_scalaire(t_R2 p1, t_R2 p2);

/* en forme Macro */
#define R2_SCALAIRE(p1, p2) (((p1).x * (p2).x) + ((p1).y * (p2).y))
/*=========================================================*/

/*=========================================================*/
/* Si on veut utiliser le format d'affichage */
/* offert (x,y) à l'écran */

/* ENTRÉE : le vecteur */
/* SORTIE : rien */
/* SPÉCIFICATIONS : aucune */
void R2_afficher(t_R2 p);

/* en forme Macro */
#define R2_AFFICHER(p) (fprintf(stdout, "\n( %f, %f )", (p).x, (p).y))
/*=========================================================*/

#endif // fin de l'interface
/*=========================================================*/