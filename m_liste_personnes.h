
/*=========================================================*/
/* Module m_liste_personnes pour INF147 - Été 2025 */
/* Gestion d'une liste dynamique de personnes */
/* Salif Diarra et Léo Bouamrane */
/*=========================================================*/
#ifndef _M_LISTE_PERSONNES_2025_1571_
#define _M_LISTE_PERSONNES_2025_1571_

/*=========================================================*/
#include "m_personnes.h"

/*=========================================================*/

/*=========================================================*/

/*=========================================================*/

/*=========================================================*/
/* DÉFINITION DU TYPE */
typedef struct {
    t_personne* liste;    // tableau dynamique de t_personne
    int taille;          // taille du tableau alloué
    int nb_personnes;    // nombre de personnes dans la liste
    int nb_malades;      // nombre de personnes malades
    int nb_sains;        // nombre de personnes saines
    int nb_morts;       // nombre de personnes mortes
    double prop_confine; //nouvelle proportion de confinement
} t_liste_personnes;
/*=========================================================*/

/*=========================================================*/
/* DÉCLARATIONS DES FONCTIONS */
/*=========================================================*/

/* CONSTRUCTEUR */
/* CREER_LISTE_PERSONNES
   Crée une liste vide de personnes avec une taille donnée
   PARAMÈTRES : taille - taille initiale du tableau dynamique
   RETOUR : une nouvelle t_liste_personnes
   SPECS : alloue dynamiquement le tableau, initialise les compteurs à 0
*/
t_liste_personnes creer_liste_personnes(int taille);

/* AJOUTER_DES_PERSONNES
   Ajoute un nombre donné de personnes saines aux premiers indices
   PARAMÈTRES : liste - pointeur sur la liste
                nb - nombre de personnes à ajouter
                largeur, hauteur - dimensions de la région
                prop_confinement - proportion de confinement
   RETOUR : nombre de personnes ajoutées
   SPECS : utilise init_personne pour chaque personne
*/
int ajouter_des_personnes(t_liste_personnes* liste, int nb, double largeur, double hauteur, double prop_confinement);

/* CREER_PATIENT_ZERO
   Choisit aléatoirement une personne saine et la rend malade
   PARAMÈTRES : liste - pointeur sur la liste
   RETOUR : 1 si un patient zéro est créé, 0 sinon
   SPECS : ajuste les compteurs, retourne 0 si aucune personne saine ou déjà des malades
*/
int creer_patient_zero(t_liste_personnes* liste);

/* DESTRUCTEUR */
/* LIBERER_LISTE
   Libère la mémoire du tableau dynamique et réinitialise la liste
   PARAMÈTRES : liste - pointeur sur la liste
   RETOUR : rien
   SPECS : met le pointeur à NULL, taille et compteurs à 0
*/
void liberer_liste(t_liste_personnes* liste);

/* MUTATRICES */
/* DEPLACER_UN_MORT
   Permute une personne morte avec la dernière personne vivante
   PARAMÈTRES : liste - pointeur sur la liste
                indice - indice de la personne à vérifier
   RETOUR : 1 si permutation effectuée, 0 sinon
   SPECS : ajuste les compteurs si nécessaire
*/
int deplacer_un_mort(t_liste_personnes* liste, int indice);

/* ASSURER_TEMPS_MALADIE
   Incrémente les heures de maladie des malades
   PARAMÈTRES : liste - pointeur sur la liste
   RETOUR : nombre de malades ayant atteint NB_HRS_MALADIE
   SPECS : utilise inc_hrs_maladie
*/
int assurer_temps_maladie(t_liste_personnes* liste);

/* TERMINER_MALADIE
   Gère le rétablissement ou la mort des malades ayant atteint NB_HRS_MALADIE
   PARAMÈTRES : liste - pointeur sur la liste
                prop_confinement - proportion de confinement pour les survivants
   RETOUR : nombre de morts apparus
   SPECS : permute les morts en fin de liste, met à jour les compteurs
*/
int terminer_maladie(t_liste_personnes* liste);

/* DEPLACER_LES_PERSONNES
   Déplace les personnes vivantes dans la liste
   PARAMÈTRES : liste - pointeur sur la liste
                largeur, hauteur - dimensions de la région
   RETOUR : nombre de personnes déplacées
   SPECS : utilise deplacer_personne
*/
int deplacer_les_personnes(t_liste_personnes* liste, double largeur, double hauteur);

/* TRAITER_CONTACTS
   Gère la propagation par contacts entre personnes vivantes
   PARAMÈTRES : liste - pointeur sur la liste
                prop_confinement - proportion de confinement pour nouveaux malades
   RETOUR : nombre de nouvelles infections
   SPECS : teste les distances, gère contagion et vitesses
*/
int traiter_contacts(t_liste_personnes* liste);

/* SIMULER_UNE_HEURE_PANDEMIE
   Simule une heure de pandémie en appelant les fonctions mutatrices
   PARAMÈTRES : liste - pointeur sur la liste
                prop_confinement - proportion de confinement
                largeur, hauteur - dimensions de la région
   RETOUR : nombre de malades actuels
   SPECS : définit la séquence des appels
*/
int simuler_une_heure_pandemie(t_liste_personnes* liste, double largeur, double hauteur, int* infection_heure, int* morts_heure);

/*MODIFIER_CONFINEMENT
  ajuste la proportion de confinement de la population en fonction de l etat de sante de la personne
  PARAMETRES : liste - pointeur sur la liste
             nouvelle_prop - nouvelle propriete de confinement
  RETOUR : moyenne de la proababilite de mouvement des personnes vivantes
  */
double modifier_confinement(t_liste_personnes* liste, double nouvelle_prop);

/*SIMULER_PANDEMIE
* Fait une simulation complète d'une pandémie, puis enregistre le résultat dans un fichier.
* PARAMETRES : hauteur, largeur - dimensions de la région en mètres
*              population - taille de la population
*              prop_initiale - proportion initiale de confinement telle que définie dans config_simulations
*              periode_affichage - période d'affichage des bilans provisoires
*              log - pointeur vers le fichier où le log est écrit
* RETOUR : aucun
* SPECS : le préfixe du fichier log n'a jamais plus de 12 caractères et doit être sans caractères blancs
*/
void simuler_pandemie(double hauteur, double largeur, int population, double prop_initiale, int periode_affichage, FILE* log);

/* INFORMATICES */
int get_nb_personnes(const t_liste_personnes* liste);
int get_nb_malades(const t_liste_personnes* liste);
int get_nb_sains(const t_liste_personnes* liste);
int get_nb_morts(const t_liste_personnes* liste);
double get_confinement(const t_liste_personnes* liste);

//Les fonctions suivantes retournent 0 s'il n'y a personne dans la liste.
double get_prop_malades(const t_liste_personnes* liste);
double get_prop_morts(const t_liste_personnes* liste);


/* AFFICHER_LISTE_PERSONNES
   Affiche l'état de la liste pour débogage
   PARAMÈTRES : liste - pointeur constant sur la liste
   RETOUR : rien
   SPECS : utilise afficher_personne
*/
void afficher_liste_personnes(const t_liste_personnes* liste);


int ajouter_une_personne(t_liste_personnes* liste, const t_personne* src);

int enlever_une_personne(t_liste_personnes* liste, t_personne* dest);

#endif
/*=========================================================*/