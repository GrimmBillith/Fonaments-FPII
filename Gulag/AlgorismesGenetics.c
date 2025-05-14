
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/** Constants per defecte **/
#define NUM_GENS 30
#define TARGET 1977
#define MAX_GEN 100
#define POP_SIZE 40
#define PMUT 0.05
#define K_TOURN 5

/**
 * @brief Estructura d’un cromosoma: vector de bits i error
 */
typedef struct {
    int gens[NUM_GENS];
    int error;
} Cromosoma;

/**
 * @brief Calcula l’error d’un cromosoma segons la fórmula donada
 * @param v Vector de bits del cromosoma
 * @return Valor absolut de la diferència entre la suma ponderada i el valor objectiu
 *
 * Aquesta funció és fonamental ja que defineix el criteri d'optimització. 
 * A la pràctica, l'algorisme intentarà minimitzar aquest error fins arribar a zero.
 */
int calcula_error(const int v[NUM_GENS]) {
    int suma = 0;
    for (int i = 0; i < NUM_GENS; i++)
        suma += v[i] * (i+1) * (i+1); /** (i+1) perquè la posició comença a 1 **/
    return abs(suma - TARGET); /** Valor absolut de la diferència amb el valor objectiu **/
}

/**
 * @brief Inicialitza la població amb cromosomes aleatoris
 * @param poblacio Vector de cromosomes a inicialitzar
 * @param n Nombre d'individus de la població
 *
 * La diversitat inicial és clau: si tots els cromosomes fossin iguals, l'algorisme quedaria estancat.
 */
