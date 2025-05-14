# Projecte Final: Algorismes Genètics (C)

## Descripció

Aquest projecte implementa un algorisme genètic en llenguatge C per trobar una contrasenya binària de 30 bits que compleixi una condició matemàtica concreta. El repte consisteix a trobar un vector de 30 bits tal que la suma de cada bit multiplicat pel quadrat de la seva posició sigui igual a 1977. El problema està inspirat en una situació de seguretat fictícia on cal obrir una caixa forta.

L’algorisme genètic utilitza els mecanismes clàssics de selecció per torneig, encreuament d’un punt, mutació flip i relleu generacional per evolucionar la població de possibles solucions fins trobar la millor resposta.

## Funcionament
#### Inicialització: 
Es crea una població de cromosomes (vectors de 30 bits) amb valors aleatoris.

#### Selecció: 
S’utilitza el mètode tournament selection per escollir els pares.

#### Crossover: 
Es fa un one-point crossover per generar fills a partir dels pares.

#### Mutació: 
S’aplica flip mutation a cada gen amb una probabilitat determinada.

#### Supervivència: 
Es reemplaça tota la població per la nova generació.

#### Millor solució: 
Es guarda i mostra el millor cromosoma de cada generació i, al final, la millor solució trobada.

## Paràmetres configurables
#### Pots passar els següents paràmetres per línia de comandes (en aquest ordre):

#### Nombre de generacions 
(per defecte: 100)

#### Nombre de cromosomes 
(per defecte: 40)

#### Probabilitat de mutació 
(per defecte: 0.05)

#### Valor K pel torneig 
(per defecte: 5)

### Exemple d’execució amb paràmetres personalitzats:

```c
./algorisme_genetic 200 60 0.02 7
```

### Compilació i execució

```c
gcc -o algorisme_genetic main.c -lm
./algorisme_genetic

```


O afegeix els paràmetres que vulguis després del nom del programa.

## Sortida per pantalla
Per cada generació es mostra:

```c
Generacio X: Millor combinacio: <vector_binari>  Error: <valor_error>
```
Al final es mostra:

```c
Millor contrasenya trobada: <vector_binari>
Error: <valor_error>
Trobada a la generacio: <num_generacio>
```
