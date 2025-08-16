/******************************************************************************/
/* M_VILLE.H                                                                  */
/* module de gestion des villes devant le virus                               */

/* Salif Diarra et Léo Bouamrane */

/******************************************************************************/
#ifndef _M_VILLE_2025_2936_
#define _M_VILLE_2025_2936_

/******************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include "m_R2.h"
#include "Constante.h"
#include "m_liste_personnes.h"
#include "m_liste_migrants.h"


/******************************************************************************/
/*                         DÉCLARATIONS DE TYPES                              */
/******************************************************************************/

// concaténé au nom de la ville pour obtenir le nom du fichier de résultats 
#define EXTENSION_FICHIER    "_log.txt"   
#define MAX_NOM_VILLE 50
#define MAX_BORNE_EMIGRER 5

typedef struct ville* t_ville;

struct ville {
    char nom_ville[MAX_NOM_VILLE]; // nom de la ville
    int largeur, hauteur;        // dimensions de la ville
    double proportion_confinement;	// proportion de désiré dans cette ville  
    double prob_emigrer;	 // probabilité de vouloir quitter la ville
    t_liste_personnes population;  // les habitants de la ville
    t_liste_migrants migrants;   // SD des t_migrants, liste chainée quelconque
    int nb_hre_transit;     //  nombre d'heures de transit d'un migrant dans la SD
    
    //les compteurs du transit de personnes
    int nb_migrants_out;	       //  qui ont quitté la ville
    int nb_migrants_in;	       // qui ont rejoint la ville 
    int nb_morts_transit;	// nb. de migrants qui sont morts en transit 
    int infection_heure;
    int morts_heure;

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

//const char* get_nom_ville(t_ville ville);

/*=========================================================*/
/* INFORMATRICES
/*=========================================================*/
int get_largeur(t_ville ville);
int get_hauteur(t_ville ville);
double get_proportion_confinement(t_ville ville);
double get_prob_emigrer(t_ville ville);
t_liste_personnes get_population(t_ville ville);
t_liste_migrants get_migrants(t_ville ville);
int get_nb_hre_transit(t_ville ville);
int get_nb_migrants_out(t_ville ville);
int get_nb_migrants_in(t_ville ville);
int get_nb_morts_transit(t_ville ville);
FILE* get_logfile(t_ville ville);

/*=========================================================*/
/* MUTATRICES
/*=========================================================*/
void inoculer_ville(t_ville ville);

void simuler_une_heure_pandemie_ville(t_ville ville);

int obtenir_des_personnes_ville(t_ville ville);

int obtenir_des_migrants_ville(t_ville ville);

int transferer_des_migrants_entre_villes(t_ville src, t_ville dest);

static void traiter_heures_transit(t_liste_migrants list);

static void traiter_heures_maladie(t_liste_migrants list);

void ecrire_logfile_ville(t_ville ville);

void detruire_ville(t_ville  ville);

#endif 
