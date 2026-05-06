#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct NodArbore NodArbore;
struct NodArbore {
	Masina info;
	NodArbore* right;
	NodArbore* left;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int calculeazaInaltimeArbore(NodArbore* root) {
	if (root == NULL)
		return 0;

	int dr = calculeazaInaltimeArbore(root->right);
	int st = calculeazaInaltimeArbore(root->left);

	return 1 + max(dr, st);
}

void rotireStanga(NodArbore** root) {
	NodArbore* aux = (*root)->right;
	(*root)->right = aux->left;
	aux->left = (*root);
	(*root) = aux;
}

void rotireDreapta(NodArbore** root) {
	NodArbore* aux = (*root)->left;
	(*root)->left = aux->right;
	aux->right = (*root);
	(*root) = aux;
}

int verificareEchilibru(NodArbore* root) {
	return calculeazaInaltimeArbore(root->left) - calculeazaInaltimeArbore(root->right);
}

void adaugaMasinaInArboreEchilibrat(NodArbore** root, Masina masinaNoua) {
	if ((*root) != NULL) {
		if (masinaNoua.id > (*root)->info.id) {
			adaugaMasinaInArboreEchilibrat(&(*root)->right, masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&(*root)->left, masinaNoua);
		}
		//incepe algoritmul de verificare a echilibrului
		int factorEchilibru = verificareEchilibru(*root);
		if (factorEchilibru == -2) {
			if (verificareEchilibru((*root)->right) == -1) {
				rotireStanga(&(*root));
			}
			else {
				rotireDreapta((*root)->right);
				rotireStanga(&(*root));
			}
		}

		if (factorEchilibru == 2) {
			if (verificareEchilibru((*root)->left) == -1) {
				rotireStanga(&(*root)->left);
			}
			rotireDreapta(&(*root));
		}
	}
	else {
		NodArbore* nou = malloc(sizeof(NodArbore));
		nou->left = NULL;
		nou->right = NULL;
		nou->info = masinaNoua;
		(*root) = nou;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	NodArbore* root = NULL;
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		while (!feof(file)) {
			adaugaMasinaInArboreEchilibrat(&root, citireMasinaDinFisier(file));
		}
	}
	fclose(file);
	return root;
}

void afisareMasiniDinArbore(NodArbore* root) {
	if (root) {
		afisareMasina(root->info);
		afisareMasiniDinArbore(root->left);
		afisareMasiniDinArbore(root->right);
	}
}

void dezalocareArboreDeMasini(NodArbore** root) {
	if (*root) {
		dezalocareArboreDeMasini(&(*root)->left);
		dezalocareArboreDeMasini(&(*root)->right);

		free((*root)->info.model);
		free((*root)->info.numeSofer);

		free(*root);
		*root = NULL;
	}
}
//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(/*arborele de masini*/int id);

int determinaNumarNoduri(/*arborele de masini*/);

float calculeazaPretTotal(/*arbore de masini*/);

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer);

int main() {
	NodArbore* root = NULL;
	root = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(root);

	return 0;
}