void inicialitza_poblacio(Cromosoma poblacio[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < NUM_GENS; j++)
            poblacio[i].gens[j] = rand() % 2; /** Assigna 0 o 1 aleatòriament **/
        poblacio[i].error = calcula_error(poblacio[i].gens); /** Calcula l’error del cromosoma **/
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
 *
 * L'encreuament permet combinar informació de dos pares. Això pot crear noves combinacions útils.
 * Si el punt de tall és sempre el mateix, la diversitat es redueix; per això es tria aleatòriament.
 */
void crossover(const int pare1[NUM_GENS], const int pare2[NUM_GENS], int fill1[NUM_GENS], int fill2[NUM_GENS]) {
    int punt = rand() % (NUM_GENS - 1) + 1; /** Punt de tall entre 1 i 29 **/
    for (int i = 0; i < punt; i++) {
        fill1[i] = pare1[i];
        fill2[i] = pare2[i];
    }
    for (int i = punt; i < NUM_GENS; i++) {
        fill1[i] = pare2[i];
        fill2[i] = pare1[i];
    }
}

/**
 * @brief Mutació flip: cada gen té probabilitat de canviar
 * @param gens Vector de bits del cromosoma
 * @param prob Probabilitat de mutació per cada gen
 *
 * La mutació evita que la població es quedi atrapada en màxims locals.
 * Massa mutació pot fer perdre bones solucions; massa poca pot fer que tot convergeixi massa ràpid.
 */
void mutacio(int gens[NUM_GENS], double prob) {
    for (int i = 0; i < NUM_GENS; i++)
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
 */
void mostra_cromosoma(const Cromosoma *c) {
    for (int i = 0; i < NUM_GENS; i++)
        printf("%d", c->gens[i]);
}

/**
 * @brief Llegeix els paràmetres de la línia de comandes o assigna valors per defecte
 * @param argc Nombre d'arguments
 * @param argv Vector d'arguments
 * @param generacions Punter a nombre de generacions
 * @param n Punter a nombre de cromosomes
 * @param pmut Punter a probabilitat de mutació
 * @param k Punter a paràmetre K del torneig
 *
 * Permet que l'usuari experimenti amb diferents paràmetres i vegi com afecta la cerca.
 */
void llegeix_parametres(int argc, char *argv[], int *generacions, int *n, double *pmut, int *k) {
    *generacions = (argc > 1) ? atoi(argv[1]) : MAX_GEN; /** Si hi ha argument, el llegeix, sinó valor per defecte **/
    *n = (argc > 2) ? atoi(argv[2]) : POP_SIZE;
    *pmut = (argc > 3) ? atof(argv[3]) : PMUT;
    *k = (argc > 4) ? atoi(argv[4]) : K_TOURN;
}

/**
 * @brief Funció principal del programa. Controla el flux de l'algorisme genètic.
 * @param argc Nombre d'arguments de la línia de comandes
 * @param argv Vector d'arguments de la línia de comandes
 * @return 0 si tot ha anat bé
 *
 * El bucle principal fa evolucionar la població. Si la mutació és massa baixa, la població pot convergir massa ràpid i quedar atrapada. 
 * Si la selecció és massa forta, també es pot perdre diversitat. 
 * A la llarga, el procés tendeix a trobar una bona solució, però no sempre la òptima absoluta.
 */
int main(int argc, char *argv[]) {
    srand(time(NULL)); /** Inicialitza la llavor aleatòria **/

    int generacions, n, k;
    double pmut;
    llegeix_parametres(argc, argv, &generacions, &n, &pmut, &k); /** Llegeix els paràmetres **/

    Cromosoma *poblacio = malloc(n * sizeof(Cromosoma)); /** Reserva memòria per la població **/
    Cromosoma *nova_pob = malloc(n * sizeof(Cromosoma)); /** Reserva memòria per la nova població **/
    Cromosoma millor_solucio;
    int generacio_millor = 0;

    inicialitza_poblacio(poblacio, n); /** Inicialitza la població inicial **/
    millor_solucio = poblacio[millor_cromosoma(poblacio, n)]; /** Guarda el millor cromosoma inicial **/
    generacio_millor = 1;

    /** Evolució per generacions **/
    for (int gen = 0; gen < generacions; gen++) {
        int idx_millor = millor_cromosoma(poblacio, n); /** Troba el millor cromosoma de la generació **/
        printf("Generacio %d: Millor combinacio: ", gen+1);
        mostra_cromosoma(&poblacio[idx_millor]);
        printf("  Error: %d\n", poblacio[idx_millor].error);

        /** Actualitza el millor cromosoma global si cal **/
        if (poblacio[idx_millor].error < millor_solucio.error) {
            millor_solucio = poblacio[idx_millor];
            generacio_millor = gen+1;
        }

        /** Si trobem error 0, aturem l’evolució abans d’arribar al màxim **/
        if (millor_solucio.error == 0)
            break;

        /** Nova generació: selecció, crossover, mutació **/
        for (int i = 0; i < n; i += 2) {
            int idx1 = tournament_selection(poblacio, n, k); /** Selecciona el primer pare **/
            int idx2 = tournament_selection(poblacio, n, k); /** Selecciona el segon pare **/

            int fill1[NUM_GENS], fill2[NUM_GENS];
            crossover(poblacio[idx1].gens, poblacio[idx2].gens, fill1, fill2); /** Genera dos fills nous **/

            mutacio(fill1, pmut); /** Aplica mutació al primer fill **/
            mutacio(fill2, pmut); /** Aplica mutació al segon fill **/

            memcpy(nova_pob[i].gens, fill1, sizeof(fill1)); /** Copia els gens al cromosoma de la nova població **/
            nova_pob[i].error = calcula_error(fill1);

            if (i+1 < n) {
                memcpy(nova_pob[i+1].gens, fill2, sizeof(fill2));
                nova_pob[i+1].error = calcula_error(fill2);
            }
        }

        /** Canvi de generació: intercanvi de punters per reutilitzar memòria **/
        Cromosoma *tmp = poblacio;
        poblacio = nova_pob;
        nova_pob = tmp;
    }

    printf("\nMillor contrasenya trobada: ");
    mostra_cromosoma(&millor_solucio);
    printf("\nError: %d\n", millor_solucio.error);
    printf("Trobada a la generacio: %d\n", generacio_millor);

    free(poblacio); /** Allibera la memòria de la població **/
    free(nova_pob); /** Allibera la memòria de la nova població **/
    return 0;
}

