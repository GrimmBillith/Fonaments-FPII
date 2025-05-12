// main.c
#include "persona.h"
#include <stdio.h>

int main() {
    Persona *persones = NULL;
    int n = 0;
    char opcio;
    char cognom[50];

    printf("Introdueix el nom del fitxer: ");
    char fitxer[100];
    scanf("%s", fitxer);

    llegirFitxer(fitxer, &persones, &n);
    printf("Dades llegides correctament (%d persones).\n", n);

    printf("Ordenant taula per cognom...\n");
    ordenacioPerInsercio(persones, n);

    do {
        printf("\n--- Menú ---\n");
        printf("C - Cercar per cognom\n");
        printf("A - Afegir una nova persona\n");
        printf("M - Mostrar totes les persones\n");
        printf("S - Sortir\n");
        printf("Tria una opció: ");
        scanf(" %c", &opcio);

        switch(opcio) {
            case 'C':
            case 'c':
                printf("Introdueix el cognom a cercar: ");
                scanf("%s", cognom);
                cercarIImprimir(persones, n, cognom);
                break;
            case 'A':
            case 'a': {
                Persona nova;
                printf("Nom: ");
                scanf("%s", nova.nom);
                printf("Cognom: ");
                scanf("%s", nova.cognom);
                printf("DNI: ");
                scanf("%s", nova.dni);
                printf("Edat: ");
                scanf("%d", &nova.edat);

                n += 1;
                persones = (Persona*)realloc(persones, n * sizeof(Persona));
                if (!persones) {
                    fprintf(stderr, "Error redimensionant memòria.\n");
                    exit(1);
                }

                persones[n - 1] = nova;
                ordenacioPerInsercio(persones, n);

                printf("Persona afegida i llista reordenada.\n");
                break;
            }
            case 'M':
            case 'm':
                mostrarTotes(persones, n);
                break;
            case 'S':
            case 's':
                guardarFitxer(fitxer, persones, n);
                free(persones);
                printf("Sortint...\n");
                return 0;
            default:
                printf("Opció no vàlida.\n");
        }
    } while (1);
}