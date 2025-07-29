/******************************************************************************/
/* M_VILLE.H                                                                  */
/* module de gestion des villes devant le virus                               */

/* Salif Diarra et Léo Bouamrane */

/******************************************************************************/
#ifndef _M_VILLE_2025_2936_
#define _M_VILLE_2025_2936_

/******************************************************************************/

#include "m_R2.h"
#include "Constante.h"
#include "m_liste_personnes.h"
//#include "m_liste_migrants.h"


/******************************************************************************/
/*                         DÉCLARATIONS DE TYPES                              */
/******************************************************************************/

// concaténé au nom de la ville pour obtenir le nom du fichier de résultats 
#define EXTENSION_FICHIER    "_log.txt"   
#define MAX_NOM_VILLE 50   

typedef struct ville* t_ville;

struct ville {
    char nom_ville[MAX_NOM_VILLE]; // nom de la ville
    int largeur, hauteur;        // dimensions de la ville
    double proportion_confinement;	// proportion de désiré dans cette ville  
    double prob_emigrer;	 // probabilité de vouloir quitter la ville
    t_liste_personnes population;  // les habitants de la ville
    //t_liste_migrants migrants;   // SD des t_migrants, liste chainée quelconque
    int nb_hre_transit;     //  nombre d'heures de transit d'un migrant dans la SD
    
    //les compteurs du transit de personnes
    int nb_migrants_out;	       //  qui ont quitté la ville
    int nb_migrants_in;	       // qui ont rejoint la ville 
    int nb_morts_transit;	// nb. de migrants qui sont morts en transit 
    
    // le fichier de log des résultats de la simulation  
    FILE* logfile;	            // lien au fichier texte des résultats
};

/******************************************************************************/
/*                         DÉCLARATIONS DE FONCTIONS                          */
/******************************************************************************/

/****************** constructeur ***************************/
/* INIT_VILLE
   Fonction qui va initialiser et retourner une nouvelle ville

   PARAMÈTRES : nom_ville                   - le nom de la ville
                largeur, hauteur            - dimensions de la ville
                proportion_confinement      - c'est d'elle que sera déduite la probabilité
                                              haute ou basse de se déplacer
                taille_pop_initiale         - taille de la population initiale
                prob_emigrer                - probabilité qu'un migrant quitte la ville
                proportion_confinement      - proportion de confinement désirée
                prob_emigrer                - probabilité qu'un habitant veuille quitter la ville
                nb_hre_transit              - nombre d'heures passées dans la liste de migrants de la ville

   RETOUR : une nouvelle ville
   SPECS  : aucune
*/
t_ville  init_ville(const char* nom_ville, int largeur, int hauteur, int taille_pop_initiale, double proportion_confinement, double prob_emigrer, int nb_hre_transit);

#endif 
