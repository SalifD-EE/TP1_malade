/*=========================================================*/
/* Pandemic.c - Programme principal pour INF147 TP2 */
/* Salif Diarra et Léo Bouamrane */
/*=========================================================*/

#include "m_alea_pop.h"
#include "m_personne.h"
#include "m_liste_personnes.h"
#include "m_R2.h"
#include "m_matrice_transition.h"
#include "m_groupe_villes.h"
#include "m_ensemble_noms.h"    
#include "m_ville.h" 
#include "m_migrant.h"
#include "m_liste_migrants.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*=========================================================*/

/*=========================================================*/

//Affichage sur la console.
#if (MODE_SIMULATION) == 0 //Peut être modifié dans dans Constante.h

int main(void) {
    /* Initialisation du générateur aléatoire */
    srand_sys();

    /* Paramètres de la simulation */
    const int TAILLE_LISTE = 2501;     /* Capacité initiale */
    const int NB_PERSONNES = 2500;     /* Nombre de personnes */
    const double LARGEUR = 1000;     /* Largeur du quartier (mètres) */
    const double HAUTEUR = 1000;     /* Hauteur du quartier (mètres) */
    const int INTERVALLE_AFFICHAGE = 100; /* Afficher toutes les 100 heures */

    /* Statistiques */
    int heures = 0;
    int total_infections = 0;
    int max_infections_heure = 0;
    int min_infections_heure = 0; 
    double prop_malades = 0;
    double prop_morts = 0;
    int total_morts = 0;
    int max_morts_heure = 0;
    int min_morts_heure = 0;

    /* Création de la liste */
    t_liste_personnes liste = creer_liste_personnes(TAILLE_LISTE);
    printf("Liste creee avec une capacite de %d personnes.\n", TAILLE_LISTE);

    /* Ajout de personnes */
    int nb_ajoutes = ajouter_des_personnes(&liste, NB_PERSONNES, LARGEUR, HAUTEUR, PROP_INITIALE_CONSOLE);
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

        prop_malades = get_prop_malades(&liste);
        prop_morts = get_prop_morts(&liste);

        simuler_une_heure_pandemie(&liste, LARGEUR, HAUTEUR, &infections_heure, &morts_heure);

     
       
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

         /*Affichage périodique*/
        if (heures % INTERVALLE_AFFICHAGE == 0) {
            printf("\nHeure %d: Sains=%d, Malades=%d, Morts=%d, Infections=%d, Confinement=%.2f \n",
                heures, get_nb_sains(&liste), get_nb_malades(&liste), get_nb_morts(&liste),
                infections_heure, get_confinement(&liste));
            max_morts_heure = morts_heure > max_morts_heure ? morts_heure : max_morts_heure;
            min_morts_heure = morts_heure < min_morts_heure ? morts_heure : min_morts_heure;

        }
    }
     total_morts = get_nb_morts(&liste);

    /* Affichage des résultats finaux */
    printf("\nSimulation terminee apres %d heures.\n", heures);
    printf("Statistiques finales :\n");
    printf("  Total infections : %d\n", total_infections);
    printf("  Max infections/heure : %d\n", max_infections_heure);
    printf("  Min infections/heure : %d\n", min_infections_heure);
    printf("  Moyenne infections/heure : %.5f\n", heures > 0 ? (double)total_infections / heures : 0.000);
    printf("  Total morts : %d\n", total_morts);
    printf("  Max morts/heure : %d\n", max_morts_heure);
    printf("  Min morts/heure : %d\n", min_morts_heure);
    printf("  Moyenne morts/heure : %.5f\n", heures > 0 ? (double)total_morts / heures : 0.000);
    //afficher_liste_personnes(&liste);

    /* Libération de la mémoire */
    liberer_liste(&liste);

    return EXIT_SUCCESS;
}
#endif

//Écriture dans un fichier log.
#if (MODE_SIMULATION) == 1
int main(void) {
    /* Initialisation du générateur aléatoire */
    srand_sys();

#endif

//Groupe de villes
#if (MODE_SIMULATION) == 2
int main(void) {
    FILE* mat_transition;
    FILE* config_villes;
    t_groupe_villes gr;
    int nb_villes_test;
    int nb_heures_max = 1000;
    int period_affich = 1;

    //Initialisation du générateur aléatoire
    srand_sys();
    
    //Construction de la matrice de transition
    mat_transition = fopen("mat_transition.txt", "rt");
    
    if (!mat_transition) {
        printf("Erreur : impossible d'ouvrir mat_transition.txt\n");
        return EXIT_FAILURE;
    }

    lire_mat_transition(mat_transition);

    //Ouverture et validation du fichier de config des villes
    config_villes = fopen("config_villes_final.txt", "rt");

    if (!config_villes) {
        printf("Erreur : impossible d'ouvrir config_villes_final.txt\n");
        return EXIT_FAILURE;
    }

    fscanf(config_villes, "%d", &nb_villes_test);
    
    assert(nb_villes_test == get_nb_depart_transition());
    rewind(config_villes);
    
    gr = init_groupe_villes(config_villes, "groupe_villes_final.txt");
    
    //Exécution de la simulation
    simuler_pandemie_groupe_villes(&gr, nb_heures_max, period_affich);

    //Fin de la simulation
    detruire_groupe_villes(&gr);
    detruire_mat_transition();
    detruire_ensemble_noms_villes();
    
    system("pause");
    return EXIT_SUCCESS;

    //Pour déboguer la population de chaque ville
    /*for (int i = 0; i < get_nb_depart_transition(); i++)
    {
        t_liste_personnes pop = gr.tab_villes[i]->population;
        afficher_liste_personnes(&pop);
    }*/
}
#endif
/*=========================================================*/