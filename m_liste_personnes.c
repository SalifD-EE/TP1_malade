
/*=========================================================*/
/* Module m_liste_personnes pour INF147 - �t� 2025 */
/*=========================================================*/
#include "m_liste_personnes.h"
#include "m_alea_pop.h"
#include "m_personnes.h"
#include "m_R3.h"
#include "m_R2.h"
#include "Constante.h"




#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*=========================================================*/
t_liste_personnes creer_liste_personnes(int taille) {
    t_liste_personnes liste;
    liste.liste = (t_personne*)malloc(taille * sizeof(t_personne));
    liste.taille = taille;
    liste.nb_personnes = 0;
    liste.nb_sains = 0;
    liste.nb_malades = 0;
    liste.nb_morts = 0;
    liste.prop_confine = PROP_CONFINEMENT;
    return liste;
}


/*=========================================================*/
int ajouter_des_personnes(t_liste_personnes* liste, int nb, double largeur, double hauteur, double prop_confinement) {
    int i, ajoutes = 0;
    for (i = 0; i < nb && liste->nb_personnes < liste->taille; i++) {
        liste->liste[liste->nb_personnes] = init_personne(largeur, hauteur, prop_confinement);
        liste->nb_personnes++;
        liste->nb_sains++;
        ajoutes++;
    }
    return ajoutes;
}

/*=========================================================*/
/*fonction permettant de creer le premier patient atteint de la maladie sans infection par contact avec une personne infecte*/

int creer_patient_zero(t_liste_personnes* liste) {
    if (liste->nb_sains == 0 || liste->nb_malades > 0) {
        return 0;
    }
    int idx = randi(liste->nb_personnes) - 1;
    if (get_etat(&liste->liste[idx]) == SAIN) {
        modifier_etat_personne(&liste->liste[idx], MALADE, 0.75); /* Confinement par d�faut */
        liste->nb_sains--;
        liste->nb_malades++;
        return 1;
    }
    return 0;
}

/*=========================================================*/
void liberer_liste(t_liste_personnes* liste) {
    free(liste->liste);
    liste->liste = NULL;
    liste->taille = 0;
    liste->nb_personnes = 0;
    liste->nb_sains = 0;
    liste->nb_malades = 0;
    liste->nb_morts = 0;
    liste->prop_confine = 0;
}

/*=========================================================*/
/*cette fonction permet de verifier su la personne dans la liste a un indice donnee est morte.
Si c est le cas la personne est deplace a la fin de la liste*/

int deplacer_un_mort(t_liste_personnes* liste, int indice) {
    if (indice < 0 || indice >= liste->nb_personnes || get_etat(&liste->liste[indice]) != MORT) {
        return 0;
    }
    int dernier_vivant = liste->nb_personnes - liste->nb_morts;
    if (indice < dernier_vivant) {
        t_personne temp = liste->liste[indice];
        liste->liste[indice] = liste->liste[dernier_vivant];
        liste->liste[dernier_vivant] = temp;
        return 1;
    }
    return 0;
}

/*=========================================================*/
int assurer_temps_maladie(t_liste_personnes* liste) {
    int nb_termines = 0;
    for (int i = 0; i < liste->nb_personnes - liste->nb_morts; i++) {
        if (get_etat(&liste->liste[i]) == MALADE) {
            inc_hrs_maladie(&liste->liste[i]);
            if (get_hrs_maladie(&liste->liste[i]) >= NB_HRS_MALADIE) {
                nb_termines++;
            }
        }
    }
    return nb_termines;
}

/*=========================================================*/
/*fonction qui permet de decider apres qu une personne ai contracter la maladie,
si la personne decede ou guerit apres que le temps maximal de maladie sois atteint.*/

int terminer_maladie(t_liste_personnes* liste) {
    int nb_morts = 0;
    for (int i = 0; i < liste->nb_personnes - liste->nb_morts; i++) {
        int resultat = determiner_mort_ou_retabli(&liste->liste[i]);
        if (resultat == 1) { /* Mort */
            modifier_etat_personne(&liste->liste[i], MORT, liste->prop_confine);
            liste->nb_malades--;
            liste->nb_morts++;
            deplacer_un_mort(liste, i);


            nb_morts++;

        }
        else if (resultat == 2) { /* R�tabli */
            modifier_etat_personne(&liste->liste[i], SAIN, liste->prop_confine);
            liste->nb_malades--;
            liste->nb_sains++;
        }
    }
    return nb_morts;
}

