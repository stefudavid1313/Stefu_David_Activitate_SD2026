
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

struct Nod {
	Masina info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
	//int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

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

void afisareListaMasini(ListaDubla lista) {
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void afisareInversaListaMasini(ListaDubla lista) {
	Nod* p = lista.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* ld, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = ld->ultim;
	if (ld->ultim) {
		ld->ultim->next = nou;
	}
	else {
		ld->prim = nou;
	}
	ld->ultim = nou;



}

void adaugaLaInceputInLista(ListaDubla* ld, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = ld->prim;
	nou->prev = NULL;
	if (ld->prim) {
		ld->ultim->prev = nou;
	}
	else {
		ld->ultim = nou;
	}
	ld->prim = nou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {

	FILE* f = fopen(numeFisier, "r");
	if (f) {
		ListaDubla ld;
		ld.prim = ld.ultim = NULL;
		while (!feof(f)) {
			adaugaMasinaInLista(&ld, citireMasinaDinFisier(f));
		}
		fclose(f);
		return ld;
	}

}

void dezalocareLDMasini(ListaDubla* ld) {
	while (ld->prim && ld->prim->next) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		ld->prim = ld->prim->next;
		free(ld->prim->prev);
	}
	if (ld->prim) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		free(ld->prim);


	}
	ld->prim = ld->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
	if (lista.prim == NULL) {
		return 0;
	}

	float suma = 0;
	int contor = 0;
	Nod* p = lista.prim;

	while (p != NULL) {
		suma += p->info.pret;
		contor++;
		p = p->next;
	}

	return suma / contor;
}

void stergeMasinaDupaID(ListaDubla* ld, int id) {
	if (ld == NULL || ld->prim == NULL) {
		return;
	}
	Nod* p = ld->prim;
	while (p != NULL && p->info.id != id) {
		p = p->next;
	}
	if (p == NULL) {
		return;
	}

	if (p == ld->prim && p == ld->ultim) {
		ld->prim = NULL;
		ld->ultim = NULL;
	}

	else if (p == ld->prim) {
		ld->prim = p->next;
		ld->prim->prev = NULL;
	}

	else if (p == ld->ultim) {
		ld->ultim = p->prev;
		ld->ultim->next = NULL;
	}

	else {
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}

	free(p->info.model);
	free(p->info.numeSofer);
	free(p);
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	if (lista.prim == NULL) {
		return NULL;
	}

	Nod* maxNod = lista.prim;
	Nod* p = lista.prim->next;

	while (p != NULL) {
		if (p->info.pret > maxNod->info.pret) {
			maxNod = p;
		}
		p = p->next;
	}

	char* numeSofer = malloc(strlen(maxNod->info.numeSofer) + 1);
	strcpy_s(numeSofer, strlen(maxNod->info.numeSofer) + 1, maxNod->info.numeSofer);

	return numeSofer;
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	printf("Pretul mediu al masinilor este: %.2f\n", calculeazaPretMediu(lista));
	stergeMasinaDupaID(&lista, 2);

	char* numeSoferMasinaScumpa = getNumeSoferMasinaScumpa(lista);
	printf("Numele soferului cu maisna scumpa: %s", numeSoferMasinaScumpa);


	return 0;
}
