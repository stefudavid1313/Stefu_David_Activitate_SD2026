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



void afisareMasina(Masina masina) {
    printf("ID: %d | Nr usi: %d | Pret: %.2f | Model: %s | Sofer: %s | Serie: %c\n",
        masina.id, masina.nrUsi, masina.pret,
        masina.model, masina.numeSofer, masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
    if (masini == NULL || nrMasini == 0) {
        printf("Vectorul este gol!\n");
        return;
    }

    for (int i = 0; i < nrMasini; i++) {
        afisareMasina(masini[i]);
    }
}


void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {

    Masina* temp = (Masina*)malloc(((*nrMasini) + 1) * sizeof(Masina));

    for (int i = 0; i < *nrMasini; i++) {
        temp[i] = (*masini)[i];
    }

    temp[*nrMasini] = masinaNoua;

    free(*masini);
    *masini = temp;
    (*nrMasini)++;
}


// ---------------- CITIRE DIN FISIER ----------------

Masina citireMasinaFisier(FILE* file) {

    char linie[256];

    Masina masina;
    masina.model = NULL;
    masina.numeSofer = NULL;

    if (!fgets(linie, sizeof(linie), file)) {
        masina.id = -1;
        return masina;
    }

    char delimitator[] = ",\n";

    char* token = strtok(linie, delimitator);
    if (!token) { masina.id = -1; return masina; }
    masina.id = atoi(token);

    token = strtok(NULL, delimitator);
    masina.nrUsi = atoi(token);

    token = strtok(NULL, delimitator);
    masina.pret = atof(token);

    token = strtok(NULL, delimitator);
    masina.model = (char*)malloc(strlen(token) + 1);
    strcpy(masina.model, token);

    token = strtok(NULL, delimitator);
    masina.numeSofer = (char*)malloc(strlen(token) + 1);
    strcpy(masina.numeSofer, token);

    token = strtok(NULL, delimitator);
    masina.serie = token[0];

    return masina;
}


Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {

    FILE* file = fopen(numeFisier, "r");

    if (!file) {
        printf("Eroare la deschiderea fisierului!\n");
        return NULL;
    }

    Masina* vectorMasini = NULL;
    *nrMasiniCitite = 0;

    while (1) {

        Masina masina = citireMasinaFisier(file);

        if (masina.id == -1)
            break;

        adaugaMasinaInVector(&vectorMasini, nrMasiniCitite, masina);
    }

    fclose(file);
    return vectorMasini;
}



void dezalocareVectorMasini(Masina** vector, int* nrMasini) {

    for (int i = 0; i < *nrMasini; i++) {
        free((*vector)[i].model);
        free((*vector)[i].numeSofer);
    }

    free(*vector);
    *vector = NULL;
    *nrMasini = 0;
}


// ---------------- MAIN ----------------

int main() {

    int nrMasini = 0;

    Masina* masini = citireVectorMasiniFisier("TextFile1.txt", &nrMasini);

    afisareVectorMasini(masini, nrMasini);

    dezalocareVectorMasini(&masini, &nrMasini);

    return 0;
}