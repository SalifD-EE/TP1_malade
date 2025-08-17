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

/* INOCULER_VILLE
   Fonction qui va créer le patient zéro dans une ville
   PARAMÈTRES : ville - pointeur vers la ville à inoculer
   RETOUR : aucun
   SPECS  : choisit aléatoirement une personne saine de la population et la rend malade
            utilise creer_patient_zero() de m_liste_personnes
            ne fait rien si la ville est vide ou s'il y a déjà des malades
*/
void inoculer_ville(t_ville ville);

/* SIMULER_UNE_HEURE_PANDEMIE_VILLE
   Fonction qui simule une heure complète de pandémie dans une ville
   PARAMÈTRES : ville - pointeur vers la ville à simuler
   RETOUR : aucun
   SPECS  : effectue dans l'ordre :
            1. simulation d'une heure de pandémie sur la population
            2. traitement des heures de transit des migrants
            3. traitement des maladies des migrants en transit
            met à jour les compteurs infection_heure et morts_heure de la ville
*/
void simuler_une_heure_pandemie_ville(t_ville ville);

/* TRAITER_HEURES_TRANSIT
   Fonction privée qui décrémente les heures de transit de tous les migrants
   PARAMÈTRES : liste - liste des migrants à traiter
   RETOUR : aucun
   SPECS  : parcourt tous les migrants de la liste et décrémente leur temps de transit
            utilise dec_hrs_transit_liste_migrants() pour chaque migrant
            fonction statique, usage interne au module seulement
*/
void traiter_heures_transit(t_liste_migrants liste);

/* TRAITER_HEURES_MALADIE_MIGRANTS
   Fonction privée qui gère l'évolution des maladies des migrants en transit
   PARAMÈTRES : liste - liste des migrants à traiter
                proportion_confinement - proportion de confinement de la ville
   RETOUR : nombre de migrants morts en transit
   SPECS  : pour chaque migrant malade :
            - incrémente ses heures de maladie
            - si la maladie est terminée, détermine s'il meurt ou guérit
            - retire les morts de la liste des migrants
            fonction statique, usage interne au module seulement
*/
int traiter_heures_maladie_migrants(t_liste_migrants liste, double proportion_confinement);

/* OBTENIR_DES_PERSONNES_VILLE
   Fonction qui intègre les migrants arrivés à destination dans la population
   PARAMÈTRES : ville - pointeur vers la ville de destination
   RETOUR : nombre de migrants intégrés à la population
   SPECS  : parcourt la liste des migrants et pour chaque migrant :
            - vérifie s'il est arrivé à destination (nom de ville correspond)
            - lui assigne une position et vitesse aléatoires
            - l'ajoute à la population locale
            - le retire de la liste des migrants
            met à jour le compteur nb_migrants_in
*/
int obtenir_des_personnes_ville(t_ville ville);

/* OBTENIR_DES_MIGRANTS_VILLE
   Fonction qui génère des migrants à partir de la population locale
   PARAMÈTRES : ville - pointeur vers la ville source
   RETOUR : nombre de migrants générés
   SPECS  : avec une probabilité prob_emigrer de la ville :
            - choisit aléatoirement des personnes de la population
            - détermine leur destination via la matrice de transition
            - les convertit en migrants avec temps de transit à 0
            - les ajoute à la liste des migrants
            - les retire de la population locale
            met à jour le compteur nb_migrants_out
*/
int obtenir_des_migrants_ville(t_ville ville);

/* TRANSFERER_DES_MIGRANTS_ENTRE_VILLES
   Fonction qui transfère des migrants d'une ville source vers une ville destination
   PARAMÈTRES : src - pointeur vers la ville source
                dest - pointeur vers la ville destination
   RETOUR : nombre de migrants transférés
   SPECS  : parcourt les migrants de la ville source et pour chaque migrant :
            - vérifie s'il a terminé son transit (hrs_transit == 0)
            - vérifie si sa destination ne correspond pas à la ville dest
            - le transfère vers la liste des migrants de dest
            - lui assigne le temps de transit de la ville dest
            - le retire de la liste source
            ne transfère que si dest est la ville suivante dans l'ordre
*/
int transferer_des_migrants_entre_villes(t_ville src, t_ville dest);

/* ECRIRE_LOGFILE_VILLE
   Fonction qui écrit les statistiques actuelles de la ville dans son fichier log
   PARAMÈTRES : ville - pointeur vers la ville à documenter
   RETOUR : aucun
   SPECS  : écrit dans le fichier logfile de la ville :
            - l'ID de la ville (nom_ville)
            - le nombre de personnes dans la population
            - le nombre de migrants entrés
            - le nombre de migrants morts en transit
            ajoute une ligne de séparation
            ne ferme pas le fichier après écriture
*/
void ecrire_logfile_ville(t_ville ville);

/* DETRUIRE_VILLE
   Fonction destructrice qui libère toutes les ressources d'une ville
   PARAMÈTRES : ville - pointeur vers la ville à détruire
   RETOUR : aucun
   SPECS  : effectue dans l'ordre :
            1. libère la liste de population (liberer_liste)
            2. libère la liste des migrants (detruire_liste_migrants)
            3. ferme le fichier logfile s'il est ouvert
            4. libère la structure ville elle-même
            après appel, le pointeur ville ne doit plus être utilisé
*/
void detruire_ville(t_ville ville);

#endif 
