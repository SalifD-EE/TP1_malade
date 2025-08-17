/*---------------------------------------------------*/
/* 	par husiro 	*/
/* 	modifié par salif diarra et léo bouamrane 	     */
/*---------------------------------------------------*/

#include "m_liste_migrants.h"

/*-----------------------------------------------------------------*/
/* LE NOEUD de ces listes est complet				   */
/*-----------------------------------------------------------------*/
/* la liste sera simplement chaînée, chaque noeud a un lien
	un sur le noeud qui suit et un sur celui qui précède.
	invariants de la liste :
		le queue noeud a son next à NULL
*/

struct noeud_migrant {
	t_el_liste_migrants  valeur;
	t_noeud_migrant next;
};
/*-----------------------------------------------------------------*/

#if (LISTE_CACHER)
/*-----------------------------------------------------------------*/
struct liste_migrants {

	/* la taille actuelle de la liste */
	int  taille;

	t_noeud_migrant  iterateur;
	
	/* position de l'itérateur*/
	int  position;

	/*pointeur du premier noeud*/
	t_noeud_migrant tete;

	/* pointeur du dernier noeud*/
	t_noeud_migrant  queue;

	/* un  invariant de la liste
	Si elle n'est pas vide,
	l'itérateur actuel, tete et queue  ne seront jamais NULL
	*/
};
#endif


/*-----------------------------------------------------------------*/
/*	 fonction privée,   constructeur du noeud complet
	fera le noeud voulu à inserer à la liste
	simple et efficace ainsi , les trois champs sont
	correctement traités.
	observez-le bien
*/

static t_noeud_migrant init_noeud(const t_el_liste_migrants* src, t_noeud_migrant  next) {
	/*on obtient la mémoire*/
	t_noeud_migrant temp = malloc(sizeof(struct noeud_migrant));
	if (temp == NULL) return NULL;

	/*copier la source, ajuster les deux pointeurs */
	temp->valeur = *src;
	temp->next = next;
	return temp;
}

// privé au module
// retourne l'adresse du noeud à cette position dans la liste
static t_noeud_migrant get_noeud(const t_liste_migrants liste, int position) {
	t_noeud_migrant temp = liste->tete;
	int i;
	assert(position < liste->taille);
	assert(!(position < 0));

	for (i = 0; i < position; ++i) { (temp = temp->next); }
	return temp;
}

/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/*  L'IMPLÉMENTATION de  listeD00  */
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/* le constructeur */
t_liste_migrants init_liste_migrants(void) {
	t_liste_migrants nouveau = malloc(sizeof(struct liste_migrants));
	/* la nouvelle liste est vide */
	if (nouveau) {
		nouveau->tete = nouveau->queue = nouveau->iterateur = NULL;
		nouveau->taille = 0;
		nouveau->position = -1;
	}
	return nouveau;
}
/*-----------------------------------------------------------------*/
/* le destructeur */
void detruire_liste_migrants(t_liste_migrants liste) {
	if (liste) {
		/*  liberer tous les noeuds*/
		vider_liste_migrants(liste);
		/*on libere aussi  la structure liste */
		free(liste);
	}
}
/*-----------------------------------------------------------------*/
/* LES INFORMATRICES ÉLÉMENTAIRES  */

 /*consulter la valeur de l'itérateur*/
int get_valeur_liste_migrants(const t_liste_migrants liste, t_el_liste_migrants* dest) {
	if (liste->taille == 0) return 0;

	*dest = liste->iterateur->valeur;
	return 1;
}
/*-----------------------------------------------------------------*/
int est_vide_liste_migrants(const t_liste_migrants liste) {
	return liste->taille == 0;
}
/*-----------------------------------------------------------------*/
int get_dans_liste_migrants(const t_liste_migrants liste) {
	return liste->taille;
}
/*-----------------------------------------------------------------*/
int get_position_liste_migrants(const t_liste_migrants liste) {
	return liste->position;
}
/*-----------------------------------------------------------------*/


/*-----------------------------------------------------------------*/
/* LES CINQ FONCTIONS DE POSITIONNEMENT */
/*-----------------------------------------------------------------*/
/* 1 si avance , 0 sinon */
int avancer_liste_migrants(t_liste_migrants liste) {
	/*si la liste != vide et si l'itérateur n'est pas à la fin */
	if (liste->iterateur == liste->queue) return 0;

	/* l'itérateur passe au next */
	liste->iterateur = liste->iterateur->next;
	++liste->position;
	return 1;

}
/*-----------------------------------------------------------------*/

