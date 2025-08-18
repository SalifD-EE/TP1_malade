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
#include "m_groupe_villes.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*=========================================================*/

t_ville init_ville(const char* nom_ville, int largeur, int hauteur, int taille_pop_initiale, 
	double proportion_confinement, double prob_emigrer, int nb_hre_transit) {
	
	t_ville ville = malloc(sizeof(struct ville));
	char* nom_logfile;

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
	ville->infection_heure = 0;
	ville->morts_heure = 0;
	ville->population = creer_liste_personnes(taille_pop_initiale);
	
	//Populer la ville
	ajouter_des_personnes(&ville->population, taille_pop_initiale, largeur, hauteur, proportion_confinement);

	//Copier le nom de la ville dans la structure ainsi que ensemble_nom
	strcpy(ville->nom_ville, nom_ville);
	assert(ajouter_nom_ville(nom_ville) == 1);

	//Initialiser le fichier d'écriture
	nom_logfile = (char*)malloc((strlen(nom_ville) + strlen(EXTENSION_FICHIER) + 1) * sizeof(char));
	strcpy(nom_logfile, nom_ville);
	strcat(nom_logfile, EXTENSION_FICHIER);

	ville->logfile = fopen(nom_logfile, "w");
	if (!ville->logfile) {
		printf("Erreur : impossible d'ouvrir le fichier logfile.%s\n", nom_ville);
		fclose(ville->nom_ville);
		return 0;
	}
	
	free(nom_logfile);

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

void simuler_une_heure_pandemie_ville(t_ville ville) {
	simuler_une_heure_pandemie(&ville->population, ville->largeur, ville->hauteur, &ville->infection_heure, &ville->morts_heure);
	traiter_heures_transit(ville->migrants);
	ville->nb_morts_transit += traiter_heures_maladie_migrants(ville->migrants, ville->proportion_confinement);
}

static void traiter_heures_transit(t_liste_migrants liste) {
	aller_debut_liste_migrants(liste);
	for (int i = 0; i < get_dans_liste_migrants(liste); i++) {
		dec_hrs_transit_liste_migrants(liste);
		avancer_liste_migrants(liste);
	}
}

static int traiter_heures_maladie_migrants(t_liste_migrants liste, double proportion_confinement) {
	int ctr_morts_transit = 0;
	
	//Parcourir à l'envers pour éviter les ennuis lors de suppression
	for (int i = get_dans_liste_migrants(liste) - 1; i >= 0; --i) {
		positionner_liste_migrants(liste, i);

		if (assurer_temps_maladie_migrants(liste) == 1) {
			/*Si le migrant meurt en transit, incrémenter le nombre de morts 
				en transit et le retirer de la liste de migrants*/
			++ctr_morts_transit;
			supprimer_liste_migrants(liste);
		}
	}

	return ctr_morts_transit;
}

int obtenir_des_personnes_ville(t_ville ville) {
	t_migrant migrant_cour;
	t_personne personne_cour;
	t_R2 nouvelle_pos;
	t_R2 nouvelle_vit;
	int ctr_migrants_in = 0;
	double norme_vit = NORME_VIT_MIN + randf() * (NORME_VIT_MAX - NORME_VIT_MIN);
	double angle = randf() * 2 * M_PI; /* Angle entre [0, 2pi] */
	int dest_migrant = DESTINATION_ABSENTE;

	aller_debut_liste_migrants(ville->migrants);

	for (int i = 0; i < get_dans_liste_migrants(ville->migrants); ++i) {
		get_valeur_liste_migrants(ville->migrants, &migrant_cour);
		dest_migrant = get_destination_migrant(&migrant_cour);

		//Vérifier si le migrant est dans sa ville de destination.
		if (dest_migrant != DESTINATION_ABSENTE && dest_migrant == get_position_ville(ville->nom_ville)) {

			nouvelle_pos = R2_nouveau(randf() * ville->largeur, randf() * ville->hauteur);
			nouvelle_vit = R2_nouveau(norme_vit * cos(angle), norme_vit * sin(angle));
			personne_cour = get_personne_migrant(&migrant_cour);

			supprimer_liste_migrants(ville->migrants);
			modifier_position_personne(&personne_cour, nouvelle_pos);
			modifier_vitesse_personne(&personne_cour, nouvelle_vit);
			modifier_prob_deplacer(&personne_cour, ville->proportion_confinement);

			ajouter_une_personne(&ville->population, &personne_cour);
			++ville->nb_migrants_in;
			++ctr_migrants_in;
		}
		avancer_liste_migrants(ville->migrants);
	}

	return ctr_migrants_in;
}

int obtenir_des_migrants_ville(t_ville ville) {
	t_personne personne_cour;
	t_migrant nouveau_migrant;

	int ville_dest = get_destination_transition(get_position_ville(ville->nom_ville));
	int ctr_migrants_out = 0;


	if (ville_dest != DESTINATION_ABSENTE && randf() < ville->prob_emigrer) {
		for (int i = 0; i < randi(MAX_BORNE_EMIGRER); i++) {
			enlever_une_personne(&ville->population, &personne_cour);
			nouveau_migrant = init_migrant(&personne_cour, get_position_ville(ville->nom_ville), ville_dest, ville->nb_hre_transit);

			inserer_liste_migrants(ville->migrants, &nouveau_migrant);
			++ville->nb_migrants_out;
			++ctr_migrants_out;
		}
	}

	return ctr_migrants_out;
}

int transferer_des_migrants_entre_villes(t_ville src, t_ville dest) {
	t_migrant migrant_cour;
	int ctr_migrants_transf = 0;
	int nb_migrants_src = 0;

	if (get_position_ville(dest) == get_position_ville(src) + 1) {
		
		for (int i = get_dans_liste_migrants(src->migrants) - 1; i >= 0; --i) {
			positionner_liste_migrants(src->migrants, i);
			get_valeur_liste_migrants(src->migrants, &migrant_cour);
			
			if (get_hrs_transit(&migrant_cour) == 0 && comparer_noms_villes(get_nom_ville(get_destination_migrant(&migrant_cour)), src->nom_ville) == 0) {
				supprimer_liste_migrants(src->migrants);
				set_transit_migrant(&migrant_cour, dest->nb_hre_transit);
				inserer_liste_migrants(dest->migrants, &migrant_cour);
				++ctr_migrants_transf;
			}
		}
	}

	return ctr_migrants_transf;
}

void ecrire_logfile_ville(t_ville ville) {
	fprintf(ville->logfile, "Nom: %s\n", ville->nom_ville);
	fprintf(ville->logfile, "Population: %d\n", get_nb_personnes(&ville->population));
	fprintf(ville->logfile, "Migrants entrants: %d\n", ville->nb_migrants_in);
    fprintf(ville->logfile, "Migrants sortants: %d\n", ville->nb_migrants_out);
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