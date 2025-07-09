/*=========================================================*/
/*	par hs du SEG  */
/*=========================================================*/

/*=========================================================*/
/*  implémentation des vecteurs de l'espace  */
/*  qu'on appelle traditionnellement t_R3 */
/*=========================================================*/

/*  CE FICHIER NE DOIT PAS ÊTRE OUVERT ET LU AVANT D'AVOIR
	COMPLÉTÉ LES TESTS UNITAIRES DE COMPORTEMENT DES
	FONCTIONS DU même module
*/
/*=========================================================*/

/*=========================================================*/
#include "m_R3.h"
#include "Constante.h"

/*=========================================================*/
//#define M_PI 3.1416922653589793
//#define DEUX_PI (2.0 * M_PI) /* Utilisation de M_PI pour plus de précision si disponible */

//#define DEUX_PI 6.283185307179586 /* Valeur approchée de 2π */


/*=========================================================*/
// le constructeur explicite des 3 composantes
t_R3 R3_nouveau(double x, double y, double z) {
	t_R3 r = { x, y, z };
	return r;
}
/*=========================================================*/

/*  STRATÉGIE du constructeur
	obtenir theta entre 0 et 2*pi
	obtenir phi entre 0 et pi
	puis avec la norme, construire les composantes x, y et z
	des formules de conversion entre coordonnées
	sphériques et coordonnées rectangulaires
*/
t_R3 R3_rand(double norme) {
	t_R3 r = { 0, 0, 0 };
	/* Note : srand() doit être appelé au préalable pour initialiser la graine */
	if (norme > 0) {
		double theta = (DEUX_PI * rand()) / RAND_MAX;
		double phi = (DEUX_PI / 2.0 * rand()) / RAND_MAX;
		r.x = norme * cos(theta) * cos(phi);
		r.y = norme * sin(theta) * cos(phi);
		r.z = norme * sin(phi);
	}
	return r;
}

/*=========================================================*/
/* DÉFINITIONS DES FONCTIONS ASSOCIÉES À UN MANIEMENT NORMAL
   DES POINTS ET DES VECTEURS DE L'ESPACE */
   /*=========================================================*/

   /*=========================================================*/
   /*	STRATÉGIE
	   obtenue de la racine carrée de la somme
	   des carrés, c'est Pythagore
   */
double R3_norme(t_R3 p) {
	return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

/*=========================================================*/
/*	STRATÉGIE
	si norme(p) > 0 on retourne le vecteur
	dont chaque composante est divisée par sa norme
*/
t_R3 R3_normer(t_R3 p) {
	t_R3 resul = p;
	double longueur = R3_norme(resul);
	if (longueur > EPSILON) { /* Utilisation d'EPSILON pour éviter division par zéro */
		resul = R3_dilater(resul, 1.0 / longueur);
	}
	return resul;
}

/*=========================================================*/
/* STRATÉGIE
	la distance entre deux points est
	obtenue de la norme de leur différence
*/
double R3_distance(t_R3 p1, t_R3 p2) {
	return R3_norme(R3_diff(p1, p2));
}

/*=========================================================*/
/* STRATÉGIE
	test d'égalité des 3 composantes
*/
int R3_eq(t_R3 p1, t_R3 p2) {
	/* Vrai ssi les trois composantes sont égales */
	return (p1.x == p2.x) && (p1.y == p2.y) && (p1.z == p2.z);
}
/*=========================================================*/

/*=========================================================*/
/* STRATÉGIE
	tester si la distance est inférieure à un epsilon constant
*/
int R3_presque_eq(t_R3 p1, t_R3 p2) {
	return (R3_distance(p1, p2) < EPSILON) ? 1 : 0;
}

/*=========================================================*/

/*=========================================================*/
/* STRATÉGIE
	chaque composante est multipliée par le facteur voulu
	ce produit allonge le vecteur position
*/
t_R3 R3_dilater(t_R3 p, double facteur) {
	t_R3 resul;
	resul.x = facteur * p.x;
	resul.y = facteur * p.y;
	resul.z = facteur * p.z;
	return resul;
}

/*=========================================================*/
/* STRATÉGIE
	la somme se fait composante à composante
*/
t_R3 R3_sum(t_R3 p1, t_R3 p2) {
	t_R3 resul;
	resul.x = p1.x + p2.x;
	resul.y = p1.y + p2.y;
	resul.z = p1.z + p2.z;
	return resul;
}

/*=========================================================*/
/* STRATÉGIE
	la différence se fait composante à composante
*/
t_R3 R3_diff(t_R3 p1, t_R3 p2) {
	t_R3 resul;
	resul.x = p1.x - p2.x;
	resul.y = p1.y - p2.y;
	resul.z = p1.z - p2.z;
	return resul;
}

/*=========================================================*/
/* STRATÉGIE
	le produit scalaire obtenu
	avec la somme des produits des trois composantes
*/
double R3_scalaire(t_R3 p1, t_R3 p2) {
	return (p1.x * p2.x) + (p1.y * p2.y) + (p1.z * p2.z);
}

/*=========================================================*/
/* Stratégie
	le produit vectoriel obtenu
	avec le développement du déterminant 3x3 voir WIKI
*/
t_R3 R3_vectoriel(t_R3 p1, t_R3 p2) {
	t_R3 resul;
	resul.x = (p1.y * p2.z) - (p1.z * p2.y);
	resul.y = -1.0 * ((p1.x * p2.z) - (p1.z * p2.x));
	resul.z = (p1.x * p2.y) - (p1.y * p2.x);
	return resul;
}

/*=========================================================*/
/* Stratégie un simple printf */
void R3_afficher(t_R3 p) {
	fprintf(stdout, "\n( %f, %f, %f )", p.x, p.y, p.z);
}
/*=========================================================*/

// FIN DE L'IMPLÉMENTATION