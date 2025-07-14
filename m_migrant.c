/*=========================================================*/
/* Module m_migrant pour INF147 - Été 2025 */
/*=========================================================*/

#include "m_migrant.h"
#include"m_ensemble_noms.h"
#include"m_personnes.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*=========================================================*/

t_migrant init_migrant(const t_personne* src, int ville_dep, int ville_dest, int hrs_trans) {
    t_migrant voyageur;
    voyageur.voyageur = (t_migrant*)malloc(sizeof(t_migrant));
    voyageur.ville_depart = ville_dep;
    voyageur.ville_destination = ville_dest;
    voyageur.hrs_transit = hrs_trans;
    return voyageur;
}

/*=========================================================*/

void set_transit_migrant(t_migrant* lui, int hrs);

/*=========================================================*/

int dec_hrs_transit_migrant(t_migrant* lui);


/*=========================================================*/

int get_destination_migrant(const t_migrant* lui);


/*=========================================================*/

int get_depart_migrant(const t_migrant* lui);


/*=========================================================*/

int est_vivant_migrant(t_migrant* lui);


/*=========================================================*/

int est_malade_migrant(const t_migrant* lui);


/*=========================================================*/

int inc_hrs_maladie_migrant(t_migrant* lui);


/*=========================================================*/

t_personne get_personne_migrant(const t_migrant* lui);


/*=========================================================*/
/*=========================================================*/

