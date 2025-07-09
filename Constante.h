#ifndef _CONSTANTE_2025_1571_
#define _CONSTANTE_2025_1571_

#define M_PI 3.1416922653589793
#define INC_CAT_AGE      10     //écart entre chaque tranche d'age suivante
#define DEUX_PI 6.283185307179586 /* Valeur approchée de 2π */
#define DISTANCE_CONTACT 2.5   //  distance maximale du risque d'infection 
#define PROB_INFECTION  0.9    // probabilité initiale de contracter la maladie
#define REDUCTEUR_INFECTION  2.25  // facteur réducteur de la prob. précédente suite 
#define NB_HRS_MALADIE     15*24    // nombre d'heures de maladie 
#define NB_HRS_TRANSMISSION 2*24 /* Minimum d'heures pour transmission */




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