#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
	Masina m;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodS* next;
	NodP* info;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[128];
	if (fgets(buffer, 128, file)) {
		Masina m;
		char* token = strtok(buffer, ",\n");
		m.id = atoi(token);

		m.nrUsi = atoi(strtok(NULL, ",\n"));
		m.pret = (float)atof(strtok(NULL, ",\n"));

		char* tempModel = strtok(NULL, ",\n");
		m.model = (char*)malloc(strlen(tempModel) + 1);
		strcpy(m.model, tempModel);

		char* tempSofer = strtok(NULL, ",\n");
		m.numeSofer = (char*)malloc(strlen(tempSofer) + 1);
		strcpy(m.numeSofer, tempSofer);

		m.serie = (unsigned char)(*strtok(NULL, ",\n"));
		return m;
	}
	Masina vida = { 0, 0, 0.0f, NULL, NULL, 0 };
	return vida;
}

void afisareMasina(Masina masina) {
	printf("ID: %d | Usi: %d | Pret: %.1f | Model: %s | Sofer: %s | Serie: %c\n",
		masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);
}

void inserareListaP(NodP** cap, Masina m) {
	NodP* nou = (NodP*)malloc(sizeof(NodP));
	nou->m = m;
	nou->next = NULL;
	nou->vecini = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		NodP* parcurgere = *cap;
		while (parcurgere->next) {
			parcurgere = parcurgere->next;
		}
		parcurgere->next = nou;
	}
}

void inserareListaS(NodS** cap, NodP* nodTinta) {
	NodS* nouS = (NodS*)malloc(sizeof(NodS));
	nouS->info = nodTinta;
	nouS->next = NULL;

	if (*cap == NULL) {
		*cap = nouS;
	}
	else {
		NodS* p = *cap;
		while (p->next) p = p->next;
		p->next = nouS;
	}
}

NodP* cautaNod(NodP* graf, int id) {
	while (graf && graf->m.id != id) {
		graf = graf->next;
	}
	return graf;
}

void adaugaMuchie(NodP* graf, int id1, int id2) {
	NodP* n1 = cautaNod(graf, id1);
	NodP* n2 = cautaNod(graf, id2);
	if (n1 && n2) {
		inserareListaS(&(n1->vecini), n2);
		inserareListaS(&(n2->vecini), n1);
	}
}

NodP* incarcaGraf(const char* fisier) {
	FILE* f = fopen(fisier, "r");
	if (!f) return NULL;
	NodP* cap = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.model != NULL) {
			inserareListaP(&cap, m);
		}
	}
	fclose(f);
	return cap;
}

void incarcaMuchii(const char* fisier, NodP* graf) {
	FILE* f = fopen(fisier, "r");
	if (!f) return;
	int a, b;
	while (fscanf(f, "%d %d", &a, &b) == 2) {
		adaugaMuchie(graf, a, b);
	}
	fclose(f);
}

void curataGraf(NodP** graf) {
	NodP* p = *graf;
	while (p) {
		NodS* s = p->vecini;
		while (s) {
			NodS* deStersS = s;
			s = s->next;
			free(deStersS);
		}
		free(p->m.model);
		free(p->m.numeSofer);
		NodP* deStersP = p;
		p = p->next;
		free(deStersP);
	}
	*graf = NULL;
}

int main() {
	NodP* reteaMasini = incarcaGraf("masini.txt");
	incarcaMuchii("muchii.txt", reteaMasini);

	printf("Vecinii masinii cu ID 3:\n");
	NodP* cautat = cautaNod(reteaMasini, 3);
	if (cautat) {
		NodS* v = cautat->vecini;
		while (v) {
			afisareMasina(v->info->m);
			v = v->next;
		}
	}

	curataGraf(&reteaMasini);
	return 0;
}