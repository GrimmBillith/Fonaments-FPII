// trobar_clau.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "algorisme_genetic.h"

/** Constants per defecte **/
#define NUM_GENS_DEF 30
#define TARGET_DEF 1977
#define MAX_GEN_DEF 100
#define POP_SIZE_DEF 40
#define PMUT_DEF 0.05
#define K_TOURN_DEF 5

/**
 * @brief Llegeix els paràmetres de la línia de comandes o assigna valors per defecte.
 */
void llegeix_parametres(int argc, char *argv[], int *generacions, int *n, double *pmut, int *k, int *num_gens, int *target) {
    *generacions = MAX_GEN_DEF;
    *n = POP_SIZE_DEF;
    *pmut = PMUT_DEF;
    *k = K_TOURN_DEF;
    *num_gens = NUM_GENS_DEF;
    *target = TARGET_DEF;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--generacions=", 14) == 0) {
            *generacions = atoi(argv[i] + 14);
        } else if (strncmp(argv[i], "--poblacio=", 11) == 0) {
            *n = atoi(argv[i] + 11);
        } else if (strncmp(argv[i], "--mutacio=", 10) == 0) {
            *pmut = atof(argv[i] + 10);
        } else if (strncmp(argv[i], "--torneig=", 11) == 0) {
            *k = atoi(argv[i] + 11);
        } else if (strncmp(argv[i], "--gens=", 7) == 0) {
            *num_gens = atoi(argv[i] + 7);
        } else if (strncmp(argv[i], "--objectiu=", 11) == 0) {
            *target = atoi(argv[i] + 11);
        } else if (i == 1) { *generacions = atoi(argv[i]); }
        else if (i == 2) { *n = atoi(argv[i]); }
        else if (i == 3) { *pmut = atof(argv[i]); }
        else if (i == 4) { *k = atoi(argv[i]); }
        else if (i == 5) { *num_gens = atoi(argv[i]); }
        else if (i == 6) { *target = atoi(argv[i]); }
    }
}

/**
 * @brief Funció principal del programa. Controla el flux de l'algorisme genètic.
 */
int main(int argc, char *argv[]) {
    srand(time(NULL));

    int generacions, n, k, num_gens, target;
    double pmut;
    llegeix_parametres(argc, argv, &generacions, &n, &pmut, &k, &num_gens, &target);

    Cromosoma *poblacio = (Cromosoma *)malloc(n * sizeof(Cromosoma));
    Cromosoma *nova_pob = (Cromosoma *)malloc(n * sizeof(Cromosoma));
    Cromosoma millor_solucio;
    int generacio_millor = 0;

    // Inicialitzem cada cromosoma
    for (int i = 0; i < n; i++) {
        poblacio[i].gens = (int *)malloc(num_gens * sizeof(int));
        nova_pob[i].gens = (int *)malloc(num_gens * sizeof(int));
    }

    inicialitza_poblacio(poblacio, n, num_gens, target);
    int idx_inicial = millor_cromosoma(poblacio, n);
    millor_solucio = poblacio[idx_inicial];
    generacio_millor = 1;

    for (int gen = 0; gen < generacions; gen++) {
        int idx_millor = millor_cromosoma(poblacio, n);
        printf("Generacio %d: Millor combinacio: ", gen + 1);
        mostra_cromosoma(&poblacio[idx_millor], num_gens);
        printf("  Error: %d\n", poblacio[idx_millor].error);

        if (poblacio[idx_millor].error < millor_solucio.error) {
            millor_solucio = poblacio[idx_millor];
            generacio_millor = gen + 1;
        }

        if (millor_solucio.error == 0)
            break;

        // Nova generació
        for (int i = 0; i < n; i += 2) {
            int idx1 = tournament_selection(poblacio, n, k);
            int idx2 = tournament_selection(poblacio, n, k);

            int *fill1 = (int *)malloc(num_gens * sizeof(int));
            int *fill2 = (int *)malloc(num_gens * sizeof(int));
            if (!fill1 || !fill2) {
                perror("Error en l'assignació de memòria");
                exit(EXIT_FAILURE);
            }

            crossover(poblacio[idx1].gens, poblacio[idx2].gens, fill1, fill2, num_gens);
            mutacio(fill1, pmut, num_gens);
            mutacio(fill2, pmut, num_gens);

            memcpy(nova_pob[i].gens, fill1, num_gens * sizeof(int));
            nova_pob[i].error = calcula_error(fill1, num_gens, target);

            if (i + 1 < n) {
                memcpy(nova_pob[i + 1].gens, fill2, num_gens * sizeof(int));
                nova_pob[i + 1].error = calcula_error(fill2, num_gens, target);
            }

            free(fill1);
            free(fill2);
        }

        // Copiem els nous gens a la població actual
        for (int i = 0; i < n; i++) {
            free(poblacio[i].gens);
            poblacio[i].gens = (int *)malloc(num_gens * sizeof(int));
            memcpy(poblacio[i].gens, nova_pob[i].gens, num_gens * sizeof(int));
            poblacio[i].error = nova_pob[i].error;
        }
    }

    printf("\nMillor contrasenya trobada: ");
    mostra_cromosoma(&millor_solucio, num_gens);
    printf("\nError: %d\n", millor_solucio.error);
    printf("Trobada a la generacio: %d\n", generacio_millor);

    // Alliberem memòria
    for (int i = 0; i < n; i++) {
        free(poblacio[i].gens);
        free(nova_pob[i].gens);
    }
    free(poblacio);
    free(nova_pob);

    return 0;
}