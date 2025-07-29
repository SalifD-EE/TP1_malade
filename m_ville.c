/*=========================================================*/
/* Module m_ville pour INF147 - Été 2025 */
/* Salif Diarra et Léo Bouamrane */
/*=========================================================*/
#include "m_ville.h"
#include "m_liste_personnes.h"
#include "m_R2.h"
#include "Constante.h"
#include "m_ensemble_noms.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*=========================================================*/

t_ville  init_ville(const char* nom_ville, int largeur, int hauteur, int taille_pop_initiale, double proportion_confinement, double prob_emigrer, int nb_hre_transit) {
	t_ville ville;

	ville->hauteur = hauteur;
	ville->largeur = largeur;
	ville->prob_emigrer = prob_emigrer;
	ville->proportion_confinement = proportion_confinement;
	ville->nb_hre_transit = nb_hre_transit;

	//TODO : la liste chainée de migrants est initialisée
	
	ville->nb_migrants_in = 0;
	ville->nb_migrants_out = 0;
	ville->nb_morts_transit = 0;

	ville->population = creer_liste_personnes(taille_pop_initiale);

	//Copier le nom de la ville dans la structure ainsi que ensemble_nom
	strcpy(ville->nom_ville, nom_ville);
	assert(ajouter_nom_ville(nom_ville) == 1);

	//Initialiser le fichier d'écriture
	ville->logfile = fopen(strcat(nom_ville, EXTENSION_FICHIER), "w");

	return ville;
}