///*-----------------------------------------------------------------*/
/* 0 si vide , 1 sinon */
int aller_debut_liste_migrants(t_liste_migrants liste) {
	/*si la listeD00 est vide,  0 */
	if (liste->iterateur == NULL) return 0;

	/*l'itérateur va au début */
	liste->iterateur = liste->tete;
	liste->position = 0;
	return 1;
}
/*-----------------------------------------------------------------*/
/* 0 si vide , 1 sinon */
int aller_fin_liste_migrants(t_liste_migrants liste) {
	/*si la listeD00 est vide */
	if (liste->iterateur == NULL) return 0;

	/*  l'itérateur ira à la fin */
	liste->iterateur = liste->queue;
	liste->position = liste->taille - 1;
	return 1;

}

/*-----------------------------------------------------------------*/
int positionner_liste_migrants(t_liste_migrants liste, int position) {

	/* si la position est inatteignable  .*/
	if ((position < 0) || (position >= liste->taille)) return 0;

	// si la liste est vide
	if (!aller_debut_liste_migrants(liste)) return 0;

	while (liste->position < position) avancer_liste_migrants(liste);

	return 1;
}

/*-----------------------------------------------------------------*/


/*-----------------------------------------------------------------*/
// ATTEINDRE LE CONTENU D'UN NOEUD, consulter ou modifier
int consulter_liste_migrants(const t_liste_migrants liste, t_el_liste_migrants* dest) {
	if (liste->iterateur) {
		*dest = liste->iterateur->valeur;
		return 1;
	}
	return 0;
}
/*-----------------------------------------------------------------*/
t_el_liste_migrants* modifier_liste_migrants(t_liste_migrants liste) {

	if (liste->iterateur) {
		return &(liste->iterateur->valeur);
	}
	return NULL;
}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
// INSERER
/*-----------------------------------------------------------------*/
/*
Pour inserer un noeud,  on considère

CAS 0 la liste est vide, static  inserer_premier
CAS 2 si l'itérateur est sur la tete inserer au debut
CAS 3 si l'itérateur est sur le queue inserer à la fin
CAS 4 sinon inserer, static inserer_interieur

*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/* CAS 0  si la liste_migrants est vide, tous les membres sont ajustés */
static int inserer_premier_noeud(t_liste_migrants liste, const t_el_liste_migrants* src) {
	t_noeud_migrant temp = init_noeud(src, NULL);
	if (temp) {
		liste->tete = liste->queue = liste->iterateur = temp;
		liste->taille = 1;
		liste->position = 0;
		return 1;
	}
	return 0;

}
/*-----------------------------------------------------------------*/
/*	CAS 1 inserer au début  */
int inserer_debut_liste_migrants(t_liste_migrants liste, const t_el_liste_migrants* src) {
	t_noeud_migrant temp;

	if (liste->taille == 0)
		return inserer_premier_noeud(liste, src);

	temp = init_noeud(src, liste->tete);

	if (temp) {
		//list->tete->preced = temp;
		liste->tete = temp;
		liste->taille += 1;

		aller_debut_liste_migrants(liste);
		return 1;
	}
	return 0;
}
/*-----------------------------------------------------------------*/
/*	CAS 2 inserer a la fin  de la liste */
int inserer_fin_liste_migrants(t_liste_migrants liste, const t_el_liste_migrants* src) {
	t_noeud_migrant temp;
	if (liste->taille == 0)
		return inserer_premier_noeud(liste, src);

	temp = init_noeud(src, NULL);

	if (temp) {

		liste->queue->next = temp;

		liste->queue = temp;
		liste->taille += 1;

		aller_fin_liste_migrants(liste);
		return 1;
	}
	return 0;

}
/*-----------------------------------------------------------------*/

/*CAS #3 inserer à l'intérieur de la liste */
static int inserer_position_liste_migrants(t_liste_migrants liste, const t_el_liste_migrants* src) {
	t_noeud_migrant temp;

	//Positionner l'itérateur au noeud qui PRÉCÈDE celui où on veut insérer.
	if(!positionner_liste_migrants(liste, liste->position - 1)) return 0;

	//Le nouveau noeud pointe vers celui qui suit l'ancien noeud à cette position.
	temp = init_noeud(src, liste->iterateur->next);
	if (temp) {
		//L'ancien noeud à cette position pointe vers le nouveau noeud.
		liste->iterateur->next = temp;
		avancer_liste_migrants(liste);
		liste->taille += 1;
		return 1;
	}

	return 0;
}
/*-----------------------------------------------------------------*/

