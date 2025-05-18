// algorisme_genetic.c
#include "algorisme_genetic.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/**
 * @brief Calcula l’error d’un cromosoma segons la fórmula donada
 * @param v Vector de bits del cromosoma
 * @param num_gens Nombre de gens del cromosoma
 * @param target Valor objectiu
 * @return Valor absolut de la diferència entre la suma ponderada i el valor objectiu
 *
 * Aquesta funció és fonamental ja que defineix el criteri d'optimització.
 * A la pràctica, l'algorisme intentarà minimitzar aquest error fins arribar a zero.
 */
int calcula_error(const int v[], int num_gens, int target) {
    int suma = 0;
    for (int i = 0; i < num_gens; i++)
        suma += v[i] * (i + 1) * (i + 1); /** (i+1) perquè la posició comença a 1 **/
    return abs(suma - target); /** Valor absolut de la diferència amb el valor objectiu **/
}

/**
 * @brief Inicialitza la població amb cromosomes aleatoris
 * @param poblacio Vector de cromosomes a inicialitzar
 * @param n Nombre d'individus de la població
 * @param num_gens Nombre de gens per cromosoma
 * @param target Valor objectiu
 *
 * La diversitat inicial és clau: si tots els cromosomes fossin iguals, l'algorisme quedaria estancat.
 */
void inicialitza_poblacio(Cromosoma poblacio[], int n, int num_gens, int target) {
    for (int i = 0; i < n; i++) {
        poblacio[i].gens = (int *)malloc(num_gens * sizeof(int));
        if (poblacio[i].gens == NULL) {
            perror("Error en l'assignació de memòria per al cromosoma");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < num_gens; j++)
            poblacio[i].gens[j] = rand() % 2; /** Assigna 0 o 1 aleatòriament **/
        poblacio[i].error = calcula_error(poblacio[i].gens, num_gens, target); /** Calcula l’error del cromosoma **/
    }
}

/**
 * @brief Selecció per torneig: retorna l’índex del millor de K individus aleatoris
 * @param poblacio Vector de cromosomes
 * @param n Nombre d'individus de la població
 * @param k Nombre d'individus seleccionats per torneig
 * @return Índex del millor cromosoma seleccionat
 *
 * La selecció per torneig afavoreix els individus amb millor error, però manté certa diversitat.
 * Si K és molt gran, la pressió selectiva és molt alta i pot fer perdre diversitat massa ràpid.
 */
int tournament_selection(Cromosoma poblacio[], int n, int k) {
    int millor = rand() % n; /** Selecciona un individu aleatori com a millor inicial **/
    for (int i = 1; i < k; i++) {
        int idx = rand() % n; /** Escull un altre individu aleatori **/
        if (poblacio[idx].error < poblacio[millor].error)
            millor = idx; /** Si el nou té millor error, l’actualitzem **/
    }
    return millor;
}

/**
 * @brief One-point crossover per generar dos fills a partir de dos pares
 * @param pare1 Vector de bits del primer pare
 * @param pare2 Vector de bits del segon pare
 * @param fill1 Vector de bits del primer fill (sortida)
 * @param fill2 Vector de bits del segon fill (sortida)
 * @param num_gens Nombre de gens per cromosoma
 *
 * L'encreuament permet combinar informació de dos pares. Això pot crear noves combinacions útils.
 * Si el punt de tall és sempre el mateix, la diversitat es redueix; per això es tria aleatòriament.
 */
void crossover(const int pare1[], const int pare2[], int fill1[], int fill2[], int num_gens) {
    int punt = rand() % (num_gens - 1) + 1; /** Punt de tall entre 1 i num_gens-1 **/
    for (int i = 0; i < punt; i++) {
        fill1[i] = pare1[i];
        fill2[i] = pare2[i];
    }
    for (int i = punt; i < num_gens; i++) {
        fill1[i] = pare2[i];
        fill2[i] = pare1[i];
    }
}

/**
 * @brief Mutació flip: cada gen té probabilitat de canviar
 * @param gens Vector de bits del cromosoma
 * @param prob Probabilitat de mutació per cada gen
 * @param num_gens Nombre de gens del cromosoma
 *
 * La mutació evita que la població es quedi atrapada en màxims locals.
 * Massa mutació pot fer perdre bones solucions; massa poca pot fer que tot convergeixi massa ràpid.
 */
void mutacio(int gens[], double prob, int num_gens) {
    for (int i = 0; i < num_gens; i++)
        if ((double)rand() / RAND_MAX < prob) /** Probabilitat de mutació per cada gen **/
            gens[i] = 1 - gens[i]; /** Inverteix el bit **/
}

/**
 * @brief Troba l’índex del millor cromosoma de la població
 * @param poblacio Vector de cromosomes
 * @param n Nombre d'individus de la població
 * @return Índex del millor cromosoma
 *
 * Aquesta funció permet saber en cada generació quina és la millor solució trobada fins ara.
 */
int millor_cromosoma(Cromosoma poblacio[], int n) {
    int millor = 0;
    for (int i = 1; i < n; i++)
        if (poblacio[i].error < poblacio[millor].error)
            millor = i;
    return millor;
}

/**
 * @brief Mostra un cromosoma per pantalla
 * @param c Punter al cromosoma a mostrar
 * @param num_gens Nombre de gens del cromosoma
 */
void mostra_cromosoma(const Cromosoma *c, int num_gens) {
    for (int i = 0; i < num_gens; i++)
        printf("%d", c->gens[i]);
}