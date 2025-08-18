/*=========================================================*/
/* Module m_migrant pour INF147 - �t� 2025 */
/* Salif Diarra et L�o Bouamrane */
/*=========================================================*/

#include "m_migrant.h"
#include"m_ensemble_noms.h"
#include"m_personne.h"
#include"m_liste_personnes.h"


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*=========================================================*/

t_migrant init_migrant(const t_personne* src, int ville_dep, int ville_dest, int hrs_trans) {
    t_migrant migrant;
    migrant.voyageur = *src;
    migrant.ville_depart = ville_dep;
    migrant.ville_destination = ville_dest;
    migrant.hrs_transit = hrs_trans;
    return migrant;
}

/*=========================================================*/

void set_transit_migrant(t_migrant* lui, int hrs) {
    lui->hrs_transit = hrs;
}

/*=========================================================*/

int dec_hrs_transit_migrant(t_migrant* lui) {
    if (lui->hrs_transit > 0) {
        lui->hrs_transit--;
    }
    return lui->hrs_transit;
}

/*=========================================================*/

int get_hrs_transit(const t_migrant* lui) {
    return lui->hrs_transit;
}

int get_destination_migrant(const t_migrant* lui) {
    return lui->ville_destination;
}

/*=========================================================*/

int get_depart_migrant(const t_migrant* lui) {
    return lui->ville_depart;
}


/*=========================================================*/

int est_vivant_migrant(t_migrant* lui) {
    return get_etat(&lui->voyageur) != 2;
}


/*=========================================================*/

int est_malade_migrant(const t_migrant* lui) {
    return get_etat(&lui->voyageur) == 1;
}


/*=========================================================*/

int inc_hrs_maladie_migrant(t_migrant* lui) {
    int resultat;
    
    if (lui != NULL && est_malade_migrant(lui) == 1) {
        
        // Si le nombre d'heures de maladie atteint NB_HRS_MALADIE, d�terminer l'�tat
        if (inc_hrs_maladie(&lui->voyageur) >= NB_HRS_MALADIE) {
            resultat = determiner_mort_ou_retabli(&lui->voyageur); // Fonction du module m_personnes
            
            if (resultat == 1) { /* Mort */
                modifier_etat_personne(&lui->voyageur, MORT, PROP_INITIALE);
            }
            else if (resultat == 2) { /* R�tabli */
                //La proportion changera � nouveau une fois
                modifier_etat_personne(&lui->voyageur, SAIN, PROP_INITIALE);
            }
            
            modifier_hrs_maladie_personne(&lui->voyageur, 0);
            return 1; // �tat chang�
        }

    }
    return 0;
}

/*=========================================================*/

t_personne get_personne_migrant(const t_migrant* lui) {
    return lui->voyageur;
}


/*=========================================================*/
