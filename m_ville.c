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

	//Copier le nom de la ville dans la structure ainsi que ensemble_nom
	strcpy(ville->nom_ville, nom_ville);
	assert(ajouter_nom_ville(nom_ville) == 1);

	//Initialiser le fichier d'écriture

	nom_logfile = (char*)malloc((strlen(nom_ville) + strlen(EXTENSION_FICHIER) + 1) * sizeof(char));
	strcpy(nom_logfile, nom_ville);  // Use strcpy for the first string, not strcat
	strcat(nom_logfile, EXTENSION_FICHIER);

	ville->logfile = fopen(nom_logfile, "w");
	
	if (!ville->logfile) {
		printf("Erreur : impossible d'ouvrir le fichier logfile.%s\n", nom_ville);
		fclose(ville->nom_ville);
		return 0;
	}
	
	free(nom_logfile);  // Don't forget to free the allocated memory

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
	
	aller_debut_liste_migrants(liste);

	for (int i = 0; i < get_dans_liste_migrants(liste); i++) {
		
		//Si la maladie du migrant est terminée, terminer sa maladie.
		if (assurer_temps_maladie_migrants(liste)) {
			
			/*Si le migrant meurt en transit, incrémenter le nombre de morts en transit
				et le retirer de la liste de migrants*/
			if (terminer_maladie_migrants(liste, proportion_confinement)) {
				++ctr_morts_transit;
				supprimer_liste_migrants(liste);
			}
		}
		avancer_liste_migrants(liste);
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

	aller_debut_liste_migrants(ville->migrants);

	for (int i = 0; i < get_dans_liste_migrants(ville->migrants); ++i) {
		get_valeur_liste_migrants(ville->migrants, &migrant_cour);

		//Vérifier si le migrant est dans sa ville de destination.
		if (comparer_noms_villes(get_nom_ville(get_destination_migrant(&migrant_cour)), ville->nom_ville) == 1) {
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
			
			if (get_hrs_transit(&migrant_cour) == 0 && comparer_noms_villes(get_nom_ville(get_destination_migrant(&migrant_cour)), dest->nom_ville) == 0) {
				supprimer_liste_migrants(src->migrants);
				set_transit_migrant(&migrant_cour, dest->nb_hre_transit);
				inserer_liste_migrants(&dest->migrants, &migrant_cour);
				++ctr_migrants_transf;
			}
			avancer_liste_migrants(src->migrants);
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

#if 0
int main(void) {
	init_ensemble_noms_villes(10);
	
	// Initialize test data
    t_ville ville1 = init_ville("TestVille1", 100, 100, 50, 0.5, 0.1, 5);
    t_ville ville2 = init_ville("TestVille2", 80, 80, 30, 0.3, 0.2, 3);

    assert(ville1);
    assert(ville2);

    // Add initial population to both cities
    assert(ajouter_des_personnes(&ville1->population, 20, 100, 100, 0.5) == 20);
    assert(ajouter_des_personnes(&ville2->population, 15, 80, 80, 0.3) == 15);

    // Test inoculer_ville
    assert(get_nb_malades(&ville1->population) == 0);
    inoculer_ville(ville1);
    assert(get_nb_malades(&ville1->population) == 1);
    assert(get_nb_sains(&ville1->population) == 19);

    // Test with second city
    assert(get_nb_malades(&ville2->population) == 0);
    inoculer_ville(ville2);
    assert(get_nb_malades(&ville2->population) == 1);
    assert(get_nb_sains(&ville2->population) == 14);

    // Test simuler_une_heure_pandemie_ville
    int initial_malades = get_nb_malades(&ville1->population);
    simuler_une_heure_pandemie_ville(ville1);
    // After simulation, we should still have the same or different number of sick people
    assert(get_nb_malades(&ville1->population) >= 0);
    assert(get_nb_personnes(&ville1->population) >= get_nb_morts(&ville1->population));

    // Test obtenir_des_migrants_ville
    int migrants_out_before = ville1->nb_migrants_out;
    int migrants_created = obtenir_des_migrants_ville(ville1);
    assert(migrants_created >= 0);
    assert(ville1->nb_migrants_out >= migrants_out_before);

    // Create some test migrants manually for testing
    t_personne test_personne1 = init_personne(50, 50, 0.5);
    t_personne test_personne2 = init_personne(60, 60, 0.3);
    t_migrant test_migrant1 = init_migrant(&test_personne1, 0, 1, 2);
    t_migrant test_migrant2 = init_migrant(&test_personne2, 1, 0, 0);

    // Add migrants to ville1's migrant list
    assert(inserer_liste_migrants(ville1->migrants, &test_migrant1));
    assert(inserer_liste_migrants(ville1->migrants, &test_migrant2));
    assert(get_dans_liste_migrants(ville1->migrants) >= 2);

    // Test transferer_des_migrants_entre_villes
    int migrants_ville1_before = get_dans_liste_migrants(ville1->migrants);
    int migrants_ville2_before = get_dans_liste_migrants(ville2->migrants);
    int transferred = transferer_des_migrants_entre_villes(ville1, ville2);
    assert(transferred >= 0);

    // Test obtenir_des_personnes_ville
    // Add a migrant destined for ville1
    t_personne arriving_personne = init_personne(40, 40, 0.4);
    t_migrant arriving_migrant = init_migrant(&arriving_personne, 1, 0, 0);
    assert(inserer_liste_migrants(ville1->migrants, &arriving_migrant));

    int pop_before = get_nb_personnes(&ville1->population);
    int migrants_in_before = ville1->nb_migrants_in;
    int received = obtenir_des_personnes_ville(ville1);
    assert(received >= 0);
    assert(ville1->nb_migrants_in >= migrants_in_before);

    // Test ecrire_logfile_ville
    assert(ville1->logfile != NULL);
    ecrire_logfile_ville(ville1);
    assert(ville1->logfile != NULL); // Should still be open

    assert(ville2->logfile != NULL);
    ecrire_logfile_ville(ville2);
    assert(ville2->logfile != NULL); // Should still be open

    // Test edge cases

    // Test with empty migrant list
    t_ville empty_ville = init_ville("EmptyVille", 50, 50, 10, 0.2, 0.05, 2);
    assert(empty_ville);
    assert(ajouter_des_personnes(&empty_ville->population, 5, 50, 50, 0.2) == 5);

    // Test functions with empty lists
    assert(obtenir_des_personnes_ville(empty_ville) == 0);
    int empty_migrants = obtenir_des_migrants_ville(empty_ville);
    assert(empty_migrants >= 0);

    // Test simulation on empty ville
    simuler_une_heure_pandemie_ville(empty_ville);
    assert(get_nb_personnes(&empty_ville->population) >= 0);

    // Test transfer with no matching migrants
    int no_transfer = transferer_des_migrants_entre_villes(empty_ville, ville1);
    assert(no_transfer == 0);

    // Test with ville that has migrants but no matching destinations
    t_personne non_matching_personne = init_personne(30, 30, 0.6);
    t_migrant non_matching_migrant = init_migrant(&non_matching_personne, 2, 3, 1);
    assert(inserer_liste_migrants(empty_ville->migrants, &non_matching_migrant));

    int no_match_received = obtenir_des_personnes_ville(ville1);
    assert(no_match_received >= 0);

    // Test with multiple simulation rounds
    for (int i = 0; i < 5; i++) {
        simuler_une_heure_pandemie_ville(ville1);
        simuler_une_heure_pandemie_ville(ville2);
        assert(get_nb_personnes(&ville1->population) >= get_nb_morts(&ville1->population));
        assert(get_nb_personnes(&ville2->population) >= get_nb_morts(&ville2->population));
    }

    // Test multiple migrant operations
    for (int i = 0; i < 3; i++) {
        obtenir_des_migrants_ville(ville1);
        obtenir_des_migrants_ville(ville2);
        transferer_des_migrants_entre_villes(ville1, ville2);
        obtenir_des_personnes_ville(ville1);
        obtenir_des_personnes_ville(ville2);
    }

    // Verify counters are consistent
    assert(ville1->nb_migrants_in >= 0);
    assert(ville1->nb_migrants_out >= 0);
    assert(ville1->nb_morts_transit >= 0);
    assert(ville2->nb_migrants_in >= 0);
    assert(ville2->nb_migrants_out >= 0);
    assert(ville2->nb_morts_transit >= 0);

    // Test final log writes
    ecrire_logfile_ville(ville1);
    ecrire_logfile_ville(ville2);
    ecrire_logfile_ville(empty_ville);

    // Test detruire_ville (this should be last)
    detruire_ville(ville1);
    detruire_ville(ville2);
    detruire_ville(empty_ville);

    // After destruction, pointers should be cleaned up
    // Note: We can't test the pointers directly since they're freed,
    // but the function should not crash when called

    system("pause");
    return EXIT_SUCCESS;
}
#endif

#if 1
int main(void) {
    init_ensemble_noms_villes(10);

    // Initialiser les données de test
    t_ville ville1 = init_ville("VilleTest1", 100, 100, 50, 0.5, 0.1, 5);
    t_ville ville2 = init_ville("VilleTest2", 80, 80, 30, 0.3, 0.2, 3);

    assert(ville1);
    assert(ville2);

    // Ajouter une population initiale aux deux villes
    assert(ajouter_des_personnes(&ville1->population, 20, 100, 100, 0.5) == 20);
    assert(ajouter_des_personnes(&ville2->population, 15, 80, 80, 0.3) == 15);

    // Tester inoculer_ville
    assert(get_nb_malades(&ville1->population) == 0);
    inoculer_ville(ville1);
    assert(get_nb_malades(&ville1->population) == 1);
    assert(get_nb_sains(&ville1->population) == 19);

    // Tester avec la deuxième ville
    assert(get_nb_malades(&ville2->population) == 0);
    inoculer_ville(ville2);
    assert(get_nb_malades(&ville2->population) == 1);
    assert(get_nb_sains(&ville2->population) == 14);

    // Tester simuler_une_heure_pandemie_ville
    int malades_initiaux = get_nb_malades(&ville1->population);
    simuler_une_heure_pandemie_ville(ville1);
    // Après simulation, nous devrions toujours avoir un nombre valide de malades
    assert(get_nb_malades(&ville1->population) >= 0);
    assert(get_nb_personnes(&ville1->population) >= get_nb_morts(&ville1->population));

    // Tester obtenir_des_migrants_ville
    int migrants_sortis_avant = ville1->nb_migrants_out;
    int migrants_crees = obtenir_des_migrants_ville(ville1);
    assert(migrants_crees >= 0);
    assert(ville1->nb_migrants_out >= migrants_sortis_avant);

    // Créer quelques migrants de test manuellement
    t_personne personne_test1 = init_personne(50, 50, 0.5);
    t_personne personne_test2 = init_personne(60, 60, 0.3);
    t_migrant migrant_test1 = init_migrant(&personne_test1, 0, 1, 2);
    t_migrant migrant_test2 = init_migrant(&personne_test2, 1, 0, 0);

    // Ajouter les migrants à la liste de migrants de ville1
    assert(inserer_liste_migrants(ville1->migrants, &migrant_test1));
    assert(inserer_liste_migrants(ville1->migrants, &migrant_test2));
    assert(get_dans_liste_migrants(ville1->migrants) >= 2);

    // Tester transferer_des_migrants_entre_villes
    int migrants_ville1_avant = get_dans_liste_migrants(ville1->migrants);
    int migrants_ville2_avant = get_dans_liste_migrants(ville2->migrants);
    int transferes = transferer_des_migrants_entre_villes(ville1, ville2);
    assert(transferes >= 0);

    // Tester obtenir_des_personnes_ville
    // Ajouter un migrant destiné à ville1
    t_personne personne_arrivante = init_personne(40, 40, 0.4);
    t_migrant migrant_arrivant = init_migrant(&personne_arrivante, 1, 0, 0);
    assert(inserer_liste_migrants(ville1->migrants, &migrant_arrivant));

    int pop_avant = get_nb_personnes(&ville1->population);
    int migrants_entres_avant = ville1->nb_migrants_in;
    int recus = obtenir_des_personnes_ville(ville1);
    assert(recus >= 0);
    assert(ville1->nb_migrants_in >= migrants_entres_avant);

    // Tester ecrire_logfile_ville
    assert(ville1->logfile != NULL);
    ecrire_logfile_ville(ville1);
    assert(ville1->logfile != NULL); // Devrait toujours être ouvert

    assert(ville2->logfile != NULL);
    ecrire_logfile_ville(ville2);
    assert(ville2->logfile != NULL); // Devrait toujours être ouvert

    // Tester les cas limites

    // Tester avec une liste de migrants vide
    t_ville ville_vide = init_ville("VilleVide", 50, 50, 10, 0.2, 0.05, 2);
    assert(ville_vide);
    assert(ajouter_des_personnes(&ville_vide->population, 5, 50, 50, 0.2) == 5);

    // Tester les fonctions avec des listes vides
    assert(obtenir_des_personnes_ville(ville_vide) == 0);
    int migrants_vides = obtenir_des_migrants_ville(ville_vide);
    assert(migrants_vides >= 0);

    // Tester la simulation sur une ville vide
    simuler_une_heure_pandemie_ville(ville_vide);
    assert(get_nb_personnes(&ville_vide->population) >= 0);

    // Tester le transfert sans migrants correspondants
    int pas_de_transfert = transferer_des_migrants_entre_villes(ville_vide, ville1);
    assert(pas_de_transfert == 0);

    // Tester avec une ville qui a des migrants mais pas de destinations correspondantes
    t_personne personne_non_correspondante = init_personne(30, 30, 0.6);
    t_migrant migrant_non_correspondant = init_migrant(&personne_non_correspondante, 2, 3, 1);
    assert(inserer_liste_migrants(ville_vide->migrants, &migrant_non_correspondant));

    int pas_de_correspondance = obtenir_des_personnes_ville(ville1);
    assert(pas_de_correspondance >= 0);

    // Tester avec plusieurs tours de simulation
    for (int i = 0; i < 5; i++) {
        simuler_une_heure_pandemie_ville(ville1);
        simuler_une_heure_pandemie_ville(ville2);
        assert(get_nb_personnes(&ville1->population) >= get_nb_morts(&ville1->population));
        assert(get_nb_personnes(&ville2->population) >= get_nb_morts(&ville2->population));
    }

    // Tester plusieurs opérations de migration
    for (int i = 0; i < 3; i++) {
        obtenir_des_migrants_ville(ville1);
        obtenir_des_migrants_ville(ville2);
        transferer_des_migrants_entre_villes(ville1, ville2);
        obtenir_des_personnes_ville(ville1);
        obtenir_des_personnes_ville(ville2);
    }

    // Vérifier que les compteurs sont cohérents
    assert(ville1->nb_migrants_in >= 0);
    assert(ville1->nb_migrants_out >= 0);
    assert(ville1->nb_morts_transit >= 0);
    assert(ville2->nb_migrants_in >= 0);
    assert(ville2->nb_migrants_out >= 0);
    assert(ville2->nb_morts_transit >= 0);

    // Tester les écritures finales de log
    ecrire_logfile_ville(ville1);
    ecrire_logfile_ville(ville2);
    ecrire_logfile_ville(ville_vide);

    // Tester detruire_ville (ceci devrait être en dernier)
    detruire_ville(ville1);
    detruire_ville(ville2);
    detruire_ville(ville_vide);


    system("pause");
    return EXIT_SUCCESS;
}
#endif