/*=========================================================*/
/* Module m_migrant pour INF147 - Été 2025 */
/*=========================================================*/

#include "m_migrant.h"
#include"m_ensemble_noms.h"
#include"m_personnes.h"
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

int get_destination_migrant(const t_migrant* lui) {

    return lui->ville_destination;
}


/*=========================================================*/

int get_depart_migrant(const t_migrant* lui) {

    return lui->ville_depart;
}


/*=========================================================*/

int est_vivant_migrant(t_migrant* lui) {

    return get_etat(&lui->voyageur);
  
}


/*=========================================================*/

int est_malade_migrant(const t_migrant* lui) {

    return get_etat(&lui->voyageur);

    
}


/*=========================================================*/

int inc_hrs_maladie_migrant(t_migrant* lui) {
    int changement = 0;
    if ( est_malade_migrant(lui) ) {
        // Incrémente les heures de maladie via le module m_personnes
        int changement = inc_hrs_maladie(&lui->voyageur);
        // Si le nombre d'heures de maladie atteint NB_HRS_MALADIE, déterminer l'état
        if (get_hrs_maladie(&lui->voyageur) >= NB_HRS_MALADIE) {
            determiner_mort_ou_retabli(&lui->voyageur); // Fonction du module m_personnes
            return 1; // État changé
        }
        return changement; // Retourne 1 si les heures de maladie ont changé, 0 sinon
    }
}



/*=========================================================*/

t_personne get_personne_migrant(const t_migrant* lui) {
    return lui->voyageur;
}


/*=========================================================*/
/*=========================================================*/

