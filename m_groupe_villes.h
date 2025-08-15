/*=========================================================*/
/* Module m_groupe_villes pour INF147 - �t� 2025 */

/* Salif Diarra et L�o Bouamrane */

/*=========================================================*/

#ifndef M_GROUPE_VILLES_H
#define M_GROUPE_VILLES_H

#include "m_ville.h" // Inclusion pour t_ville

typedef struct {
    t_ville* tab_villes; // Tableau dynamique des t_ville du groupe
    int taille_tab;      // La taille du tableau pr�c�dent
    int nb_villes;       // Nombre actuel de villes dans le groupe

    FILE* log_villes;    // Fichier d��criture des comptes globaux du groupe

    // Ajout de membres pour les stats (exemples, � adapter)
    int total_malades;   // Nombre total de malades
    int total_retablis;  // Nombre total de r�tablis
    int total_vivants;   // Nombre total de vivants
    int total_morts;     // Nombre total de morts
} t_groupe_villes;


/******************************************************************************/
/*                    D�CLARATION DES FONCTIONS                               */
/******************************************************************************/

// Constructeur du groupe de villes
t_groupe_villes init_groupe_villes(FILE* config, const char* nom_log);

// Simulation de la pand�mie pour le groupe
int simuler_pandemie_groupe_villes(t_groupe_villes* gr, int nb_heures_max, int periode_affich);

// Destructeur du groupe de villes
void detruire_groupe_villes(t_groupe_villes* gr);

/*=========================================================*/
/* INFORMATRICES
/*=========================================================*/

int get_nb_total_malades_groupe(t_groupe_villes* gr);
int get_nb_total_retablis_groupe(t_groupe_villes* gr);
int get_nb_total_vivants_groupe(t_groupe_villes* gr);
int get_nb_total_morts_groupe(t_groupe_villes* gr);

#endif 