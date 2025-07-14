/*=========================================================*/
/* Module m_R2 pour INF147 - Été 2025 */
/* Gestion des vecteurs et points en 2D */
/* Salif Diarra et Léo Bouamrane */
/*=========================================================*/
#include "m_R2.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Constante.h"



/*=========================================================*/
/* Constructeur : crée un t_R2 avec les composantes données */
t_R2 R2_nouveau(double x, double y) {
    t_R2 p;
    p.x = x;
    p.y = y;
    return p;
}

/*=========================================================*/
/* Constructeur : crée un t_R2 aléatoire de norme donnée */
t_R2 R2_rand(double norme) {
    if (norme <= 0) {
        return R2_nouveau(0.0, 0.0); /* Retourne l'origine si norme négative */
    }

    /* Générer un angle aléatoire [0, 2π] */
    double theta = 2 * M_PI * (rand() / (double)RAND_MAX);

    /* Convertir en coordonnées cartésiennes */
    double x = norme * cos(theta);
    double y = norme * sin(theta);

    return R2_nouveau(x, y);
}

/*=========================================================*/
/* Calcule la norme d'un vecteur */
double R2_norme(t_R2 p) {
    return sqrt(p.x * p.x + p.y * p.y);
}

/*=========================================================*/
/* Normalise un vecteur (retourne un vecteur unitaire) */
t_R2 R2_normer(t_R2 p) {
    double norme = R2_norme(p);
    if (norme <= 0) {
        return p; /* Retourne l'origine si norme nulle */
    }
    return R2_nouveau(p.x / norme, p.y / norme);
}

/*=========================================================*/
/* Calcule la distance entre deux points */
double R2_distance(t_R2 p1, t_R2 p2) {
    t_R2 diff = R2_diff(p1, p2);
    return R2_norme(diff);
}

/*=========================================================*/
/* Teste l'égalité exacte entre deux points */
int R2_eq(t_R2 p1, t_R2 p2) {
    return (p1.x == p2.x && p1.y == p2.y);
}

/*=========================================================*/
/* Teste la proximité (distance < EPSILON) */
int R2_presque_eq(t_R2 p1, t_R2 p2) {
    return R2_distance(p1, p2) < EPSILON;
}

/*=========================================================*/
/* Somme de deux vecteurs */
t_R2 R2_sum(t_R2 p1, t_R2 p2) {
    return R2_nouveau(p1.x + p2.x, p1.y + p2.y);
}

/*=========================================================*/
/* Différence de deux vecteurs */
t_R2 R2_diff(t_R2 p1, t_R2 p2) {
    return R2_nouveau(p1.x - p2.x, p1.y - p2.y);
}

/*=========================================================*/
/* Dilatation d'un vecteur par un facteur */
t_R2 R2_dilater(t_R2 p, double facteur) {
    return R2_nouveau(facteur * p.x, facteur * p.y);
}

/*=========================================================*/
/* Produit scalaire de deux vecteurs */
double R2_scalaire(t_R2 p1, t_R2 p2) {
    return p1.x * p2.x + p1.y * p2.y;
}

/*=========================================================*/
/* Affiche un vecteur au format (x, y) */
void R2_afficher(t_R2 p) {
    fprintf(stdout, "(%f, %f)", p.x, p.y);
}
/*=========================================================*/