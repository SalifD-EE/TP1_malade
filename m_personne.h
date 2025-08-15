/******************************************************************************/
/* M_PERSONNE.H                                                                 */
/* module de gestion des personnes devant le virus                            */

/* Salif Diarra et L�o Bouamrane */

/******************************************************************************/
#ifndef _M_PERSONNE_2025_1571_
#define _M_PERSONNE_2025_1571_

/******************************************************************************/

#include "m_alea_pop.h"
#include "m_R2.h"
#include "Constante.h"


/******************************************************************************/
/*                         D�CLARATIONS DE TYPES                              */
/******************************************************************************/
// va caracteriser l'etat de la personne 
typedef enum { SAIN, MALADE, MORT } t_etat;

/* Type d�fini pour une personne */
typedef struct {

	// donn� SAIN dans l'initialisation 
	t_etat  etat;        //  �tat actuel 

	 // al�atoire dans le quartier dans l'initialisation 
	t_R2  position;     //  position actuelle dans le quartier

	//   al�atoire  dans l'initialisation 
	//   respecte les normes de vitesse MIN et MAX constantes
	t_R2  vitesse;      //  vecteur vitesse dans le plan

	// obtenu de rand_age_canada dans l'initialisation 
	int     age;         //  age (en ann�es)

	 // 0 dans l'initialisation 
	int     nb_infections;    // nombre d'infections v�cues

	 // 0 dans l'initialisation 
	int    cause_infections;    // nombre d'infections dont la personne est responsable

	// 0 dans l'initialisation 
	int     hrs_maladie;      // nombre d'heures depuis l'infection 

	// obtenu al�atoirement haute ou basse dans l'initialisation 
	double  prob_deplacer;    // probabilit� de se d�placer

	// initialement d'une constante et actualis� dans la simulation 
	double  prob_infection;    // probabilit� actuelle d'�tre infect�

	// initialement d'une constante et actualis� dans la simulation 
	double  prob_mort;   // probabilit� actuelle de mourir au terme de la maladie

} t_personne;


/******************************************************************************/
/*                         D�CLARATIONS DE FONCTIONS                          */
/******************************************************************************/

/****************** constructeur ***************************/
/* INIT_PERSONNE
   Fonction qui va initialiser et retourner une nouvelle personne en sant�
   son age suit la distribution canadienne
 
   PARAM�TRES : hauteur,largeur  -  dimensions de la r�gion de simulation    
                proportion_confinement - c'est d'elle que sera d�duite la probabilit�
				                         haute ou basse de se d�placer 
				
   RETOUR : une nouvelle personne
   SPECS  : aucune
          
*/

t_personne init_personne(double largeur, double hauteur, double proportion_confinement );
/******************************************************************************/

/******************************************************************************/
/****************** informatrices ***************************/

//--------------------------------------------
/* GET_POSITION
   Re�oit une personne et retourne son vecteur position 
   PARAM�TRES : pointeur constant sur une personne 
                
   RETOUR : un t_R2
   SPECS  : aucune
 */
t_R2 get_position(const t_personne * lui);

//--------------------------------------------
/* GET_VITESSE
   Re�oit une personne et retourne son vecteur vitesse
   PARAM�TRES : pointeur constant sur une personne
			
   RETOUR : un t_R2
   SPECS  : aucune
 */
t_R2 get_vitesse(const t_personne* lui);

//--------------------------------------------
/* GET_ETAT        
   Re�oit une personne et retourne son �tat actuel
   PARAM�TRES :  pointeur constant sur une personne 
   RETOUR : l'�tat de la personne ( entier ). 
   SPECS  : aucune   
*/
t_etat get_etat(const t_personne * lui);

//--------------------------------------------
/* GET_AGE
   Re�oit une personne et retourne son age
   PARAM�TRES :  pointeur constant sur une personne
   RETOUR : l'age de la personne ( entier ).
   SPECS  : aucune
*/
int get_age(const t_personne* lui);

//--------------------------------------------
/* GET_PROB_INFECTION
   Re�oit une personne, retourne sa probabilit� actuelle d'�tre infect�
   PARAM�TRES : pointeur constant sur une personne
   RETOUR : la valeur r�elle voulue
   SPECS  : retour  0 s'il n'est pas malade
*/
double get_prob_infection(const t_personne* lui);

//--------------------------------------------
/* GET_PROB_DEPLACEMENT
   Re�oit une personne et retourne sa probabilit�  de se d�placer
   PARAM�TRES : pointeur constant sur une personne
   RETOUR : la valeur r�elle voulue
   SPECS  : aucune
*/
double get_prob_deplacement(const t_personne* lui);

//--------------------------------------------
/* GET_PROB_MORT
   Re�oit une personne et retourne sa probabilit� actuelle de d�c�s
   PARAM�TRES : pointeur constant sur une personne
   RETOUR : la valeur r�elle voulue
   SPECS  : aucune
*/
double get_prob_mort(const t_personne* lui);

//--------------------------------------------
/* GET_NB_INFECTION
   Re�oit une personne et retourne le nombre d'infections qu'elle a v�cues
   PARAM�TRES : pointeur constant sur une personne
   RETOUR : son nombre d'occurences de la maladie
   SPECS  : aucune
*/
int get_nb_infection(const t_personne* lui);

//--------------------------------------------
/* GET_CAUSE_INFECTIONS
   Re�oit une personne et retourne le nombre d'infections qu'elle a caus�es
   PARAM�TRES : pointeur constant sur une personne
   RETOUR : nombre de personnes qu'il a infect� lors d'un contact
   SPECS  : aucune
*/
int get_cause_infections(const t_personne* lui);