/*=========================================================*/
/*Fonction repertoriant le nombre de personne vivante etant deplace chaque heure sur la surface definie initialement*/

int deplacer_les_personnes(t_liste_personnes* liste, double largeur, double hauteur) {
    int nb_deplaces = 0;
    for (int i = 0; i < liste->nb_personnes - liste->nb_morts; i++) {
        nb_deplaces += deplacer_personne(&liste->liste[i], largeur, hauteur);
    }
    return nb_deplaces;
}

/*=========================================================*/
/*Fonction qui determine la transmission de la maladie en cas de contact entre une personne saine et infecte.*/

int traiter_contacts(t_liste_personnes* liste) {
    int nb_infections = 0;
    for (int i = 0; i < liste->nb_personnes - liste->nb_morts; i++) {
        for (int j = i + 1; j < liste->nb_personnes - liste->nb_morts; j++) {
            if (distance_personnes(&liste->liste[i], &liste->liste[j]) <= DISTANCE_CONTACT) {
                t_personne* p1 = &liste->liste[i];
                t_personne* p2 = &liste->liste[j];
                if (get_etat(p1) == MALADE && get_hrs_maladie(p1) >= NB_HRS_TRANSMISSION && get_etat(p2) == SAIN) {
                    if (randf() < get_prob_infection(p1)) {
                        modifier_etat_personne(p2, MALADE, liste->prop_confine);
                        inc_cause_infections(p1);
                        liste->nb_sains--;
                        liste->nb_malades++;
                        nb_infections++;
                    }
                    inverser_les_vitesses(p1, p2);
                }
                else if (get_etat(p2) == MALADE && get_hrs_maladie(p2) >= NB_HRS_TRANSMISSION && get_etat(p1) == SAIN) {
                    if (randf() < get_prob_infection(p2)) {
                        modifier_etat_personne(p1, MALADE, liste->prop_confine);
                        inc_cause_infections(p2);
                        liste->nb_sains--;
                        liste->nb_malades++;
                        nb_infections++;
                    }
                    inverser_les_vitesses(p1, p2);
                }
            }
        }
    }
    return nb_infections;
}

/*=========================================================*/
int simuler_une_heure_pandemie(t_liste_personnes* liste, double largeur, double hauteur) {
    /* S�quence :
       1. D�placer les personnes vivantes
       2. Traiter les contacts et infections
       3. Mettre � jour le temps de maladie
       4. G�rer les r�tablissements et d�c�s
    */
    deplacer_les_personnes(liste, largeur, hauteur);
    traiter_contacts(liste);
    assurer_temps_maladie(liste);
    terminer_maladie(liste);
    return get_nb_malades(liste);
}

/*=========================================================*/
int get_nb_personnes(const t_liste_personnes* liste) {
    return liste->nb_personnes;
}

/*=========================================================*/
int get_nb_malades(const t_liste_personnes* liste) {
    return liste->nb_malades;
}

/*=========================================================*/
int get_nb_sains(const t_liste_personnes* liste) {
    return liste->nb_sains;
}

/*=========================================================*/
int get_nb_morts(const t_liste_personnes* liste) {
    return liste->nb_morts;
}

/*=========================================================*/
double get_confinement(const t_liste_personnes* liste) {
    return liste->prop_confine;
}

/*=========================================================*/
double get_prop_malades(const t_liste_personnes* liste) {
    if (liste->nb_personnes > 0) {
        return (double)liste->nb_malades / liste->nb_personnes;
    }

    return -1;
}

/*=========================================================*/
double get_prop_morts(const t_liste_personnes* liste) {
    if (liste->nb_personnes > 0) {
        return (double)liste->nb_morts / liste->nb_personnes;
    }

    return -1;
}

/*=========================================================*/

//void afficher_liste_personnes(const t_liste_personnes* liste) {
//    printf("\n\nListe de personnes (%d personnes, %d sains, %d malades, %d morts):\n",
//        liste->nb_personnes, liste->nb_sains, liste->nb_malades, liste->nb_morts);
//    for (int i = 0; i < liste->nb_personnes; i++) {
//        printf("Personne %d: ", i);
//        afficher_personne(&liste->liste[i]);
//    }
//}

