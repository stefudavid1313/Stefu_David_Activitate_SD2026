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

struct Nod {
	Masina info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};
typedef struct ListaDubla ListaDubla;

struct NodArbore {
	Masina masina;
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
};
typedef struct NodArbore NodArbore;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[4] = ",\n\r";
	Masina m1;
	m1.id = -1;
	if (fgets(buffer, 100, file)) {
		char* aux;
		aux = strtok(buffer, sep);
		if (aux == NULL) return m1;
		m1.id = atoi(aux);
		m1.nrUsi = atoi(strtok(NULL, sep));
		m1.pret = (float)atof(strtok(NULL, sep));
		aux = strtok(NULL, sep);
		m1.model = (char*)malloc(strlen(aux) + 1);
		strcpy(m1.model, aux);
		aux = strtok(NULL, sep);
		m1.numeSofer = (char*)malloc(strlen(aux) + 1);
		strcpy(m1.numeSofer, aux);
		m1.serie = strtok(NULL, sep)[0];
	}
	return m1;
}

void afisareMasina(Masina masina) {
	if (masina.id == -1) return;
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
	Nod* nou = (Nod*)malloc(sizeof(Nod));
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
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = ld->prim;
	nou->prev = NULL;
	if (ld->prim) {
		ld->prim->prev = nou;
	}
	else {
		ld->ultim = nou;
	}
	ld->prim = nou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla ld;
	ld.prim = ld.ultim = NULL;
	if (f) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			if (m.id != -1) {
				adaugaMasinaInLista(&ld, m);
			}
		}
		fclose(f);
	}
	return ld;
}

void dezalocareLDMasini(ListaDubla* ld) {
	while (ld->prim) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		Nod* temp = ld->prim;
		ld->prim = ld->prim->next;
		free(temp);
	}
	ld->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
	if (lista.prim == NULL) return 0;
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
	if (ld == NULL || ld->prim == NULL) return;
	Nod* p = ld->prim;
	while (p != NULL && p->info.id != id) {
		p = p->next;
	}
	if (p == NULL) return;
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
	if (lista.prim == NULL) return NULL;
	Nod* maxNod = lista.prim;
	Nod* p = lista.prim->next;
	while (p != NULL) {
		if (p->info.pret > maxNod->info.pret) {
			maxNod = p;
		}
		p = p->next;
	}
	char* numeSofer = (char*)malloc(strlen(maxNod->info.numeSofer) + 1);
	strcpy(numeSofer, maxNod->info.numeSofer);
	return numeSofer;
}

void adaugaMasinaInArbore(NodArbore** radacina, Masina masinaNoua) {
	NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
	nou->dreapta = NULL;
	nou->stanga = NULL;
	nou->masina = masinaNoua;
	if (*radacina == NULL) {
		*radacina = nou;
		return;
	}
	if (masinaNoua.id < (*radacina)->masina.id) {
		adaugaMasinaInArbore(&(*radacina)->stanga, masinaNoua);
	}
	else {
		adaugaMasinaInArbore(&(*radacina)->dreapta, masinaNoua);
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodArbore* arbore = NULL;
	if (f) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			if (m.id != -1) adaugaMasinaInArbore(&arbore, m);
		}
		fclose(f);
	}
	return arbore;
}

void afisareMasiniDinArbore(NodArbore* radacina) {
	if (radacina == NULL) return;
	afisareMasiniDinArbore(radacina->stanga);
	afisareMasina(radacina->masina);
	afisareMasiniDinArbore(radacina->dreapta);
}

void afisareMasiniDinArbore2(NodArbore* radacina) {
	if (radacina == NULL) return;
	afisareMasiniDinArbore2(radacina->stanga);
	afisareMasiniDinArbore2(radacina->dreapta);
	afisareMasina(radacina->masina);
}

void dezalocareArboreDeMasini(NodArbore** radacina) {
	if (*radacina == NULL) return;
	dezalocareArboreDeMasini(&(*radacina)->stanga);
	dezalocareArboreDeMasini(&(*radacina)->dreapta);
	free((*radacina)->masina.model);
	free((*radacina)->masina.numeSofer);
	free(*radacina);
	*radacina = NULL;
}

Masina getMasinaByID(NodArbore* arbore, int id) {
	Masina m;
	if (arbore) {
		if (arbore->masina.id == id) {
			m = arbore->masina;
			m.model = (char*)malloc(strlen(arbore->masina.model) + 1);
			strcpy(m.model, arbore->masina.model);
			m.numeSofer = (char*)malloc(strlen(arbore->masina.numeSofer) + 1);
			strcpy(m.numeSofer, arbore->masina.numeSofer);
		}
		else {
			if (arbore->masina.id < id) m = getMasinaByID(arbore->stanga, id);
			else m = getMasinaByID(arbore->dreapta, id);
		}
	}
	else {
		m.id = -1;
	}
	return m;
}

int determinaNumarNoduri(NodArbore* arbore) {
	if (!arbore) return 0;
	return 1 + determinaNumarNoduri(arbore->stanga) + determinaNumarNoduri(arbore->dreapta);
}

int maxim(int a, int b) {
	if (a > b) return a;
	else return b;
}

int calculeazaInaltimeArbore(NodArbore* arbore) {
	if (!arbore) return -1;
	return 1 + maxim(calculeazaInaltimeArbore(arbore->stanga), calculeazaInaltimeArbore(arbore->dreapta));
}

float calculeazaPretTotal(NodArbore* arbore) {
	if (!arbore) return 0;
	return arbore->masina.pret + calculeazaPretTotal(arbore->stanga) + calculeazaPretTotal(arbore->dreapta);
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore* radacina, const char* numeSofer) {
	if (!radacina) return 0;
	float pretCurent = 0;
	if (strcmp(radacina->masina.numeSofer, numeSofer) == 0) {
		pretCurent = radacina->masina.pret;
	}
	return pretCurent + calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	printf("Pretul mediu al masinilor este: %.2f\n", calculeazaPretMediu(lista));
	stergeMasinaDupaID(&lista, 2);
	char* numeSoferMasinaScumpa = getNumeSoferMasinaScumpa(lista);
	if (numeSoferMasinaScumpa) {
		printf("Numele soferului cu masina scumpa: %s\n", numeSoferMasinaScumpa);
		free(numeSoferMasinaScumpa);
	}
	dezalocareLDMasini(&lista);

	NodArbore* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareMasiniDinArbore(arbore);
	Masina cautata = getMasinaByID(arbore, 5);
	afisareMasina(cautata);
	if (cautata.id != -1) {
		free(cautata.model);
		free(cautata.numeSofer);
	}
	int nrNoduri = determinaNumarNoduri(arbore);
	printf("%d\n", nrNoduri);
	int inaltime = calculeazaInaltimeArbore(arbore);
	printf("%d\n", inaltime);
	float pretTotal = calculeazaPretTotal(arbore);
	printf("%.2f\n", pretTotal);
	dezalocareArboreDeMasini(&arbore);

	return 0;
}