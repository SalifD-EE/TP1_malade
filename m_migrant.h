/******************************************************************************/     
/* t_migrant.h      (*** donné aux etudiant·e·s ***)    */
/*   module de gestion d'un migrant    */
/******************************************************************************/
/* 
	une t_personne qui change de ville devient un t_migrant 
	et le restera tant  qu'il n'integre pas  la population d'une autre ville
	ou qu'il meure et quitte la simulation 

	un migrant ne se déplace pas et ne transmet pas la maladie
	s'il est malade, après la période de maladie, il quérit ou il meurt  

*/
//**********************************

#ifndef __MIGRER_2020IS__
#define __MIGRER_2020IS__

/*=========================================================*/

#include"m_ensemble_noms.h"
#include"m_personne.h"
#include"m_liste_personnes.h"

//**********************************


//**********************************
/* Type de la structure pour un voyageur  */
typedef struct{
  t_personne voyageur;
  int ville_depart;   // numero de la ville de départ
  int ville_destination;   // numero de la ville destination
  int  hrs_transit;     // le nombre d'heures de transit de la ville qu'il quitte
} t_migrant;
//**********************************

//**********les méthodes du migrant ***************

/* constructeur
   créer un nouveau migrant 
   PARAMÈTRES : la personne qui devient migrant
                ville_dep  - numero de la ville dont il quitte la population
                ville_dest  - numero de la ville vers laquelle il  migre.
				hrs_trans -  le nombre d'heures de son premier transit dans sa ville de départ
				
   RETOUR : un nouveau migrant
 */
t_migrant  init_migrant(const t_personne* src, int ville_dep, int ville_dest, int hrs_trans);
//**********************************

/*  mutatrice (lorsqu'un migrant passe à la ville suivante)
    modifier son nombre d'heures de transit 
   PARAMÈTRES :  pointeur sur un migrant 
                nouveau nombre d'heures de transit  */ 

void set_transit_migrant(t_migrant* lui, int hrs);

//**********************************
/*  mutatrice
   décrémenter (-1) son nombre d'heures de transit
   s'il est > 0
   PARAMÈTRES : pointeur sur un migrant
   RETOUR : le nombre d'heures de transit restantes 
*/
int dec_hrs_transit_migrant(t_migrant* lui);

//**********************************

//**********************************
/* informatrice
   obtenir  la ville destination du migrant
   PARAMÈTRES : pointeur sur un migrant
   RETOUR : numéro de la ville_destination */
int get_destination_migrant(const t_migrant* lui);

//**********************************
/* informatrice
   obtenir  la ville depart du migrant
   PARAMÈTRES : pointeur sur un migrant
   RETOUR : numéro de la ville_depart */
int get_depart_migrant(const t_migrant* lui);

//**********************************
/* informatrice
   retourner 1 s'il est vivant et 0 s'il est mort
   PARAMÈTRES : pointeur sur un migrant
   RETOUR : 0 ou 1  */
int est_vivant_migrant(t_migrant* lui);


//**********************************
/* informatrice
   obtenir le nombre d'heures de transit d'un migrant
   PARAMÈTRES : pointeur sur un migrant
   RETOUR : nombre d'hrs de transit */
int get_hrs_transit(const t_migrant* lui);
//**********************************

/* informatrice
    retourner 1 s'il est malade et 0 sinon
   PARAMÈTRES : lui - pointeur sur une migrant
   RETOUR : 0 ou 1  */
int est_malade_migrant(const t_migrant* lui);
//**********************************

/* mutatrice
   si le migrant est malade 
   incrementer (+1) son nombre d'heures de maladie
   si le nombre d'heures de maladie atteint NB_HRS_MALADIE
   déclencher determiner_mort_ou_retabli et modifier son état
   PARAMÈTRES : pointeur sur un migrant 
   RETOUR : 1 si la personne sous-jacente a changé d'état
            0 sinon 
*/
int inc_hrs_maladie_migrant(t_migrant* lui);
//**********************************
//**********************************


//**********************************
/* informatrice 
   retourne une copie de la personne dans la structure migrant    
   PARAMÈTRES : pointeur sur un migrant
   RETOUR :   la t_personne */
t_personne get_personne_migrant(const t_migrant* lui);
//**********************************
//**********************************


#endif