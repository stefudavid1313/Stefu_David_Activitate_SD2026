#include<stdio.h>
#include<malloc.h>
#define _CRT_SECURE_NO_WARNINGS


struct Telefon {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};
struct Telefon initializare(int id, int RAM, char* producator, float pret, char serie) {
	struct Telefon t;
	t.id = id;
	t.RAM = RAM;
	t.producator = (char*)malloc((strlen(producator) + 1) * sizeof(char));
	strcpy(t.producator, producator);
	t.pret = pret;
	t.serie = serie;
	return t;
}

struct Telefon copiazaTelefon(struct Telefon t) {
	struct Telefon telefon = initializare(t.id, t.RAM, t.producator, t.pret, t.serie);
	return telefon;
}

void afisare(struct Telefon t) {
	printf("Id: %d, RAM: %d, Producator: %s, Pret: %5.2f, Serie: %c\n", t.id, t.RAM, t.producator, t.pret, t.serie);
}

void afisareVector(struct Telefon* vector, int nrElemente) {
	//afisarea elementelor din vector apeland functia afisare
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}



struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	if (nrElementeCopiate > nrElemente) {
		nrElementeCopiate = nrElemente;
	}
	struct Telefon* vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = copiazaTelefon(vector[i]);
	}
	return vectorNou;
}

void dezalocare(struct Telefon** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].producator);
	}
	free(*vector);
	*nrElemente = 0;
}

void copiazaTelefoaneScumpe(struct Telefon* vector, char nrElemente, float pret_min, struct Telefon** vectorNou, int* dimensiune) {

	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pret_min) {
			(*dimensiune)++;
		}
	}

	*vectorNou = malloc(sizeof(struct Telefon) * (*dimensiune));

	for (int i = 0, j = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pret_min) {
			(*vectorNou)[j] = copiazaTelefon(vector[i]);
			j++;
		}
	}

}

struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* prod) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.

	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(prod, vector[i].producator) == 0) return vector[i];
	}


}



int main() {

	struct Telefon t;
	t = initializare(0, 8, "Apple", 5900.00, 'I');
	afisare(t);
	printf("\n");
	int nrTelefoane = 3;
	struct Telefon* telefoane = (struct Telefon*)malloc(sizeof(struct Telefon) * nrTelefoane);
	telefoane[0] = initializare(1, 8, "Samsung", 2500.00, 'A');
	telefoane[1] = initializare(2, 16, "Apple", 5000.00, 'B');
	telefoane[2] = t;

	afisareVector(telefoane, nrTelefoane);
	printf("\n");

	int nrTelefoaneCopiate = 2;
	struct Telefon* telefoaneCopiate = copiazaPrimeleNElemente(telefoane, nrTelefoane, nrTelefoaneCopiate);
	printf("\n");
	afisareVector(telefoaneCopiate, nrTelefoaneCopiate);
	printf("\n");

	dezalocare(&telefoaneCopiate, &nrTelefoaneCopiate);
	printf("\n");
	afisareVector(&telefoaneCopiate, nrTelefoaneCopiate);
	printf("\n");


	struct Telefon* vectorNou;
	int dimensiune;
	copiazaTelefoaneScumpe(telefoane, nrTelefoane, 2700, &vectorNou, &dimensiune);
	printf("\n");
	afisareVector(vectorNou, dimensiune);
	printf("\n");

	struct Telefon telefonGasit = getPrimulElementConditionat(telefoane, nrTelefoane, "Xiaomi");
	printf("\n");
	afisare(telefonGasit);

	return 0;
}