// persona.c
#include "persona.h"

void llegirFitxer(const char *nom_fitxer, Persona **persones, int *n) {
    FILE *f = fopen(nom_fitxer, "r");
    if (!f) {
        fprintf(stderr, "No s'ha pogut obrir el fitxer.\n");
        exit(1);
    }

    fscanf(f, "%d", n);
    char buffer[256];
    fgets(buffer, 256, f);

    *persones = (Persona *)malloc(*n * sizeof(Persona));
    if (!*persones) {
        fclose(f);
        fprintf(stderr, "Error en la reserva de memòria.\n");
        exit(1);
    }

    for (int i = 0; i < *n; i++) {
        fscanf(f, "%s\t%s\t%s\t%d\n", (*persones)[i].nom,
                                      (*persones)[i].cognom,
                                      (*persones)[i].dni,
                                      &(*persones)[i].edat);
    }

    fclose(f);
}

void mostrarPersona(Persona p) {
    printf("Nom: %s, Cognom: %s, DNI: %s, Edat: %d\n",
           p.nom, p.cognom, p.dni, p.edat);
}

void mostrarTotes(Persona *persones, int n) {
    for (int i = 0; i < n; i++) {
        printf("[%d] ", i);
        mostrarPersona(persones[i]);
    }
}

void ordenacioPerInsercio(Persona *persones, int n) {
    for (int i = 1; i < n; i++) {
        Persona actual = persones[i];
        int j = i - 1;

        while (j >= 0 && strcmp(persones[j].cognom, actual.cognom) > 0) {
            persones[j + 1] = persones[j];
            j--;
        }
        persones[j + 1] = actual;
    }
}

int cercaBinaria(Persona *persones, int n, const char *cognom) {
    int esq = 0, dre = n - 1;
    while (esq <= dre) {
        int mig = (esq + dre) / 2;
        int comp = strcmp(persones[mig].cognom, cognom);

        if (comp == 0)
            return mig;
        else if (comp < 0)
            esq = mig + 1;
        else
            dre = mig - 1;
    }
    return -1;
}

void cercarIImprimir(Persona *persones, int n, const char *cognom) {
    int pos = cercaBinaria(persones, n, cognom);

    if (pos == -1) {
        printf("No s'ha trobat cap persona amb el cognom '%s'.\n", cognom);
        return;
    }

    printf("Resultat trobat:\n");
    mostrarPersona(persones[pos]);
}

void guardarFitxer(const char *nom_fitxer, Persona *persones, int n) {
    FILE *f = fopen(nom_fitxer, "w");
    if (!f) {
        fprintf(stderr, "No s'ha pogut obrir el fitxer per escriure.\n");
        return;
    }

    fprintf(f, "%d\n", n);
    fprintf(f, "Nom\tCognom\tDNI\tEdat\n");

    for (int i = 0; i < n; i++) {
        fprintf(f, "%s\t%s\t%s\t%d\n",
                persones[i].nom,
                persones[i].cognom,
                persones[i].dni,
                persones[i].edat);
    }

    fclose(f);
    printf("Dades desades correctament a '%s'.\n", nom_fitxer);
}
