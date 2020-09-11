#ifndef KMIS_VNS_H
#define KMIS_VNS_H
#include <cmath>
#include "graph.h"
#include "vnd.h"
#include "grasp.h"

using namespace std;

/*
Código da estrutura Lista simplesmente encadeada
*/
typedef struct item{
   bitset<nElem> chave;
   struct item* proximo;
}Item;

typedef struct {
   Item *inicio;
   int tamanho;
}Lista;

Lista* criarLista();
void addLista(Lista* fila, bitset<nElem> elem);
bool testList(Lista* fila, bitset<nElem> elem, int k);
void liberarLista(Lista* fila);
//##############################################################################

/**Algoritmo VNS Reativo.**/
Solucao VNS_Reativo(Graph &graph, Solucao &solucao_entrada);



#endif // KMIS_VNS_H
