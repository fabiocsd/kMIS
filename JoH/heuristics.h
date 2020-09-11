#ifndef KMIS_HEURISTICS_H
#define KMIS_HEURISTICS_H

#include "graph.h"
#include "grasp.h"

using namespace std;

/**
Heurística kInter (Artigo do sbpo 2013).
**/
Solucao heuristica_kinter(Graph &graph);

/**
Heurística kInter estendida.
**/
Solucao heuristica_kinter_estendida(Graph &graph);

/**
Heurística kInter estendida sendo que sempre gero a solução mesmo que a solução parcial tenha valor
menor que a melhor até agora encotrado.
**/
Solucao heuristica_kinter_estendida_path_relinking(Graph &graph);

#endif // KMIS_HEURISTICS_H
