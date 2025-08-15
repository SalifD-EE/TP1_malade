/******************************************************************************/
/* M_PERSONNE.H                                                                 */
/* module de gestion des personnes devant le virus                            */

/* Salif Diarra et Léo Bouamrane */

/******************************************************************************/
#ifndef _M_PERSONNE_2025_1571_
#define _M_PERSONNE_2025_1571_

/******************************************************************************/

#include "m_alea_pop.h"
#include "m_R2.h"
#include "Constante.h"


/******************************************************************************/
/*                         DÉCLARATIONS DE TYPES                              */
/******************************************************************************/
// va caracteriser l'etat de la personne 
typedef enum { SAIN, MALADE, MORT } t_etat;

/* Type défini pour une personne */
typedef struct {

	// donné SAIN dans l'initialisation 
	t_etat  etat;        //  état actuel 

	 // aléatoire dans le quartier dans l'initialisation 
	t_R2  position;     //  position actuelle dans le quartier

	//   aléatoire  dans l'initialisation 
	//   respecte les normes de vitesse MIN et MAX constantes
	t_R2  vitesse;      //  vecteur vitesse dans le plan

	// obtenu de rand_age_canada dans l'initialisation 
	int     age;         //  age (en années)

	 // 0 dans l'initialisation 
	int     nb_infections;    // nombre d'infections vécues

	 // 0 dans l'initialisation 
	int    cause_infections;    // nombre d'infections dont la personne est responsable

	// 0 dans l'initialisation 
	int     hrs_maladie;      // nombre d'heures depuis l'infection 

	// obtenu aléatoirement haute ou basse dans l'initialisation 
	double  prob_deplacer;    // probabilité de se déplacer

	// initialement d'une constante et actualisé dans la simulation 
	double  prob_infection;    // probabilité actuelle d'être infecté

	// initialement d'une constante et actualisé dans la simulation 
	double  prob_mort;   // probabilité actuelle de mourir au terme de la maladie

} t_personne;


/******************************************************************************/
/*                         DÉCLARATIONS DE FONCTIONS                          */
/******************************************************************************/

/****************** constructeur ***************************/
/* INIT_PERSONNE
   Fonction qui va initialiser et retourner une nouvelle personne en santé
   son age suit la distribution canadienne
 
   PARAMÈTRES : hauteur,largeur  -  dimensions de la région de simulation    
                proportion_confinement - c'est d'elle que sera déduite la probabilité
				                         haute ou basse de se déplacer 
				
   RETOUR : une nouvelle personne
   SPECS  : aucune
          
*/

t_personne init_personne(double largeur, double hauteur, double proportion_confinement );
/******************************************************************************/

/******************************************************************************/
/****************** informatrices ***************************/

//--------------------------------------------
/* GET_POSITION
   Reçoit une personne et retourne son vecteur position 
   PARAMÈTRES : pointeur constant sur une personne 
                
   RETOUR : un t_R2
   SPECS  : aucune
 */
t_R2 get_position(const t_personne * lui);

//--------------------------------------------
/* GET_VITESSE
   Reçoit une personne et retourne son vecteur vitesse
   PARAMÈTRES : pointeur constant sur une personne
			
   RETOUR : un t_R2
   SPECS  : aucune
 */
t_R2 get_vitesse(const t_personne* lui);

//--------------------------------------------
/* GET_ETAT        
   Reçoit une personne et retourne son état actuel
   PARAMÈTRES :  pointeur constant sur une personne 
   RETOUR : l'état de la personne ( entier ). 
   SPECS  : aucune   
*/
t_etat get_etat(const t_personne * lui);

//--------------------------------------------
/* GET_AGE
   Reçoit une personne et retourne son age
   PARAMÈTRES :  pointeur constant sur une personne
   RETOUR : l'age de la personne ( entier ).
   SPECS  : aucune
*/
int get_age(const t_personne* lui);

//--------------------------------------------
/* GET_PROB_INFECTION
   Reçoit une personne, retourne sa probabilité actuelle d'être infecté
   PARAMÈTRES : pointeur constant sur une personne
   RETOUR : la valeur réelle voulue
   SPECS  : retour  0 s'il n'est pas malade
*/
double get_prob_infection(const t_personne* lui);

//--------------------------------------------
/* GET_PROB_DEPLACEMENT
   Reçoit une personne et retourne sa probabilité  de se déplacer
   PARAMÈTRES : pointeur constant sur une personne
   RETOUR : la valeur réelle voulue
   SPECS  : aucune
*/
double get_prob_deplacement(const t_personne* lui);

//--------------------------------------------
/* GET_PROB_MORT
   Reçoit une personne et retourne sa probabilité actuelle de décès
   PARAMÈTRES : pointeur constant sur une personne
   RETOUR : la valeur réelle voulue
   SPECS  : aucune
*/
double get_prob_mort(const t_personne* lui);

//--------------------------------------------
/* GET_NB_INFECTION
   Reçoit une personne et retourne le nombre d'infections qu'elle a vécues
   PARAMÈTRES : pointeur constant sur une personne
   RETOUR : son nombre d'occurences de la maladie
   SPECS  : aucune
*/
int get_nb_infection(const t_personne* lui);

//--------------------------------------------
/* GET_CAUSE_INFECTIONS
   Reçoit une personne et retourne le nombre d'infections qu'elle a causées
   PARAMÈTRES : pointeur constant sur une personne
   RETOUR : nombre de personnes qu'il a infecté lors d'un contact
   SPECS  : aucune
*/
int get_cause_infections(const t_personne* lui);

