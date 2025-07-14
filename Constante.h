#ifndef _CONSTANTE_2025_1571_
#define _CONSTANTE_2025_1571_

#define MODE_SIMULATION 1 //1 = Sortie dans fichiers log, 0 = Sortie dans console

#define M_PI 3.1416922653589793
#define PROP_INITIALE 0.4  //pourcentage pou la proportion de confinement initiale pour la simulation console
#define INC_CAT_AGE      10     //écart entre chaque tranche d'age suivante
#define DEUX_PI 6.283185307179586 /* Valeur approchée de 2π */
#define DISTANCE_CONTACT 2.5   //  distance maximale du risque d'infection 
#define PROB_INFECTION  0.25    // probabilité initiale de contracter la maladie pour la simulation console
#define REDUCTEUR_INFECTION  2.25  // facteur réducteur de la proba d'infection suite a une précédente infection 
#define NB_HRS_MALADIE     15*24    // nombre d'heures de maladie 
#define NB_HRS_TRANSMISSION 2*24 /* Minimum d'heures pour transmission */

//Définition des paliers qui causent les changements de confinement
#define PALIER_MALADES_1 0.005
#define PALIER_MALADES_2 0.01
#define PALIER_MALADES_3 0.03
#define PALIER_MALADES_4 0.10

//Définition des proportions de confinement
#define PROP_CONFINEMENT_BAS  0.3
#define PROP_CONFINEMENT_INTERMEDIAIRE  0.5
#define PROP_CONFINEMENT_ELEVE  0.8
#define PROP_CONFINEMENT_TOTAL 1.0


/* uniquement pour  établir la norme du vecteur vitesse */
// intervalle du choix aléatoire de la norme du vecteur vitesse 
#define NORME_VIT_MIN   4    
#define NORME_VIT_MAX   12   

/* uniquement pour  établir la probabilité de se déplacer */
/* la probabilité de se déplacer d'une t_personne sera obtenue
	soit haute et choisie aléatoirement dans [PROB_HAUTE_MIN, 1.0]
	ou basse et choisie aléatoirement dans [0, PROB_BASSE_MAX ]*/
#define PROB_BASSE_MAX   0.2    
#define PROB_HAUTE_MIN   0.75    


#define AGE_JEUNE        29     //limite de la premiere tranche d'age
#define INC_CAT_AGE      10     //écart entre chaque tranche d'age suivante


#endif 