#ifndef INDICES
#define INDICES

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "filmes.h"

#ifndef STRING
#define STRING
typedef char *string;
#endif

#define CONSISTENTE '0'
#define INCONSISTENTE '1'

//Definições de estruturas ---------------------------------------------------------------------------------------------

//Nó que representa uma entrada no índice primário
typedef struct noP {
    //Chave primária, o código do filme
    string codigo;
    //RNN, a posição da entrada no arquivo de índice
    int rnn;
    //Ponteiro para o próximo NoP na lista
    struct noP *prox;
} NoP;

//Nó que representa um código de filme em uma lista de códigos
typedef struct noCodigo {
    //Código do filme
    string codigo;
    //Ponteiro para o próximo NoChave na lista
    struct noCodigo *prox;
} NoCodigo;

//Nó que representa uma entrada no índice secundário
typedef struct noS {
    //Chave secundária, o título do filme
    string titulo;
    //Ponteiro para o início da lista de NoCodigo, representando as chaves do índice primário
    NoCodigo *head;
    //Ponteiro para o próximo NoS na lista
    struct noS *prox;
} NoS;

//Representação na memória do índice primário
typedef struct {
    //Ponteiro para o começo de uma lista de NoP, representando as entradas do índice primário
    NoP *head;
    int tamanho;
} IndiceP;

//Representação na memória do índice secundário
typedef struct {
    //Ponteiro para o começo de uma lista de NoS, representando as entradas do índice secundário
    NoS *head;
    int tamanho;
} IndiceS;

//Declarações de Funções -----------------------------------------------------------------------------------------------

//Cria um novo NoP, que contém um código e o endereço correspondente, e aponta para NULL
NoP *newNoP(string codigo, int rnn);

//Cria um novo IndiceP (uma fila vazia de NoP)
IndiceP *newIndiceP();

//Insere ordenadamente um NoP na lista do IndiceP
void insereNoP(IndiceP *index, NoP *no);

//Por busca sequencial, retorna o endereço do NoP com chave codigo; NULL se não está presente
NoP *buscaNoP(IndiceP *index, string codigo);

//Remove o NoP correspondente ao codigo
void removeNoP(IndiceP *index, string codigo);

//Lê um arquivo de índice primário, e monta um IndiceP com as informações
IndiceP *lerP(FILE *iprimary);

//Cria um IndiceP a partir do arquivo de dados
IndiceP *refazerP(FILE *movies);

//Salva as informações de um IndiceP em um arquivo
void saveIndiceP(IndiceP *index);

//Libera o espaço alocado para um IndiceP na memória
void freeIndiceP(IndiceP *index);

//Cria um novo NoCodigo, que contém um código, e aponta para NULL;
NoCodigo *newNoCodigo(string codigo);

//Insere ordenadamente um NoCodigo na lista que começa em head
void insereNoCodigo(NoS *noS, NoCodigo *noC);

//Remove o código associado a um título no índice secundário
void removeNoCodigo(NoS *no, string codigo);

//Cria um novo NoS, que contém um titulo, tem um ponteiro para uma lista vazia de NoChave, e aponta para NULL
NoS *newNoS(string titulo);

//Cria um novo IndiceS (fila vazia de NoS)
IndiceS *newIndiceS();

//Insere ordenadamente um NoS na lista do IndiceS
void insereNoS(IndiceS *index, NoS *no);

//Por busca sequencial, retorna o endereço do NoS com a chave titulo; NULL se não está presente
NoS *buscaNoS(IndiceS *index, string titulo);

//Remove o título do índice secundário
void removeNoS(IndiceS *index, string titulo);

//Lê um arquivo de índice secundário, e monta um IndiceS com as informações
IndiceS *lerS(FILE *ititle);

//Cria um IndiceS a partir do arquivo de dados
IndiceS *refazerS(FILE *movies);

//Salva as informações de IndiceS em um arquivo
void saveIndiceS(IndiceS *index);

//Libera a lista de códigos dentro de um NoS
void freeCodigos(NoCodigo *head);

//Libera o espaço alocado para um IndiceS na memória
void freeIndiceS(IndiceS *index);

//Retorna o RNN de uma entrada de filme com um dado código; -1 se o código não está presente no IndiceP
int rnnFromCodigo(IndiceP *index, string codigo);

//Insere o filme com o códgio e titulo correspondentes nos índices
void insereFilme(IndiceP *indexP, IndiceS *indexS, string codigo, string titulo, int rnn);

//Remove o filme com o código e titulo correspondente dos índices
void removeFilmeFromIndice(IndiceP *indexP, IndiceS *indexS, string codigo, string titulo);

#endif