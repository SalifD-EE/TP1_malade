/*=========================================================*/
/* Module m_groupe_villes pour INF147 - Été 2025 */

/* Salif Diarra et Léo Bouamrane */

/*=========================================================*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "m_groupe_villes.h"
#include "m_ensemble_noms.h" 
#include "m_ville.h"     


/******************************************************************************/
/*                    DÉCLARATION DES FONCTIONS                               */
/******************************************************************************/

// Constructeur

t_groupe_villes init_groupe_villes(FILE* config, const char* nom_log) {
    t_groupe_villes gr;

    // Initialisation explicite de tous les champs
    gr.tab_villes = NULL;
    gr.taille_tab = 0;
    gr.nb_villes = 0;
    gr.log_villes = NULL;

    gr.total_malades = 0;
    gr.total_retablis = 0;
    gr.total_vivants = 0;
    gr.total_morts = 0;

    int nb_villes;

    fscanf(config, "%d", &nb_villes);
    gr.taille_tab = nb_villes;
    gr.nb_villes = nb_villes;

    gr.tab_villes = malloc(nb_villes * sizeof(t_ville));
    if (!gr.tab_villes) {
        fprintf(stderr, "Erreur allocation tab_villes\n");
        exit(EXIT_FAILURE);
    }

    // Initialisation de l’ensemble de noms
    init_ensemble_noms_villes(nb_villes);

    for (int i = 0; i < nb_villes; i++) {
        char nom[100];
        int h, l, population, transit;
        double confinement, emigr;

        fscanf(config, "%s %d %d %d %lf %lf %d",
            nom, &h, &l, &population, &confinement, &emigr, &transit);

        gr.tab_villes[i] = init_ville(nom, l, h, population, confinement, emigr, transit);

        ajouter_nom_ville(nom);
        inoculer_ville(gr.tab_villes[i]);
    }

    // ouvrir le fichier log
    gr.log_villes = fopen(nom_log, "w");
    if (!gr.log_villes) {
        fprintf(stderr, "Erreur ouverture log %s\n", nom_log);
        exit(EXIT_FAILURE);
    }

    return gr;
}




static void simuler_une_heure_groupe_villes(t_groupe_villes* gr) {
  
    for (int i = 0; i < gr->nb_villes - 1; i++) {
        transferer_des_migrants_entre_villes(gr->tab_villes[i], gr->tab_villes[i + 1]);
    }

    for (int i = 0; i < gr->nb_villes; i++) {
        obtenir_des_personnes_ville(gr->tab_villes[i]);
        obtenir_des_migrants_ville(gr->tab_villes[i]);
        simuler_une_heure_pandemie_ville(gr->tab_villes[i]);
    }
}


// Simulation de la pandémie
int simuler_pandemie_groupe_villes(t_groupe_villes* gr, int nb_heures_max, int periode_affich) {
    int heures = 0;
    while (get_nb_total_malades_groupe(gr) > 0 && heures < nb_heures_max) {
       
        if (heures % periode_affich == 0) {
            for (int i = 0; i < gr->nb_villes; i++) {
                ecrire_logfile_ville(gr->tab_villes[i]);
            }
          
            fprintf(gr->log_villes, "Heure %d: Malades %d, Rétablis %d, Vivants %d, Morts %d\n",
                heures, gr->total_malades, gr->total_retablis, gr->total_vivants, gr->total_morts);
        }

        // 2. Simuler une heure
        simuler_une_heure_groupe_villes(gr);

       
        gr->total_malades = get_nb_total_malades_groupe(gr);
        gr->total_retablis = get_nb_total_retablis_groupe(gr);
        gr->total_vivants = get_nb_total_vivants_groupe(gr);
        gr->total_morts = get_nb_total_morts_groupe(gr);

        heures++;
    }
    return heures;
}