int inserer_liste_migrants(t_liste_migrants liste, const t_el_liste_migrants* src) {

	if (liste->taille == 0)
		return inserer_premier_noeud(liste, src);
	if (liste->iterateur == liste->tete)
		return inserer_debut_liste_migrants(liste, src);
	if (liste->iterateur == liste->queue)
		return inserer_fin_liste_migrants(liste, src);

	return inserer_position_liste_migrants(liste, src);

}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*
	Pour supprimer un noeud,  on considère
	CAS 0 la liste est vide, return 0
	CAS #1 si la liste a un seul noeud, on la vide
	CAS #2 si l'itérateur est sur la tete
	CAS #3 si l'itérateur est sur le queue
	CAS #4 on supprime  un noeud  à l'intérieur

*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/*  supprimer au debut et l'iterateur ira en tete */
int supprimer_debut_liste_migrants(t_liste_migrants liste) {

	t_noeud_migrant temp = liste->tete;

	if (liste->taille == 0) return 0;

	if (liste->taille == 1) {
		return vider_liste_migrants(liste);
	}

	/* on avance tete  */
	liste->tete = liste->tete->next;

	//list->tete->preced = NULL;

	free(temp);
	liste->taille -= 1;

	// placer  l'iterateur au debut
	aller_debut_liste_migrants(liste);
	return 1;
}
/*-----------------------------------------------------------------*/
/*  supprimer a la fin et l'iterateur ira en queue */
int supprimer_fin_liste_migrants(t_liste_migrants liste) {

	t_noeud_migrant temp = liste->queue;

	if (liste->taille == 0) return 0;
	if (liste->taille == 1) {
		vider_liste_migrants(liste);
		return 1;
	}

	//list->queue = list->queue->preced;
	liste->queue->next = NULL;
	liste->taille -= 1;
	free(temp);

	// placer  l'iterateur à la fin
	aller_fin_liste_migrants(liste);
	return 1;

}
/*-----------------------------------------------------------------*/
/*  ici on traite le cas #4 */
static int supprimer_position_liste_migrants(t_liste_migrants liste) {
	t_noeud_migrant precedent;
	t_noeud_migrant supprime;

	//Positionner l'itérateur au noeud qui PRÉCÈDE celui à supprimer.
	if (!positionner_liste_migrants(liste, liste->position - 1)) return 0;
	
	precedent = liste->iterateur;
	supprime = liste->iterateur->next;

	//Le noeud précédent "saute" par dessus celui à supprimer
	precedent->next = supprime->next;

	//Remettre l'itérateur à la position sélectionnée
	avancer_liste_migrants(liste);
	
	free(supprime);
	liste->taille -= 1;

	return 1;
}

/*-----------------------------------------------------------------*/
/* supprimer le noeud sous l'itérateur
Pour supprimer ce noeud,  on considère
CAS #0 la liste est vide, rien à faire
CAS #1 si la liste a un seul noeud, on la vide
CAS #2 si l'itérateur est sur la tete
CAS #3 si l'itérateur est sur le queue
CAS #4 on supprime  un noeud  à l'intérieur (la static)

*/
int supprimer_liste_migrants(t_liste_migrants liste) {

	/* si la liste est vide */
	if (liste->taille == 0) return 0;

	/* si la liste n'a qu'un élément */
	if (liste->taille == 1) {
		vider_liste_migrants(liste);
		return 1;
	}

	/* si iter est au début */
	if (liste->iterateur == liste->tete) return supprimer_debut_liste_migrants(liste);

	/* si iter est à la fin */
	if (liste->iterateur == liste->queue) return supprimer_fin_liste_migrants(liste);

	/*sinon on supprime un noeud intérieur  */
	return supprimer_position_liste_migrants(liste);

}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*  liberer tous les noeuds et assigner les membres */
int vider_liste_migrants(t_liste_migrants liste) {
	int cpt = 0;
	t_noeud_migrant	temp;
	
	if (liste->taille == 0) return 0;

	temp = liste->tete;
	/* jusqu'au queue, on ne se trompe pas  */
	while (liste->tete) {
		cpt += 1;
		liste->tete = liste->tete->next;
		free(temp);
		temp = liste->tete;
	}

	/* les membres sont remis à ceux d'une liste vierge  */
	liste->tete = liste->queue = liste->iterateur = NULL;
	liste->taille = 0; liste->position = -1;
	return cpt;

}
/*-----------------------------------------------------------------*/

int dec_hrs_transit_liste_migrants(t_liste_migrants liste) {
	if (liste->taille == 0) return 0;
	dec_hrs_transit_migrant(liste->iterateur);
	return 1;
}