//--------------------------------------------
/* GET_HRS_MALADIE      
   Reçoit une personne et retourne le nombre d'heures depuis son infection
   PARAMÈTRES : pointeur constant sur une personne 
   RETOUR : nombre d'heures depuis le début de la maladie      
   SPECS  : retour  0 s'il n'est pas malade
*/
int get_hrs_maladie(const t_personne * lui);

//--------------------------------------------
// FIN DES GET
//--------------------------------------------

//--------------------------------------------
/* MODIFIER_POSITION_PERSONNE
   Reçoit une personne ainsi qu'une position à lui assigner.
   PARAMÈTRES : pointeur sur une personne, position à assigner
   RETOUR : aucun
   SPECS  : aucun
*/
void modifier_position_personne(t_personne* lui, t_R2 position);

//--------------------------------------------
/* MODIFIER_VITESSE_PERSONNE
   Reçoit une personne ainsi qu'une vitesse à lui assigner.
   PARAMÈTRES : pointeur sur une personne, vitesse à assigner
   RETOUR : aucun
   SPECS  : aucun
*/
void modifier_vitesse_personne(t_personne* lui, t_R2 vitesse);

//--------------------------------------------
/* DETERMINER_MORT_RETABLI
   Reçoit une personne et détermine au terme de la maladie
   s'il va mourir ou se rétablir

   PARAMÈTRES : pointeur constant sur une personne 
   RETOUR : 1 s'il meurt , 2 si retabli , 0 sinon  
   SPECS  :  retour 0 s'il n'est pas malade, s'il est déjà mort
         ou si ses heures de maladie ne sont pas complétées
 */
int determiner_mort_ou_retabli(const t_personne * lui);

//--------------------------------------------
/* DISTANCE_PERSONNES      
   retourne la distance entre 2 t_personnes en mètres
   PARAMÈTRES : deux pointeurs constant 
   RETOUR :  la distance obtenue du th.pythagore de leurs positions
   SPECS  : aucune
 */
double distance_personnes(const t_personne * p1, const t_personne* p2);

//--------------------------------------------

//--------------------------------------------
/* Uniquement pour favoriser un debuggage autre que breakpoint et assert
  faites une fonction d'affichage d'une personne
  c'est à vous de déterminer vos printf
  Cette fonction ne servira JAMAIS dans la simulation véritable
*/
void afficher_personne(const t_personne* lui);
//--------------------------------------------

/******************************************************************************/
//   MUTATRICES
/******************************************************************************/
//--------------------------------------------
/* DEPLACER_PERSONNE      
   Reçoit une personne et teste sa probabilité à se déplacer
          si oui  position += vitesse   et tester  le quartier de simulation 
   
   PARAMÈTRES : pointeur sur la personne à déplacer 
                largeur, hauteur - les dimensions de l'espace de simulation 

   RETOUR :     1 si elle bouge et 0 sinon

   SPECS  :   Une personne morte ne bouge pas, une vivante peut ne pas se déplacer, 
              pour forcer la personne dans l'espace de simulation
			  sa vitesse peut être modifiée         
*/
 int deplacer_personne(t_personne* lui, double largeur, double hauteur);

//--------------------------------------------
/* INVERSER_LES_VITESSES
    recoit deux personnes vivantes suffisammentent proches  pour changer leurs vitesses 
	                  pour deux directions aléatoires opposées 
   PARAMÈTRES :  pointeurs sur les 2 personnes 
   RETOUR :  1 si changement ou 0 sinon (peu importe la raison)
   SPECS  : aucune
  */
int inverser_les_vitesses(t_personne * lui, t_personne * autre);
//--------------------------------------------
/* MODIFIER_ETAT_PERSONNE     
   Reçoit une personne et un nouvel état 
   Modifiera l'état de la personne et  redéfinit sa 
    probabilité de se déplacer selon les mêmes règles 
	que celles du constructeur

   PARAMÈTRES : pointeur sur la personne à modifier 
                etat - nouvel état à attribuer à la personne 
				la proportion de confinement nécessaire pour le calcul 
				de la probabilité de se déplacer

   RETOUR :  Rien
   SPECS  : aucune
 */
void modifier_etat_personne(t_personne * lui, t_etat etat, double proportion_confinement);
//--------------------------------------------
//--------------------------------------------
/* MODIFIER_PROB_DEPLACER
   Reçoit une personne et la proportion de confinement voulue
   elle redéfinit la  probabilité de se déplacer de la personne
   selon les mêmes règles que celles du constructeur

   PARAMÈTRES : pointeur sur la personne à modifier
				la proportion de confinement nécessaire pour le calcul

   RETOUR :  retourne la nouvelle prob_deplacer
   SPECS  : aucune
 */
double modifier_prob_deplacer(t_personne* lui, double proportion_confinement);
//--------------------------------------------
//--------------------------------------------
/* INCREMENTER_HRS_MALADIE
   Reçoit une personne et  incrémente (+1) son nombre d'heures de maladie
   PARAMÈTRES : pointeur sur la personne à modifier .      
   RETOUR :  le nombre d'heures de maladie de la personne
   SPECS  : aucune 
 */
int inc_hrs_maladie(t_personne* lui);
//--------------------------------------------
//--------------------------------------------
/* INCREMENTER_CAUSE_INFECTIONS
   Reçoit une personne et  incrémente (+1) le nombres de personnes qu'elle a infecté
   PARAMÈTRES : pointeur sur la personne à modifier .
   RETOUR :  le nombre de personnes qu'elle a infecté 
   SPECS  : aucune
 */
int inc_cause_infections(t_personne* lui);
//--------------------------------------------/******************************************************************************/

#endif 

/******************************************************************************/