/******************************************************************************/
/* M_ENSEMBLES_NOMS.C  en devenir    (*** FILL, À COMPLÉTER ***)                              */
/* module de gestion des noms de villes dans le TP2                                      */
/******************************************************************************/
  
#include "m_ensemble_noms.h"

/******************************************************************************/
/* La structure privée qui maintient l'ensemble des noms dans les lignes
 de sa matrice */

typedef struct {
	char** tab_noms;       // matrice de caracteres contenant les noms de ville
	unsigned int max_noms;    // nombre de lignes de la matrice
	unsigned int nb_noms;     // nombre actuel de noms de villes 
} t_ensemble_noms;

/******************************************************************************/
/* La variable PRIVÉE du module, à représentation cachée mais dont  
   l'usage est disponible pour tous les modules  */

static t_ensemble_noms  groupe_noms = { 0 };

/******************************************************************************/
/*                    DÉFINITIONS DES FONCTIONS                               */
/******************************************************************************/
/* 
    créer la matrice de caractères de 
    la variable static groupe_noms
   le nombre de lignes est en paramètre
   chaque ligne a TAILLE_MAX_NOM colonnes de char
   faire des assert sur toute allocation
   
   RETOUR  0 ou 1

   SPECS si l'ensemble existe déjà
   retour automatique de 0
*/

int init_ensemble_noms_villes(unsigned int taille) {
	unsigned int i;
	
	// si l'ensemble existe déjà 
	if (groupe_noms.max_noms) return 0;

	//allocation du tableau dynamique de char *
	assert(groupe_noms.tab_noms = (char **)malloc(taille * sizeof(char *)));
	// boucle d'allocation de chaque tableau dynamique de char
	for (i = 0; i < taille; ++i) {
		assert(groupe_noms.tab_noms[i] = (char *)malloc(TAILLE_MAX_NOM * sizeof(char)));
	}

	groupe_noms.max_noms = taille;
	groupe_noms.nb_noms = 0;
	return 1;
}


/******************************************************************************/
// get des valeurs d'un des deux membres entiers de groupe_noms

unsigned int get_nombre_villes(void) {
	return groupe_noms.nb_noms; 
}

unsigned int get_nombre_max_villes(void) {
	return groupe_noms.max_noms;
}

/******************************************************************************/
/* Retour du pointeur du nom de la ville à cette position 
   (position à partir de 0) dans groupe_noms.tab_noms

   retour de NULL si position invalide
*/
const char* get_nom_ville(unsigned int position) {

	if (position < groupe_noms.max_noms && position >= 0) {
		return groupe_noms.tab_noms[position];
	}
	return NULL;
}

/******************************************************************************/
/* Retourner le numéro de ligne de la matrice contenant le nom
	ou retour de NOM_ABSENT
	boucler sur les villes presentes dans tab_noms
	 si strcmp(nom, groupe_noms.tab_noms[i])==0 retour de i
   
*/
unsigned int get_position_ville(const char* nom) {
	unsigned int i;

	for (i = 0; i < groupe_noms.nb_noms; ++i) {
		if (strcmp(nom, groupe_noms.tab_noms[i]) == 0) {
			return i;
		}
	}
	return NOM_ABSENT;
}

/******************************************************************************/
/* comparairon entre 2 noms
   Retourne 1 si les deux stings sont égales , 0 sinon (inverse d'un "strcmp").
*/
int comparer_noms_villes(const char* villeA, const char* villeB) {
	return (strcmp(villeA, villeB) == 0) ? 1 : 0;
}

/******************************************************************************/
/* 
   ajout si et seulement si
	 il reste de la place (nb_noms<max_noms)
	  si strlen(nom) < TAILLE_MAX_NOM
	  le nom n'y est PAS déjà( get_position_ville retourne NOM_ABSENT)
	 alors copier le nouveau nom sur une ligne de tab_noms
	 ajuster nb_noms
   Retour de 1 si le nom a été ajouté, 0 sinon
*/
int ajouter_nom_ville(const char* nom) {
	if (groupe_noms.nb_noms < groupe_noms.max_noms && strlen(nom) < TAILLE_MAX_NOM && get_position_ville(nom) == NOM_ABSENT)
	{
		strcpy(groupe_noms.tab_noms[groupe_noms.nb_noms], nom);
		++groupe_noms.nb_noms;
		return 1;
	}

	return 0;
}
/******************************************************************************/
/*  considérer que  l'ensemble de noms  est redevenu vide */
void vider_ensemble_noms_villes(void) {
	unsigned int i;
	/* groupe_noms.nb_noms est mis à 0 */
	/* chaque ligne  de la matrice  va commencer avec le caractere 0*/

	groupe_noms.nb_noms = 0;
	for (i = 0; i < groupe_noms.max_noms; ++i) {
		groupe_noms.tab_noms[i][0] = '\0';
	}
}

/******************************************************************************/
/* libérer toute la mémoire dyna utilisée par groupe_noms
    1- liberer chaque ligne de la matrice tab_noms
	2- liberer tab_noms
	mettre deux membres de groupe_noms à  0 et le troisième à NULL 
*/
void detruire_ensemble_noms_villes(void) {
	int i;

	for (i = 0; i < groupe_noms.max_noms; ++i) {
		free(groupe_noms.tab_noms[i]);
	}
	free(groupe_noms.tab_noms);

	groupe_noms.max_noms = 0;
	groupe_noms.nb_noms = 0;
	groupe_noms.tab_noms = NULL;
}

/******************************************************************************/
/******************************************************************************/

#if 0

/* ce petit test est fait pour être tracé 
   pour en saisir la portée
*/

int main(void) {
	int nb; int nb_max;
	int suc; int rep;

	const char* ville;

	assert(init_ensemble_noms_villes(145));

	nb = get_nombre_villes();
	nb_max = get_nombre_max_villes();

	rep = ajouter_nom_ville("toronto");
	rep = ajouter_nom_ville("london");
	rep = ajouter_nom_ville("levis");

	nb = get_nombre_villes();
	nb_max = get_nombre_max_villes();

	assert(init_ensemble_noms_villes(10)==0);

	nb = get_nombre_villes();
	nb_max = get_nombre_max_villes();

	suc = get_position_ville("london");
	suc = get_position_ville("toronto");
	suc = get_position_ville("levis");

	ville = get_nom_ville(0);
	printf("\n ville %s", ville);
	printf("\n ville %s", get_nom_ville(1));

	
	vider_ensemble_noms_villes();
	rep = get_position_ville("montreal");
	rep = get_position_ville("toronto");

	printf("\n ville %s", get_nom_ville(0));

	nb = get_nombre_villes();
	nb_max = get_nombre_max_villes();
	

	detruire_ensemble_noms_villes();
	nb = get_nombre_villes();
	nb_max = get_nombre_max_villes();

	rep = init_ensemble_noms_villes(24);
	nb = get_nombre_villes();
	nb_max = get_nombre_max_villes();

	system("pause");
	return EXIT_SUCCESS;
}
#endif

/******************************************************************************/
/******************************************************************************/
