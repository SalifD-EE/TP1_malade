
/*=========================================================*/
/* Module m_personne pour INF147 - Été 2025 */
/*=========================================================*/
#include "m_personnes.h"
#include "m_alea_pop.h"
#include "m_liste_personnes.h"
#include "m_R2.h"
#include "m_R3.h"
#include "Constante.h"



#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*=========================================================*/


/* prob. de décès  d'une infection virale respiratoire
   selon 7 groupes d'age au canada[0-29, 30-39, 40-49, 50-59, 60-69, 70-79, >=80] */

const double PROB_DECES[7] = { 0.001, 0.002, 0.005, 0.017, 0.059, 0.182, 0.736 };
/* Initialisation d'une personne */
t_personne init_personne(double largeur, double hauteur, double proportion_confinement) {
    t_personne p;

    /* État initial */
    p.etat = SAIN;

    /* Position aléatoire dans le quartier [0, largeur] x [0, hauteur] */
    p.position.x = randf() * largeur;
    p.position.y = randf() * hauteur;

    /* Vitesse aléatoire */
    double norme_vit = NORME_VIT_MIN + randf() * (NORME_VIT_MAX - NORME_VIT_MIN);
    double angle = randf() * 2 * M_PI; /* Angle entre [0, 2π] */
    p.vitesse.x = norme_vit * cos(angle);
    p.vitesse.y = norme_vit * sin(angle);

    /* Âge selon la distribution canadienne */
    p.age = rand_age_canada();

    /* Probabilité de déplacement */
    if (randf() < proportion_confinement) {
        p.prob_deplacer = randf() * PROB_BASSE_MAX; /* Probabilité basse */
    }
    else {
        p.prob_deplacer = PROB_HAUTE_MIN + randf() * (1.0 - PROB_HAUTE_MIN); /* Probabilité haute */
    }

    /* Initialisation des compteurs */
    p.nb_infections = 0;
    p.cause_infections = 0;
    p.hrs_maladie = 0;

    /* Probabilité d'infection initiale */
    p.prob_infection = PROB_INFECTION;

    /* Probabilité de décès selon l'âge */
    int idx_age;
    if (p.age <= AGE_JEUNE) {
        idx_age = 0;
    }
    else {
        idx_age = 1 + (p.age - AGE_JEUNE - 1) / INC_CAT_AGE;
        if (idx_age > 6) idx_age = 6; /* Dernière tranche pour >=80 ans */
    }
    p.prob_mort = PROB_DECES[idx_age];

    return p;
}

/*=========================================================*/

t_R2 get_position(const t_personne* lui) {
    return lui->position;
}

t_R2 get_vitesse(const t_personne* lui) {
    return lui->vitesse;
}

t_etat get_etat(const t_personne* lui) {
    return lui->etat;
}

int get_age(const t_personne* lui) {
    return lui->age;
}

double get_prob_infection(const t_personne* lui) {
    if (lui->etat != MALADE) return 0.0;
    return lui->prob_infection;
}

double get_prob_deplacement(const t_personne* lui) {
    return lui->prob_deplacer;
}

double get_prob_mort(const t_personne* lui) {
    return lui->prob_mort;
}

int get_nb_infection(const t_personne* lui) {
    return lui->nb_infections;
}

int get_cause_infections(const t_personne* lui) {
    return lui->cause_infections;
}

int get_hrs_maladie(const t_personne* lui) {
    if (lui->etat != MALADE) return 0;
    return lui->hrs_maladie;
}

/*=========================================================*/
/* Détermine si une personne meurt ou se rétablit */
int determiner_mort_ou_retabli( const t_personne* lui) {
    if (lui->etat != MALADE | lui->hrs_maladie < NB_HRS_MALADIE ) {
        return 0;
    }

    if (randf() < lui->prob_mort){
        return 1; /* Mort */
    }
    else {
        return 2; /* Rétabli */
    }
}

/*=========================================================*/
/* Calcule la distance entre deux personnes */
double distance_personnes(const t_personne* p1, const t_personne* p2) {
    double dx = p1->position.x - p2->position.x;
    double dy = p1->position.y - p2->position.y;
    return sqrt(dx * dx + dy * dy);
}

