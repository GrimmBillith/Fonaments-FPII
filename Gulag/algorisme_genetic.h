// algorisme_genetic.h
#ifndef ALGORISME_GENETIC_H
#define ALGORISME_GENETIC_H

/**
 * @brief Estructura d’un cromosoma: vector de bits i error
 */
typedef struct {
    int *gens;
    int error;
} Cromosoma;

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
int calcula_error(const int v[], int num_gens, int target);

/**
 * @brief Inicialitza la població amb cromosomes aleatoris
 * @param poblacio Vector de cromosomes a inicialitzar
 * @param n Nombre d'individus de la població
 * @param num_gens Nombre de gens per cromosoma
 * @param target Valor objectiu
 *
 * La diversitat inicial és clau: si tots els cromosomes fossin iguals, l'algorisme quedaria estancat.
 */
void inicialitza_poblacio(Cromosoma poblacio[], int n, int num_gens, int target);

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
int tournament_selection(Cromosoma poblacio[], int n, int k);

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
void crossover(const int pare1[], const int pare2[], int fill1[], int fill2[], int num_gens);

/**
 * @brief Mutació flip: cada gen té probabilitat de canviar
 * @param gens Vector de bits del cromosoma
 * @param prob Probabilitat de mutació per cada gen
 * @param num_gens Nombre de gens del cromosoma
 *
 * La mutació evita que la població es quedi atrapada en màxims locals.
 * Massa mutació pot fer perdre bones solucions; massa poca pot fer que tot convergeixi massa ràpid.
 */
void mutacio(int gens[], double prob, int num_gens);

/**
 * @brief Troba l’índex del millor cromosoma de la població
 * @param poblacio Vector de cromosomes
 * @param n Nombre d'individus de la població
 * @return Índex del millor cromosoma
 *
 * Aquesta funció permet saber en cada generació quina és la millor solució trobada fins ara.
 */
int millor_cromosoma(Cromosoma poblacio[], int n);

/**
 * @brief Mostra un cromosoma per pantalla
 * @param c Punter al cromosoma a mostrar
 * @param num_gens Nombre de gens del cromosoma
 */
void mostra_cromosoma(const Cromosoma *c, int num_gens);

#endif // ALGORISME_GENETIC_H