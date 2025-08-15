/*=========================================================*/
/* Module m_ville pour INF147 - Été 2025 */
/* Salif Diarra et Léo Bouamrane */
/*=========================================================*/
#include "m_ville.h"
#include "m_liste_personnes.h"
#include "m_R2.h"
#include "Constante.h"
#include "m_ensemble_noms.h"
#include "m_matrice_transition.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*=========================================================*/

t_ville init_ville(const char* nom_ville, int largeur, int hauteur, int taille_pop_initiale, double proportion_confinement, double prob_emigrer, int nb_hre_transit) {
	t_ville ville = malloc(sizeof(struct ville));
	if (ville == NULL) return NULL;

	ville->hauteur = hauteur;
	ville->largeur = largeur;
	ville->prob_emigrer = prob_emigrer;
	ville->proportion_confinement = proportion_confinement;
	ville->nb_hre_transit = nb_hre_transit;
	ville->migrants = init_liste_migrants();
	ville->nb_migrants_in = 0;
	ville->nb_migrants_out = 0;
	ville->nb_morts_transit = 0;
	ville->population = creer_liste_personnes(taille_pop_initiale);

	//Copier le nom de la ville dans la structure ainsi que ensemble_nom
	strcpy(ville->nom_ville, nom_ville);
	assert(ajouter_nom_ville(nom_ville) == 1);

	//Initialiser le fichier d'écriture
	ville->logfile = fopen(strcat(nom_ville, EXTENSION_FICHIER), "w");
	if (!ville->logfile) {
		printf("Erreur : impossible d'ouvrir le fichier logfile.%s\n", nom_ville);
		fclose(ville->nom_ville);
		return 0;
	}

	return ville;
}

/*=========================================================*/
/* INFORMATRICES
/*=========================================================*/
//const char* get_nom_ville(t_ville ville) { return ville->nom_ville; }

int get_largeur(t_ville ville) { return ville->largeur; }

int get_hauteur(t_ville ville) { return ville->hauteur; }

double get_proportion_confinement(t_ville ville) { return ville->proportion_confinement; }

double get_prob_emigrer(t_ville ville) { return ville->prob_emigrer; }

t_liste_personnes get_population(t_ville ville) { return ville->population; }

t_liste_migrants get_migrants(t_ville ville) { return ville->migrants; }

int get_nb_hre_transit(t_ville ville) { return ville->nb_hre_transit; }

int get_nb_migrants_out(t_ville ville) { return ville->nb_migrants_out; }

int get_nb_migrants_in(t_ville ville) { return ville->nb_migrants_in; }

int get_nb_morts_transit(t_ville ville) { return ville->nb_morts_transit; }

FILE* get_logfile(t_ville ville) { return ville->logfile; }

/*=========================================================*/
/* MUTATRICES
/*=========================================================*/
void inoculer_ville(t_ville ville) {
	creer_patient_zero(&ville->population);
}

//Pas terminé
void simuler_une_heure_pandemie_ville(t_ville ville) {

	//Deux derniers params temporairement à NULL. WIP.
	simuler_une_heure_pandemie(&ville->population, ville->largeur, ville->hauteur, NULL, NULL);

}

int obtenir_des_personnes_ville(t_ville ville) {
	t_migrant migrant_cour;
	t_personne personne_cour;
	t_R2 nouvelle_pos;
	t_R2 nouvelle_vit;
	int ctr_migrants_in = 0;
	double norme_vit = NORME_VIT_MIN + randf() * (NORME_VIT_MAX - NORME_VIT_MIN);
	double angle = randf() * 2 * M_PI; /* Angle entre [0, 2pi] */

	aller_debut_liste_migrants(ville->migrants);

	for (int i = 0; i < get_dans_liste_migrants(ville->migrants); ++i) {
		get_valeur_liste_migrants(ville->migrants, &migrant_cour);

		//Vérifier si le migrant est dans sa ville de destination.
		if (comparer_noms_villes(get_destination_migrant(&migrant_cour), ville->nom_ville) == 1) {
			nouvelle_pos = R2_nouveau(randf() * ville->largeur, randf() * ville->hauteur);
			nouvelle_vit = R2_nouveau(norme_vit * cos(angle), norme_vit * sin(angle));
			personne_cour = get_personne_migrant(&migrant_cour);

			supprimer_position_liste_migrants(ville->migrants);
			modifier_position_personne(&personne_cour, nouvelle_pos);
			modifier_vitesse_personne(&personne_cour, nouvelle_vit);
			modifier_prob_deplacer(&personne_cour, ville->proportion_confinement);

			ajouter_une_personne(&ville->population, &personne_cour);
			++ville->nb_migrants_in;
			++ctr_migrants_in;
		}
	}

	return ctr_migrants_in;
}

int obtenir_des_migrants_ville(t_ville ville) {
	t_personne personne_cour;
	t_migrant nouveau_migrant;
	int ville_dest = get_destination_transition(get_position_ville(ville->nom_ville));
	int ctr_migrants_out = 0;

	if (randf() < ville->prob_emigrer) {
		for (int i = 0; i < randi(MAX_BORNE_EMIGRER); i++) {
			enlever_une_personne(&ville->population, &personne_cour);
			nouveau_migrant = init_migrant(&personne_cour, get_position_ville(ville->nom_ville), ville_dest, 0);

			inserer_liste_migrants(&ville->migrants, &nouveau_migrant);
			++ville->nb_migrants_out;
			++ctr_migrants_out;
		}
	}

	return ctr_migrants_out;
}

int transferer_des_migrants_entre_villes(t_ville src, t_ville dest) {
	t_migrant migrant_cour;
	int ctr_migrants_transf = 0;
	
	if (get_position_ville(dest) == get_position_ville(src) + 1) {
		
		aller_debut_liste_migrants(src->migrants);
		for (int i = 0; i < get_dans_liste_migrants(src->migrants); ++i) {
			get_valeur_liste_migrants(src->migrants, &migrant_cour);
			
			if (get_hrs_transit(&migrant_cour) == 0 && comparer_noms_villes(get_destination_migrant(&migrant_cour), dest->nom_ville) == 0) {
				supprimer_liste_migrants(src->migrants);
				set_transit_migrant(&migrant_cour, dest->nb_hre_transit);
				inserer_liste_migrants(&dest->migrants, &migrant_cour);
				++ctr_migrants_transf;
			}
		}
	}

	return ctr_migrants_transf;
}

void ecrire_logfile_ville(t_ville ville) {
	fprintf(ville->logfile, "Ville ID: %d\n", ville->nom_ville);
	fprintf(ville->logfile, "Population: %d\n", ville->population);
	fprintf(ville->logfile, "Migrants entrant: %d\n", ville->nb_migrants_in);
	fprintf(ville->logfile, "Migrants morts en transit: %d\n", ville->nb_morts_transit);
	fprintf(ville->logfile, "----------------\n");
}

void detruire_ville(t_ville ville) {
	// Libérer la population
	liberer_liste(&ville->population);

	// Libérer la liste des migrants
	detruire_liste_migrants(ville->migrants);
	
	// Fermer le fichier de log
	if (ville->logfile != NULL) {
		fclose(ville->logfile);
		ville->logfile = NULL;
	}
}