//int simuler_pandemie_groupe_villes(t_groupe_villes* gr, int nb_heures_max, int periode_affich) {
//    int heures = 0;
//    while (get_nb_total_malades_groupe(gr) > 0 && heures < nb_heures_max) {
//        // 1. Affichage si période atteinte
//        if (heures % periode_affich == 0) {
//            for (int i = 0; i < gr->nb_villes; i++) {
//                ecrire_logfile_ville(gr->tab_villes[i]);
//            }
//            // Écrire log du groupe (à implémenter selon besoins)
//            fprintf(gr->log_villes, "Heure %d: Malades %d, Rétablis %d, Vivants %d, Morts %d\n",
//                heures, gr->total_malades, gr->total_retablis, gr->total_vivants, gr->total_morts);
//        }
//
//        // 2. Simuler une heure
//        simuler_une_heure_groupe_villes(gr);
//
//        // 3. Interroger les informatrices (mettre à jour les stats)
//        gr->total_malades = get_nb_total_malades_groupe(gr);
//        gr->total_retablis = get_nb_total_retablis_groupe(gr);
//        gr->total_vivants = get_nb_total_vivants_groupe(gr);
//        gr->total_morts = get_nb_total_morts_groupe(gr);
//
//        heures++;
//    }
//    return heures;
//}



void detruire_groupe_villes(t_groupe_villes* gr) {
    for (int i = 0; i < gr->nb_villes; i++) {
        detruire_ville(gr->tab_villes[i]);
    }
    free(gr->tab_villes);
    gr->tab_villes = NULL;
    gr->nb_villes = 0;
    gr->taille_tab = 0;

    if (gr->log_villes != NULL) {
        fclose(gr->log_villes);
        gr->log_villes = NULL;
    }
}


/*=========================================================*/
/* INFORMATRICES
/*=========================================================*/

int get_nb_total_malades_groupe(t_groupe_villes* gr) {
    int malades = 0;
    for (int i = 0; i < gr->nb_villes; i++) {
        malades += gr->tab_villes[i].nb_infectes; 
    }
    return malades;
}

int get_nb_total_retablis_groupe(t_groupe_villes* gr) { 
    int retablis = 0;
    for (int i = 0; i < gr->nb_villes; i++) {
        retablis += gr->tab_villes[i].nb_gueris;
    }
    return retablis;
}

int get_nb_total_vivants_groupe(t_groupe_villes* gr) {
    int vivants = 0;
    for (int i = 0; i < gr->nb_villes; i++) {
        vivants += gr->tab_villes[i].taille_population + gr->tab_villes[i].nb_migrants - gr->tab_villes[i].nb_morts;
    }
    return vivants;
}

int get_nb_total_morts_groupe(t_groupe_villes* gr) {
    int morts = 0;
    for (int i = 0; i < gr->nb_villes; i++) {
        morts += gr->tab_villes[i].nb_morts;
    }
    return morts;
}

// Informatrices
//int get_nb_total_malades_groupe(t_groupe_villes* gr) {
//    int total = 0;
//    for (int i = 0; i < gr->nb_villes; i++) {
//        total += gr->tab_villes[i].nb_infectes; 
//    }
//    return total;
//}
//
//int get_nb_total_retablis_groupe(t_groupe_villes* gr) {
//    int total = 0;
//    for (int i = 0; i < gr->nb_villes; i++) {
//        total += gr->tab_villes[i].nb_gueris;
//    }
//    return total;
//}
//
//int get_nb_total_vivants_groupe(t_groupe_villes* gr) {
//    int total = 0;
//    for (int i = 0; i < gr->nb_villes; i++) {
//        total += gr->tab_villes[i].taille_population + gr->tab_villes[i].nb_migrants - gr->tab_villes[i].nb_morts;
//    }
//    return total;
//}
//
//int get_nb_total_morts_groupe(t_groupe_villes* gr) {
//    int total = 0;
//    for (int i = 0; i < gr->nb_villes; i++) {
//        total += gr->tab_villes[i].nb_morts;
//    }
//    return total;
//}