//--------------------------------------------
/* GET_HRS_MALADIE      
   Re�oit une personne et retourne le nombre d'heures depuis son infection
   PARAM�TRES : pointeur constant sur une personne 
   RETOUR : nombre d'heures depuis le d�but de la maladie      
   SPECS  : retour  0 s'il n'est pas malade
*/
int get_hrs_maladie(const t_personne * lui);

//--------------------------------------------
// FIN DES GET
//--------------------------------------------

//--------------------------------------------
/* MODIFIER_POSITION_PERSONNE
   Re�oit une personne ainsi qu'une position � lui assigner.
   PARAM�TRES : pointeur sur une personne, position � assigner
   RETOUR : aucun
   SPECS  : aucun
*/
void modifier_position_personne(t_personne* lui, t_R2 position);

//--------------------------------------------
/* MODIFIER_VITESSE_PERSONNE
   Re�oit une personne ainsi qu'une vitesse � lui assigner.
   PARAM�TRES : pointeur sur une personne, vitesse � assigner
   RETOUR : aucun
   SPECS  : aucun
*/
void modifier_vitesse_personne(t_personne* lui, t_R2 vitesse);

//--------------------------------------------
/* DETERMINER_MORT_RETABLI
   Re�oit une personne et d�termine au terme de la maladie
   s'il va mourir ou se r�tablir

   PARAM�TRES : pointeur constant sur une personne 
   RETOUR : 1 s'il meurt , 2 si retabli , 0 sinon  
   SPECS  :  retour 0 s'il n'est pas malade, s'il est d�j� mort
         ou si ses heures de maladie ne sont pas compl�t�es
 */
int determiner_mort_ou_retabli(const t_personne * lui);

//--------------------------------------------
/* DISTANCE_PERSONNES      
   retourne la distance entre 2 t_personnes en m�tres
   PARAM�TRES : deux pointeurs constant 
   RETOUR :  la distance obtenue du th.pythagore de leurs positions
   SPECS  : aucune
 */
double distance_personnes(const t_personne * p1, const t_personne* p2);

//--------------------------------------------

//--------------------------------------------
/* Uniquement pour favoriser un debuggage autre que breakpoint et assert
  faites une fonction d'affichage d'une personne
  c'est � vous de d�terminer vos printf
  Cette fonction ne servira JAMAIS dans la simulation v�ritable
*/
void afficher_personne(const t_personne* lui);
//--------------------------------------------

/******************************************************************************/
//   MUTATRICES
/******************************************************************************/
//--------------------------------------------
/* DEPLACER_PERSONNE      
   Re�oit une personne et teste sa probabilit� � se d�placer
          si oui  position += vitesse   et tester  le quartier de simulation 
   
   PARAM�TRES : pointeur sur la personne � d�placer 
                largeur, hauteur - les dimensions de l'espace de simulation 

   RETOUR :     1 si elle bouge et 0 sinon

   SPECS  :   Une personne morte ne bouge pas, une vivante peut ne pas se d�placer, 
              pour forcer la personne dans l'espace de simulation
			  sa vitesse peut �tre modifi�e         
*/
 int deplacer_personne(t_personne* lui, double largeur, double hauteur);

//--------------------------------------------
/* INVERSER_LES_VITESSES
    recoit deux personnes vivantes suffisammentent proches  pour changer leurs vitesses 
	                  pour deux directions al�atoires oppos�es 
   PARAM�TRES :  pointeurs sur les 2 personnes 
   RETOUR :  1 si changement ou 0 sinon (peu importe la raison)
   SPECS  : aucune
  */
int inverser_les_vitesses(t_personne * lui, t_personne * autre);
//--------------------------------------------
/* MODIFIER_ETAT_PERSONNE     
   Re�oit une personne et un nouvel �tat 
   Modifiera l'�tat de la personne et  red�finit sa 
    probabilit� de se d�placer selon les m�mes r�gles 
	que celles du constructeur

   PARAM�TRES : pointeur sur la personne � modifier 
                etat - nouvel �tat � attribuer � la personne 
				la proportion de confinement n�cessaire pour le calcul 
				de la probabilit� de se d�placer

   RETOUR :  Rien
   SPECS  : aucune
 */
void modifier_etat_personne(t_personne * lui, t_etat etat, double proportion_confinement);
//--------------------------------------------
//--------------------------------------------
/* MODIFIER_PROB_DEPLACER
   Re�oit une personne et la proportion de confinement voulue
   elle red�finit la  probabilit� de se d�placer de la personne
   selon les m�mes r�gles que celles du constructeur

   PARAM�TRES : pointeur sur la personne � modifier
				la proportion de confinement n�cessaire pour le calcul

   RETOUR :  retourne la nouvelle prob_deplacer
   SPECS  : aucune
 */
double modifier_prob_deplacer(t_personne* lui, double proportion_confinement);
//--------------------------------------------
//--------------------------------------------
/* INCREMENTER_HRS_MALADIE
   Re�oit une personne et  incr�mente (+1) son nombre d'heures de maladie
   PARAM�TRES : pointeur sur la personne � modifier .      
   RETOUR :  le nombre d'heures de maladie de la personne
   SPECS  : aucune 
 */
int inc_hrs_maladie(t_personne* lui);
//--------------------------------------------
//--------------------------------------------
/* INCREMENTER_CAUSE_INFECTIONS
   Re�oit une personne et  incr�mente (+1) le nombres de personnes qu'elle a infect�
   PARAM�TRES : pointeur sur la personne � modifier .
   RETOUR :  le nombre de personnes qu'elle a infect� 
   SPECS  : aucune
 */
int inc_cause_infections(t_personne* lui);
//--------------------------------------------/******************************************************************************/

#endif 

/******************************************************************************/