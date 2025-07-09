/*=========================================================*/
// Ete 2025 pour nos etudiants en C
/*=========================================================*/
#include "m_alea_pop.h"
#include "Constante.h"

/*=========================================================*/

/*=========================================================*/
/* proportions observ�es dans la population canadienne 2018 (STATS CAN)
   selon l'�ge [0-9, 10-19, 20-29, 30-39, ..., 80-89, >=90]
   vous avez ici les proportions et la cumulative
*/
const double PROP_POPULATION[10] = { 0.106, 0.106, 0.137, 0.14, 0.129, 0.14, 0.123, 0.077, 0.034, 0.008 };
const double PROP_CUMULATIVES[10] = { 0.106, 0.212, 0.349, 0.489, 0.618, 0.758, 0.881, 0.958, 0.992, 1.0 };
/*=========================================================*/

/*=========================================================*/
// D�finitions des fonctions :
/*=========================================================*/
// des six fonctions d'interface du g�n�rateur
/*=========================================================*/
double randf(void) {
	// calcul du rapport de la valeur issue de rand au maximum
	// emissible + epsilon r�el 
	return rand() / (1.0 * RAND_MAX + EPSILON);
}
/*=========================================================*/
int randi(int n) {
	// si n<= 0 retour 0 sinon
	// remarquez le typecast (int)
	// (int)(n * randf()) donne un entier dans [0,n-1]
	// et avec +1, on l'am�ne dans [1,n]
	return (n <= 0) ? 0 : (int)(n * randf()) + 1;
}
/*=========================================================*/
double randf_bornes(double b1, double b2) {
	// de [0,1] vers l'intervalle d�sign�
	// maths : c'est une dilatation ((b2 - b1) * randf())
	// suivie d'une translation
	double min_b = fmin(b1, b2);
	double max_b = fmax(b1, b2);
	return min_b + ((max_b - min_b) * randf());
}
/*=========================================================*/

/*==========================================================*/

/*	attention ici
	ce code d�passe vos connaissances actuelles
	l'algo obtenu de la m�thode de Marsaglia
	tr�s bien d�crite sur wiki
*/
double randz(void) {
	static int test_regener = 1;
	static double v1;
	static double v2;
	static double w;
	static double facteur;
	double sortie;

	if (test_regener) {
		do {
			v1 = randf_bornes(-1, 1);
			v2 = randf_bornes(-1, 1);
			w = v1 * v1 + v2 * v2;
		} while (w >= 1 || w < EPSILON); // Ajout de w < EPSILON pour �viter log(0)
		facteur = sqrt(-2 * log(w) / w);
		sortie = v1 * facteur;
	}
	else {
		sortie = v2 * facteur;
	}
	test_regener = (test_regener + 1) % 2;
	return sortie;
}/*==========================================================*/

double rand_normal(double mu, double sigma) {
	// transformation lin�aire 
	// pareille � celle de randf_bornes
	return mu + randz() * fabs(sigma); 
}

/*==========================================================*/
int rand_age_canada(void) {
	// pour identifier le groupe d'�ge d'un canadien
	double prob_groupe = randf();
	int i = 0;
	// trouver � quel groupe d'�ge il appartient
	while (i < 10 && prob_groupe > PROP_CUMULATIVES[i]) {
		++i;
	}
	// calcul de l'�ge : groupe i couvre [i*10, i*10+9]
	return (i * 10) + randi(10) - 1; // G�n�re un �ge dans [i*10, i*10+9]
}

/*==========================================================*/
void srand_sys(void) {
	// init du g�n�rateur avec l'horloge syst�me
	srand((unsigned int)time(NULL));
}

/*=========================================================*/
// fin de l'impl�mentation
/*=========================================================*/

/*=========================================================*/
/*=========================================================*/
/* � titre p�dagogique
   je vous ai ajout� un test des �ges �mis pour le Canada
   on peut v�rifier avec la proportion originale dans les
   constantes que j'affiche � la fin
*/
/*=========================================================*/
#if 0
#define NB_EX 1000000
int main(void) {
	int i;

	// tableau pour compter dans leur groupe d'�ge
	int groupe_pop[12] = { 0 };

	srand_sys();

	// boucle d'�mission des �ges
	for (i = 0; i < NB_EX; ++i) {
		int age = rand_age_canada();
		// compter dans son groupe
		if (age >= 0 && age / 10 < 12) { // V�rification pour �viter d�bordements
			groupe_pop[age / 10] += 1;
		}
	}

	// affichage des compteurs, proportions et rapport th�orie/observation
	printf("\n\n\n");
	for (i = 0; i < 10; ++i) {
		double prop = (1.0 * groupe_pop[i]) / NB_EX;
		printf("%15d%20.3f%20.3f\n", groupe_pop[i], prop, PROP_POPULATION[i] / (prop + EPSILON));
	}

	printf("\n\n\n");

	// Note : system("pause") est sp�cifique � Windows, utiliser une alternative pour portabilit�
	system("pause");
	return EXIT_SUCCESS;
}
#endif
/*=========================================================*/