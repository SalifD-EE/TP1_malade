/*=========================================================*/
/* pandemie.c - Programme principal pour INF147 TP1 */
/* Salif Diarra et Léo Bouamrane */
/*=========================================================*/

#include "m_alea_pop.h"
#include "m_personne.h"
#include "m_liste_personnes.h"
#include "m_R2.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*=========================================================*/

/*=========================================================*/
#if (MODE_SIMULATION) == 0 //Peut être modifié dans dans Constante.h
#if 0
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
#endif


#if (MODE_SIMULATION) == 1
int main(void) {
    /* Initialisation du générateur aléatoire */
    srand_sys();

    /* Paramètres de la simulation */
    double hauteur = 0;     /* Hauteur du quartier (mètres) */
    double largeur = 0;     /* Largeur du quartier (mètres) */
    int nb_personnes = 0;
    double prop_initiale = 0;
    int intervalle_affich = 0; /* Afficher toutes les 24 heures */
    int nb_simulations = 0;
    char prefixe[12];
    char nom_fichier[256];
    nom_fichier[0] = '\0';
    char sim_courante[sizeof(int) * 4];

    FILE* config;
    FILE* log;

    /* Ouverture du fichier de configuration */
    config = fopen("config_simulations.txt", "r");
    if (!config) {
        printf("Erreur : impossible d'ouvrir config_simulations.txt\n");
        return EXIT_FAILURE;
    }

    /* Lecture des paramètres de simulation*/
    if (fscanf(config, "%d %s", &nb_simulations, prefixe) != 2) {
        printf("Erreur : echec de la lecture des parametres du fichier.\n");
        fclose(config);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < nb_simulations; ++i) {
        printf("\nExecution de la simulation %d ...\n", i);
        
        /* Lecture des conditions initiales */
        if (fscanf(config, "%lf %lf %d %lf %d",
            &hauteur,
            &largeur,
            &nb_personnes,
            &prop_initiale,
            &intervalle_affich) != 5) {
            printf("Erreur : echec de la lecture des parametres de simulation.\n");
            fclose(config);
            return 1;
        }
        
        /*Création du fichier texte*/
        sprintf(sim_courante, "%d", i);
        strcpy(nom_fichier, prefixe);
        strcat(nom_fichier, sim_courante);
        strcat(nom_fichier, ".txt");
        
        log = fopen(nom_fichier, "w");
        if (!log) {
            printf("Erreur : impossible d'ouvrir le fichier log.%s\n", nom_fichier);
            fclose(config);
            return 1;
        }

        /* Début de la simulation */
        simuler_pandemie(largeur, hauteur, nb_personnes, prop_initiale, intervalle_affich, log);

        fclose(log);

        printf("\nFin la simulation %d\n", i);
    }

    return EXIT_SUCCESS;
}

#endif

/*=========================================================*/