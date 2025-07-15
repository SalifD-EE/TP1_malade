
/*=========================================================*/
/* Module m_liste_personnes pour INF147 - �t� 2025 */
/* Gestion d'une liste dynamique de personnes */
/* Salif Diarra et L�o Bouamrane */
/*=========================================================*/
#ifndef _M_LISTE_PERSONNES_2025_1571_
#define _M_LISTE_PERSONNES_2025_1571_

/*=========================================================*/
#include "m_personnes.h"

/*=========================================================*/

/*=========================================================*/

/*=========================================================*/

/*=========================================================*/
/* D�FINITION DU TYPE */
typedef struct {
    t_personne* liste;    // tableau dynamique de t_personne
    int taille;          // taille du tableau allou�
    int nb_personnes;    // nombre de personnes dans la liste
    int nb_malades;      // nombre de personnes malades
    int nb_sains;        // nombre de personnes saines
    int nb_morts;       // nombre de personnes mortes
    double prop_confine; //nouvelle proportion de confinement
} t_liste_personnes;
/*=========================================================*/

/*=========================================================*/
/* D�CLARATIONS DES FONCTIONS */
/*=========================================================*/

/* CONSTRUCTEUR */
/* CREER_LISTE_PERSONNES
   Cr�e une liste vide de personnes avec une taille donn�e
   PARAM�TRES : taille - taille initiale du tableau dynamique
   RETOUR : une nouvelle t_liste_personnes
   SPECS : alloue dynamiquement le tableau, initialise les compteurs � 0
*/
t_liste_personnes creer_liste_personnes(int taille);

/* AJOUTER_DES_PERSONNES
   Ajoute un nombre donn� de personnes saines aux premiers indices
   PARAM�TRES : liste - pointeur sur la liste
                nb - nombre de personnes � ajouter
                largeur, hauteur - dimensions de la r�gion
                prop_confinement - proportion de confinement
   RETOUR : nombre de personnes ajout�es
   SPECS : utilise init_personne pour chaque personne
*/
int ajouter_des_personnes(t_liste_personnes* liste, int nb, double largeur, double hauteur, double prop_confinement);

/* CREER_PATIENT_ZERO
   Choisit al�atoirement une personne saine et la rend malade
   PARAM�TRES : liste - pointeur sur la liste
   RETOUR : 1 si un patient z�ro est cr��, 0 sinon
   SPECS : ajuste les compteurs, retourne 0 si aucune personne saine ou d�j� des malades
*/
int creer_patient_zero(t_liste_personnes* liste);

/* DESTRUCTEUR */
/* LIBERER_LISTE
   Lib�re la m�moire du tableau dynamique et r�initialise la liste
   PARAM�TRES : liste - pointeur sur la liste
   RETOUR : rien
   SPECS : met le pointeur � NULL, taille et compteurs � 0
*/
void liberer_liste(t_liste_personnes* liste);

/* MUTATRICES */
/* DEPLACER_UN_MORT
   Permute une personne morte avec la derni�re personne vivante
   PARAM�TRES : liste - pointeur sur la liste
                indice - indice de la personne � v�rifier
   RETOUR : 1 si permutation effectu�e, 0 sinon
   SPECS : ajuste les compteurs si n�cessaire
*/
int deplacer_un_mort(t_liste_personnes* liste, int indice);

/* ASSURER_TEMPS_MALADIE
   Incr�mente les heures de maladie des malades
   PARAM�TRES : liste - pointeur sur la liste
   RETOUR : nombre de malades ayant atteint NB_HRS_MALADIE
   SPECS : utilise inc_hrs_maladie
*/
int assurer_temps_maladie(t_liste_personnes* liste);

/* TERMINER_MALADIE
   G�re le r�tablissement ou la mort des malades ayant atteint NB_HRS_MALADIE
   PARAM�TRES : liste - pointeur sur la liste
                prop_confinement - proportion de confinement pour les survivants
   RETOUR : nombre de morts apparus
   SPECS : permute les morts en fin de liste, met � jour les compteurs
*/
int terminer_maladie(t_liste_personnes* liste);

/* DEPLACER_LES_PERSONNES
   D�place les personnes vivantes dans la liste
   PARAM�TRES : liste - pointeur sur la liste
                largeur, hauteur - dimensions de la r�gion
   RETOUR : nombre de personnes d�plac�es
   SPECS : utilise deplacer_personne
*/
int deplacer_les_personnes(t_liste_personnes* liste, double largeur, double hauteur);

/* TRAITER_CONTACTS
   G�re la propagation par contacts entre personnes vivantes
   PARAM�TRES : liste - pointeur sur la liste
                prop_confinement - proportion de confinement pour nouveaux malades
   RETOUR : nombre de nouvelles infections
   SPECS : teste les distances, g�re contagion et vitesses
*/
int traiter_contacts(t_liste_personnes* liste);

/* SIMULER_UNE_HEURE_PANDEMIE
   Simule une heure de pand�mie en appelant les fonctions mutatrices
   PARAM�TRES : liste - pointeur sur la liste
                prop_confinement - proportion de confinement
                largeur, hauteur - dimensions de la r�gion
   RETOUR : nombre de malades actuels
   SPECS : d�finit la s�quence des appels
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
* Fait une simulation compl�te d'une pand�mie, puis enregistre le r�sultat dans un fichier.
* PARAMETRES : hauteur, largeur - dimensions de la r�gion en m�tres
*              population - taille de la population
*              prop_initiale - proportion initiale de confinement telle que d�finie dans config_simulations
*              periode_affichage - p�riode d'affichage des bilans provisoires
*              log - pointeur vers le fichier o� le log est �crit
* RETOUR : aucun
* SPECS : le pr�fixe du fichier log n'a jamais plus de 12 caract�res et doit �tre sans caract�res blancs
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
   Affiche l'�tat de la liste pour d�bogage
   PARAM�TRES : liste - pointeur constant sur la liste
   RETOUR : rien
   SPECS : utilise afficher_personne
*/
void afficher_liste_personnes(const t_liste_personnes* liste);


int ajouter_une_personne(t_liste_personnes* liste, const t_personne* src);

int enlever_une_personne(t_liste_personnes* liste, t_personne* dest);

#endif
/*=========================================================*/