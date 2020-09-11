#ifndef KMIS_GRASP_H
#define KMIS_GRASP_H

#include "graph.h"

using namespace std;

#define NUMBER_VALUES 11


class Grasp{
public:
    Grasp(Graph &);
    Grasp();

    Solucao construcao(double alfa);
    void busca_local(Solucao &solucao);
    Solucao path_relinking(Solucao &solucao);
    Solucao grasp();
    Solucao grasp_reativo();

    int get_alfa();
    void atualizacao_probabilidade(int Z_max);

    int qtd_max_iteracoes = 500;
    int max_elite = 10;//Tamanho máximo da lista elite.
    double cons_X[NUMBER_VALUES] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    double prob_X[NUMBER_VALUES];
    int cont[NUMBER_VALUES];
    double score[NUMBER_VALUES];
    double avg[NUMBER_VALUES];
    int melhor_solucao_construcao = -1;
    vector<Solucao> solucoes_elites;//Lista contendo as possiveis soluções destinos.

private:
    Graph graph;
};





#endif // KMIS_GRASP_H
