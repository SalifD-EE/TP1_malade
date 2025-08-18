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


#if 0
int main(void) {
	srand_sys();

    // Initialiser les données de test
	t_personne personne_arrivante = init_personne(40, 40, 0.4);
	t_migrant migrant_arrivant = init_migrant(&personne_arrivante, 1, 0, 0);
	
	t_personne personne_arrivante2 = init_personne(40, 40, 0.4);
	t_migrant migrant_arrivant2 = init_migrant(&personne_arrivante2, 1, 0, 0);
	
	init_ensemble_noms_villes(10);

    t_ville ville1 = init_ville("VilleTest1", 100, 100, 50, 0.5, 1, 5);
    t_ville ville2 = init_ville("VilleTest2", 80, 80, 30, 0.3, 0, 3);
	t_ville ville3 = init_ville("VilleTest3", 100, 100, 5, 0.5, 1, 5);
	t_ville ville_vide = init_ville("VilleVide", 50, 50, 10, 0.2, 0, 2);
	
	ajouter_nom_ville("VilleTest1");
	ajouter_nom_ville("VilleTest2");
	ajouter_nom_ville("VilleTest3");
	ajouter_nom_ville("VilleVide");
	
    assert(ville1);
    assert(ville2);
	assert(ville3);
	assert(ville_vide);

    // Ajouter une population initiale aux deux villes
    assert(ajouter_des_personnes(&ville1->population, 20, 100, 100, 0.5) == 20);
    assert(ajouter_des_personnes(&ville2->population, 15, 80, 80, 0.3) == 15);

    // Tester inoculer_ville
    assert(get_nb_malades(&ville1->population) == 0);
    inoculer_ville(ville1);
    assert(get_nb_malades(&ville1->population) == 1);
    assert(get_nb_sains(&ville1->population) == 19);

    assert(get_nb_malades(&ville2->population) == 0);
    inoculer_ville(ville2);
    assert(get_nb_malades(&ville2->population) == 1);
    assert(get_nb_sains(&ville2->population) == 14);

	// Tester obtenir_des_personnes_ville
	// Ajouter un migrant destiné à ville1
	assert(inserer_liste_migrants(ville1->migrants, &migrant_arrivant));
	assert(obtenir_des_personnes_ville(ville1) == 1);
	assert(get_nb_personnes(&ville1->population) == 21);
	assert(ville1->nb_migrants_in == 1);
	assert(est_vide_liste_migrants(ville1->migrants) == 1);
	assert(consulter_liste_migrants(ville1->migrants, &migrant_arrivant2) == 0);

    // Tester obtenir_des_migrants_ville
    assert(obtenir_des_migrants_ville(ville1) == 5);
    assert(ville1->nb_migrants_out == 5);
	assert(get_nb_personnes(&ville1->population) == 16);

	//Mettre les heures de transit à 0 pour forcer un transfert.
	//Plus, test de traiter_heures_transit
	for (int i = 0; i < 5; i++) {
		traiter_heures_transit(ville1->migrants);
	}

	// Tester transferer_des_migrants_entre_villes
    assert(transferer_des_migrants_entre_villes(ville1, ville2) == 5);

    // Tester avec une liste de migrants vide
    assert(obtenir_des_personnes_ville(ville_vide) == 0);
    assert(obtenir_des_migrants_ville(ville_vide) == 0);
    assert(transferer_des_migrants_entre_villes(ville_vide, ville1) == 0);
	
	// Tester traiter_heures_maladie_migrants
	ajouter_des_personnes(&ville3->population, 5, 100, 100, 0.1);
	for (int j = 0; j < 5; j++) {
		//Évidemment à proscrire en utilisation normale. Je brise l'encapsulation pour les tests.
		modifier_etat_personne(&ville3->population.liste[j], MALADE, ville3->proportion_confinement);
	}
	assert(obtenir_des_migrants_ville(ville3) == 5);
	
	aller_debut_liste_migrants(ville3->migrants);
	t_migrant migrant; 
	for (int k = 0; k < NB_HRS_MALADIE - 1; k++) {
		traiter_heures_maladie_migrants(ville3->migrants, ville3->proportion_confinement);
		get_valeur_liste_migrants(ville3->migrants, &migrant);
		assert(get_hrs_maladie(&migrant.voyageur) == k + 1);
		avancer_liste_migrants(ville3->migrants);
	}

	// Tester simuler_une_heure_pandemie_ville + ctr_mort_transit
	simuler_une_heure_pandemie_ville(ville3);

    // Après simulation, nous devrions toujours avoir un nombre valide de malades
    assert(get_nb_malades(&ville1->population) >= 0);
    assert(get_nb_personnes(&ville1->population) >= get_nb_morts(&ville1->population));

	// Tester la simulation sur une ville vide
	simuler_une_heure_pandemie_ville(ville_vide);
	assert(get_nb_personnes(&ville_vide->population) >= 0);

    // Tester les écritures finales de log
    ecrire_logfile_ville(ville1);
    ecrire_logfile_ville(ville2);
	ecrire_logfile_ville(ville3);
    ecrire_logfile_ville(ville_vide);

    // Tester detruire_ville (ceci devrait être en dernier)
    detruire_ville(ville1);
    detruire_ville(ville2);
	detruire_ville(ville3);
    detruire_ville(ville_vide);

	system("pause");
	return EXIT_SUCCESS;
	
}
#endif