/*=========================================================*/
void afficher_liste_personnes(const t_liste_personnes* liste) {
    printf("\n\nListe de personnes (%d personnes, %d sains, %d malades, %d morts, prop_confine=%.2f):\n",
        liste->nb_personnes, liste->nb_sains, liste->nb_malades, liste->nb_morts, liste->prop_confine);
    for (int i = 0; i < liste->nb_personnes; i++) {
        printf("Personne %d: ", i);
        afficher_personne(&liste->liste[i]);
    }
}


/*=========================================================*/
double modifier_confinement(t_liste_personnes* liste, double nouvelle_prop) {
    double ancienne_prop = liste->prop_confine;
    liste->prop_confine = nouvelle_prop; // Mise � jour de prop_confine
    double somme_prob_deplacer = 0.0;
    int nb_vivants = liste->nb_personnes - liste->nb_morts;

    for (int i = 0; i < nb_vivants; i++) {
        t_personne* personne = &liste->liste[i];
        // Si la proportion augmente, ajuster pour tous
        // Si la proportion diminue, ajuster seulement pour les sains
        if (nouvelle_prop > ancienne_prop || get_etat(personne) == SAIN) {
            modifier_prob_deplacer(personne, nouvelle_prop);
        }
        somme_prob_deplacer += get_prob_deplacement(personne);
    }

    // Retourner la probabilit� moyenne de d�placement des vivants
    return (nb_vivants > 0) ? somme_prob_deplacer / nb_vivants : 0.0;
}

/*=========================================================*/

double calculer_prob_moyenne(const t_liste_personnes* liste, const char* type) {
    double somme = 0.0;
    int nb_vivants = liste->nb_personnes - liste->nb_morts;
    for (int i = 0; i < nb_vivants; i++) {
        if (strcmp(type, "deplacer") == 0) {
            somme += get_prob_deplacer(&liste->liste[i]);
        }
        else if (strcmp(type, "infection") == 0) {
            somme += get_prob_infection(&liste->liste[i]);
        }
        else if (strcmp(type, "mort") == 0) {
            somme += get_prob_mort(&liste->liste[i]);
        }
    }
    return (nb_vivants > 0) ? somme / nb_vivants : 0.0;
}



int simuler_pandemie(double hauteur, double largeur, int population, double prop_initial, int periode_affichage, FILE* log) {
    /* Initialisation de la liste */
    t_liste_personnes liste = creer_liste_personnes(population);
    ajouter_des_personnes(&liste, population, largeur, hauteur, prop_initial);
    creer_patient_zero(&liste);
    int heures  = 0;

    /* �criture de la premi�re ligne du log */
    fprintf(log, "%.2f %.2f %d %.2f %d\n", hauteur, largeur, population, prop_initial, periode_affichage);

    /* Boucle de simulation */
    while (get_nb_malades(&liste) > 0 && get_nb_morts(&liste) < population) {
        simuler_une_heure_pandemie(&liste, largeur, hauteur);

        /* Strat�gie de confinement : faible -> forte si malades > PROP_MALADES_CHANGEMENT */
        if (get_prop_malades(&liste) > PROP_MALADES_CHANGEMENT && get_prop_morts(&liste) > PROP_MORTS_CHANGEMENT) {
            modifier_confinement(&liste, NOUVELLE_PROP);

        }

        /* �criture p�riodique dans le log */
        if (heures % periode_affichage == 0) {
            fprintf(log, "%d %d %d %d %.3f %.3f %.3f\n",
                heures, get_nb_sains(&liste), get_nb_malades(&liste), get_nb_morts(&liste),
                calculer_prob_moyenne(&liste, "deplacer"),
                calculer_prob_moyenne(&liste, "infection"),
                calculer_prob_moyenne(&liste, "mort"));
        }

    }

    /* �criture de la derni�re ligne du log */
    fprintf(log, "%d %d %d %d %.3f %.3f %.3f\n",
        heures, get_nb_sains(&liste), get_nb_malades(&liste), get_nb_morts(&liste),
        calculer_prob_moyenne(&liste, "deplacer"),
        calculer_prob_moyenne(&liste, "infection"),
        calculer_prob_moyenne(&liste, "mort"));

    /* Lib�ration de la m�moire */
    liberer_liste(&liste);

}

