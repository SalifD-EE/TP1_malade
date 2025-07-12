/*=========================================================*/
/* pandemie.c - Programme principal pour INF147 TP1 */

/*=========================================================*/

#include "m_alea_pop.h"
#include "m_personnes.h"
#include "m_liste_personnes.h"
#include "m_R2.h"
#include "m_R3.h"


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*=========================================================*/

/*=========================================================*/
int main(void) {
    /* Initialisation du générateur aléatoire */
    srand_sys();

    /* Paramètres de la simulation */
    const int TAILLE_LISTE = 401;     /* Capacité initiale */
    const int NB_PERSONNES = 400;     /* Nombre de personnes */
    const double LARGEUR = 100;     /* Largeur du quartier (mètres) */
    const double HAUTEUR = 100;     /* Hauteur du quartier (mètres) */
    const int INTERVALLE_AFFICHAGE = 100; /* Afficher toutes les 24 heures */

    /* Statistiques */
    int heures = 0;
    int total_infections = 0;
    int max_infections_heure = 0;
    int min_infections_heure = 0; 
    int total_morts = 0;
    int max_morts_heure = 0;
    int min_morts_heure = 0;

    /* Création de la liste */
    t_liste_personnes liste = creer_liste_personnes(TAILLE_LISTE);
    printf("Liste creee avec une capacite de %d personnes.\n", TAILLE_LISTE);

    /* Ajout de personnes */
    int nb_ajoutes = ajouter_des_personnes(&liste, NB_PERSONNES, LARGEUR, HAUTEUR, PROP_CONFINEMENT);
    printf("%d personnes ajoutees.\n", nb_ajoutes);
   // afficher_liste_personnes(&liste);

    /* Introduction du patient zéro */
    if (creer_patient_zero(&liste)) {
        printf("Patient zero introduit avec succes.\n");
    }
    else {
        printf("Echec de l'introduction du patient zero.\n");
        liberer_liste(&liste);
        return EXIT_FAILURE;
    }
   //afficher_liste_personnes(&liste);
 
    /* Boucle de simulation */
    while (get_nb_malades(&liste) > 0 ) {
        ++heures;
        int infections_heure = traiter_contacts(&liste);
        int morts_heure = terminer_maladie(&liste); 

        simuler_une_heure_pandemie(&liste, LARGEUR, HAUTEUR);
       

        /* Mise à jour des statistiques */
        total_infections += infections_heure;
        max_infections_heure = infections_heure > max_infections_heure ? infections_heure : max_infections_heure;
        min_infections_heure = infections_heure < min_infections_heure ? infections_heure : min_infections_heure;
      
        if (get_prop_malades(&liste) > 0.05 && get_prop_morts(&liste) > 0.05) {
            modifier_confinement(&liste, NOUVELLE_PROP);
        }

         /*Affichage périodique*/
        if (heures % INTERVALLE_AFFICHAGE == 0) {
            printf("\nHeure %d: Sains=%d, Malades=%d, Morts=%d, Infections=%d, Confinement=%.2f \n",
                heures, get_nb_sains(&liste), get_nb_malades(&liste), get_nb_morts(&liste),
                infections_heure, get_confinement(&liste));
         max_morts_heure = get_nb_morts(&liste) > max_morts_heure ? get_nb_morts(&liste) : max_morts_heure;
        }
    }
     total_morts = get_nb_morts(&liste);

    /* Affichage des résultats finaux */
    printf("\nSimulation terminee apres %d heures.\n", heures);
    printf("Statistiques finales :\n");
    printf("  Total infections : %d\n", total_infections);
    printf("  Max infections/heure : %d\n", max_infections_heure);
    printf("  Min infections/heure : %d\n", min_infections_heure);
    printf("  Moyenne infections/heure : %.2f\n", heures > 0 ? (double)total_infections / heures : 0.000);
    printf("  Total morts : %d\n", total_morts);
    printf("  Max morts/heure : %d\n", max_morts_heure);
    printf("  Min morts/heure : %d\n", min_morts_heure);
    printf("  Moyenne morts/heure : %.10f\n", heures > 0 ? (double)total_morts / heures : 0.000);
    //afficher_liste_personnes(&liste);

    /* Libération de la mémoire */
    liberer_liste(&liste);

    return EXIT_SUCCESS;
}
/*=========================================================*/