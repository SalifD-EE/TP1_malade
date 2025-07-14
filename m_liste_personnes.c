
/*=========================================================*/
/* Module m_liste_personnes pour INF147 - Été 2025 */
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
    liste.prop_confine = PROP_CONFINEMENT_BAS;
    return liste;
}
/*=========================================================*/
/*=============== FONCTION INFORMATRICES ==================*/
/*=========================================================*/

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
    return (liste->nb_personnes > 0) ? (double)liste->nb_malades / liste->nb_personnes : 0;
}

/*=========================================================*/
double get_prop_morts(const t_liste_personnes* liste) {
    return (liste->nb_personnes > 0) ? (double)liste->nb_morts / liste->nb_personnes : 0;
}

/*=========================================================*/
/*=============== FONCTION MUTATRICES =====================*/
/*=========================================================*/

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
        modifier_etat_personne(&liste->liste[idx], MALADE, 0.75); /* Confinement par défaut */
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
        else if (resultat == 2) { /* Rétabli */
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
int simuler_une_heure_pandemie(t_liste_personnes* liste, double largeur, double hauteur, int* infection_heure, int* morts_heure) {
    /* Séquence :
       1. Déplacer les personnes vivantes
       2. Traiter les contacts et infections
       3. Mettre à jour le temps de maladie
       4. Gérer les rétablissements et décès
    */
    deplacer_les_personnes(liste, largeur, hauteur);
    *infection_heure = traiter_contacts(liste);
    assurer_temps_maladie(liste);
    *morts_heure = terminer_maladie(liste);
    return get_nb_malades(liste);
}

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
    liste->prop_confine = nouvelle_prop; // Mise à jour de prop_confine
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

    // Retourner la probabilité moyenne de déplacement des vivants
    return (nb_vivants > 0) ? somme_prob_deplacer / nb_vivants : 0.0;
}

/*=========================================================*/

double calculer_prob_moyenne(const t_liste_personnes* liste, const char* type) {
    double somme = 0.0;
    int nb_vivants = liste->nb_personnes - liste->nb_morts;
    for (int i = 0; i < nb_vivants; i++) {
        if (strcmp(type, "deplacer") == 0) {
            somme += get_prob_deplacement(&liste->liste[i]);
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

/*=========================================================*/

void simuler_pandemie(double hauteur, double largeur, int population, double prop_initiale, int periode_affichage, FILE* log) {
    int heures = 0;
    int total_infections = 0;
    int max_infections_heure = 0;
    int min_infections_heure = 0;
    int total_morts = 0;
    int max_morts_heure = 0;
    int min_morts_heure = 0;
    int infections_heure = 0;
    int morts_heure = 0;
    double prop_malades = 0;
    double prop_morts = 0;

    /* Initialisation de la liste */
    t_liste_personnes liste = creer_liste_personnes(population);
    ajouter_des_personnes(&liste, population, largeur, hauteur, prop_initiale);
    creer_patient_zero(&liste);

    /* Écriture de la première ligne du log */
    fprintf(log, "Hauteur=%lf, Largeur=%lf, Population=%d, Prop initiale=%lf, Periode=%d\n", hauteur, largeur, population, prop_initiale, periode_affichage);
    fflush(log);

    /* Boucle de simulation */
    while (get_nb_malades(&liste) > 0 && get_nb_morts(&liste) < population) {
        ++heures;
        prop_malades = get_prop_malades(&liste);
        prop_morts = get_prop_morts(&liste);

        simuler_une_heure_pandemie(&liste, largeur, hauteur, &infections_heure, &morts_heure);

        /* Mise à jour des statistiques */
        total_infections += infections_heure;
        max_infections_heure = infections_heure > max_infections_heure ? infections_heure : max_infections_heure;
        min_infections_heure = infections_heure < min_infections_heure ? infections_heure : min_infections_heure;

        /* 
            Stratégie de confinement : plusieurs paliers de confinement en fonction de
            la proportion de malades et de morts
        */
        if (prop_malades > PALIER_MALADES_4) {
            modifier_confinement(&liste, PROP_CONFINEMENT_TOTAL);
        }
        else if (prop_malades > PALIER_MALADES_3) {
            modifier_confinement(&liste, PROP_CONFINEMENT_ELEVE);
        }
        else if (prop_malades > PALIER_MALADES_2) {
            modifier_confinement(&liste, PROP_CONFINEMENT_INTERMEDIAIRE);
        }
        else if (prop_malades > PALIER_MALADES_1) {
            modifier_confinement(&liste, PROP_CONFINEMENT_BAS);
        }
        else {
            modifier_confinement(&liste, 0.00);
        }

        /* Écriture périodique dans le log */
        if (heures % periode_affichage == 0) {
            fprintf(log, "\nHeure=%d: Sains=%d, Malades=%d, Morts=%d, Moy deplacement=%.3f, Moy infection=%.3f, Moy mort=%.3f, Prop confinement=%.3f\n",
                heures, get_nb_sains(&liste), get_nb_malades(&liste), get_nb_morts(&liste),
                calculer_prob_moyenne(&liste, "deplacer"),
                calculer_prob_moyenne(&liste, "infection"),
                calculer_prob_moyenne(&liste, "mort"),
                get_confinement(&liste));
            fflush(log);
            max_morts_heure = morts_heure > max_morts_heure ? morts_heure : max_morts_heure;
        }

    }
    total_morts = get_nb_morts(&liste);

    /* Écriture de la dernière ligne du log */
    fprintf(log, "\nSimulation terminee apres %d heures.\n", heures);
    fprintf(log, "Statistiques finales :\n");
    fprintf(log, "  Total infections : %d\n", total_infections);
    fprintf(log, "  Max infections/heure : %d\n", max_infections_heure);
    fprintf(log, "  Min infections/heure : %d\n", min_infections_heure);
    fprintf(log, "  Moyenne infections/heure : %.5f\n", heures > 0 ? (double)total_infections / heures : 0.000);
    fprintf(log, "  Total morts : %d\n", total_morts);
    fprintf(log, "  Max morts/heure : %d\n", max_morts_heure);
    fprintf(log, "  Min morts/heure : %d\n", min_morts_heure);
    fprintf(log, "  Moyenne morts/heure : %.5f\n", heures > 0 ? (double)total_morts / heures : 0.000);
    fflush(log);

    liberer_liste(&liste);
}