int assurer_temps_maladie_migrants(t_liste_migrants liste) {
	if (liste->taille == 0) return 0;
	
	if (get_etat(&liste->iterateur->valeur.voyageur) == MALADE) {
		inc_hrs_maladie(&liste->iterateur->valeur.voyageur);
		if (get_hrs_maladie(&liste->iterateur->valeur.voyageur) >= NB_HRS_MALADIE) {
			return 1;
		}
	}

	return 0;
}

int terminer_maladie_migrants(t_liste_migrants liste, double proportion_confinement) {
	int resultat = determiner_mort_ou_retabli(&liste->iterateur->valeur.voyageur);
	
	if (resultat == 1) { /* Mort */
		modifier_etat_personne(&liste->iterateur->valeur.voyageur, MORT, proportion_confinement);
		return 1;
	}
	else if (resultat == 2) { /* Rétabli */
		modifier_etat_personne(&liste->iterateur->valeur.voyageur, SAIN, proportion_confinement);
	}

	return 0;
}

#if 0
int main(void) {
	t_liste_migrants list = init_liste_migrants();
	
	t_personne personne_0 = init_personne(10, 10, PROP_INITIALE);
	t_personne personne_1 = init_personne(10, 10, PROP_INITIALE);
	t_personne personne_2 = init_personne(10, 10, PROP_INITIALE);
	t_personne personne_3 = init_personne(10, 10, PROP_INITIALE);
	t_personne personne_4 = init_personne(10, 10, PROP_INITIALE);
	t_personne personne_A = init_personne(10, 10, PROP_INITIALE);
	
	t_migrant migrant_0 = init_migrant(&personne_0, 0, 0, 0);
	t_migrant migrant_1 = init_migrant(&personne_1, 1, 1, 1);
	t_migrant migrant_2 = init_migrant(&personne_2, 2, 2, 2);
	t_migrant migrant_3 = init_migrant(&personne_3, 3, 3, 3);
	t_migrant migrant_4 = init_migrant(&personne_4, 4, 4, 4);
	
	t_migrant migrant_A = init_migrant(&personne_A, 9, 9, 9);
	t_migrant migrant_A_copie;
	t_migrant migrant_2_copie;
	
	assert(list);
	assert(est_vide_liste_migrants(list));

	//Insérer des migrants dans la liste
	assert(inserer_liste_migrants(list, &migrant_0));
	assert(inserer_liste_migrants(list, &migrant_1));
	assert(inserer_liste_migrants(list, &migrant_2));
	assert(inserer_liste_migrants(list, &migrant_3));
	assert(inserer_liste_migrants(list, &migrant_4));

	//Vérifier qu'ils sont bien présents
	assert(get_dans_liste_migrants(list) == 5);
	assert(!est_vide_liste_migrants(list));

	//Insérer un migrant en plein milieu de la liste
	positionner_liste_migrants(list, 2);
	assert(inserer_liste_migrants(list, &migrant_A));

	//Vérifier que la bonne valeur est à la bonne place.
	assert(get_position_liste_migrants(list) == 2);
	assert(get_valeur_liste_migrants(list, &migrant_A_copie));

	//Toutes ces valeurs devraient être 9
	assert(migrant_A_copie.ville_destination == migrant_A.ville_destination);
	assert(migrant_A_copie.ville_depart == migrant_A.ville_depart);
	assert(migrant_A_copie.hrs_transit == migrant_A.hrs_transit);

	//Tester le positionnement d'itérateur
	assert(aller_debut_liste_migrants(list));
	assert(get_position_liste_migrants(list) == 0);

	assert(aller_fin_liste_migrants(list));
	assert(get_position_liste_migrants(list) == 5);

	positionner_liste_migrants(list, 2);
	assert(supprimer_liste_migrants(list));
	assert(get_position_liste_migrants(list) == 2);

	//Vérifier que le migrant 2 original est revenu à sa place.
	assert(get_valeur_liste_migrants(list, &migrant_2_copie));
	assert(migrant_2_copie.ville_destination == migrant_2.ville_destination);
	assert(migrant_2_copie.ville_depart == migrant_2.ville_depart);
	assert(migrant_2_copie.hrs_transit == migrant_2.hrs_transit);

	//Vérifier la supression et la destruction de la liste
	assert(vider_liste_migrants(list) == 5);
	assert(est_vide_liste_migrants(list));

	detruire_liste_migrants(list);

	system("pause");
	return EXIT_SUCCESS;
}
#endif