/*=========================================================*/
/* Affiche l'état d'une personne pour débogage */
void afficher_personne(const t_personne* lui) {
    printf("Personne: Etat=%s, Position=(%.2f, %.2f), Vitesse=(%.2f, %.2f), Age=%d\n",
        lui->etat == SAIN ? "SAIN" : (lui->etat == MALADE ? "MALADE" : "MORT"),
        lui->position.x, lui->position.y, lui->vitesse.x, lui->vitesse.y, lui->age);
    printf("  Prob. Deplacement=%.2f, Prob. Infection=%.2f, Prob. Mort=%.2f, Infections=%d, Causes=%d, Hrs Maladie=%d\n",
        lui->prob_deplacer, lui->prob_infection, lui->prob_mort, lui->nb_infections, lui->cause_infections, lui->hrs_maladie);
}

/*=========================================================*/

int deplacer_personne(t_personne* personne, double largeur, double hauteur) {
    if (personne->etat == MORT | randf() > personne->prob_deplacer) {
        return 0; /* Pas de déplacement */
    }

    /* Nouvelle position : position += vitesse */
    personne->position.x += personne->vitesse.x;
    personne->position.y += personne->vitesse.y;

    /* Vérification des limites du quartier */
    if (personne->position.x < 0 | personne->position.x >= largeur) {
        personne->vitesse.x = -personne->vitesse.x; /* Inverse vitesse */
        personne->position.x = fmax(0, fmin(largeur, personne->position.x)); 
    }
    if (personne->position.y < 0 | personne->position.y >= hauteur) {
        personne->vitesse.y = -personne->vitesse.y; /* Inverse vitesse */
        personne->position.y = fmax(0, fmin(hauteur, personne->position.y)); /* Clamp */
    }

    return 1; /* Déplacé */
}

/*=========================================================*/
int inverser_les_vitesses(t_personne* lui, t_personne* autre) {
    if (lui->etat == MORT || autre->etat == MORT | distance_personnes(lui, autre) > DISTANCE_CONTACT) {
        return 0; /* Pas de changement */
    }

    /* Générer un angle aléatoire pour des directions opposées */
    double angle = randf() * 2 * M_PI;
    double norme_vit = NORME_VIT_MIN + randf() * (NORME_VIT_MAX - NORME_VIT_MIN);

    /* Vitesse pour la première personne */
    lui->vitesse.x = norme_vit * cos(angle);
    lui->vitesse.y = norme_vit * sin(angle);

    /* Vitesse opposée pour l'autre personne */
    autre->vitesse.x = -norme_vit * cos(angle);
    autre->vitesse.y = -norme_vit * sin(angle);

    return 1; /* Vitesses changées */
}

/*=========================================================*/
/*Fonction qui modifie les parametres de la personne en fonction de son etat de sante  */

void modifier_etat_personne(t_personne* lui, t_etat etat, double proportion_confinement) {
    lui->etat = etat;

    if (etat == MALADE) {
        lui->nb_infections++;
        lui->hrs_maladie = 0;
        lui->prob_infection = PROB_INFECTION / (REDUCTEUR_INFECTION * lui->nb_infections);
        /* Redéfinit probabilité de déplacement  */
        lui->prob_deplacer = modifier_prob_deplacer(lui, proportion_confinement);
    }
    else if (etat == SAIN) {
        lui->hrs_maladie = 0;
    }
}

/*=========================================================*/
double modifier_prob_deplacer(t_personne* lui, double proportion_confinement) {
    if (randf() < proportion_confinement) {
        lui->prob_deplacer = randf() * PROB_BASSE_MAX; /* Probabilité basse */
    }
    else {
        lui->prob_deplacer = PROB_HAUTE_MIN + randf() * (1.0 - PROB_HAUTE_MIN); /* Probabilité haute */
    }
    return lui->prob_deplacer;
}

/*=========================================================*/
/*Fonction qui augmente le temps de maladie d une personne malade a chaque heure*/

int inc_hrs_maladie(t_personne* lui) {
    if (lui->etat != MALADE) {
        return 0;
    }
    lui->hrs_maladie++;
    return lui->hrs_maladie;
}

/*=========================================================*/
int inc_cause_infections(t_personne* lui) {
    lui->cause_infections++;
    return lui->cause_infections;
}
/*=========================================================*/