#ifndef KMIS_GRAPH_H
#define KMIS_GRAPH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <map>
#include <cstring>
#include <algorithm>

#define nElem 301

using namespace std;

/**
Classe que representa uma solução do problema.
**/
class Solucao{
public:
    bitset<nElem> vectorBits;//Bitset da solução (interseção)
    vector<bool> is_elem;//Vetor de tamanho igual a quantidade de vertices, onde o índice j será true se o subconjunto j está na solução.
    vector<int> elem;//Vetor com os indices dos subconjuntos que pertecem a solução.
    int tam_L;

    Solucao(int tam_L){
        this->tam_L = tam_L;
        setar();
        vectorBits.reset();
    }

    void limpar(){
        elem.clear();
        is_elem.clear();
        vectorBits.set();
        setar();
    }

    void setar(){
        for (int j = 0; j < tam_L; j++)
            is_elem.push_back(false);
    }

    void addSubConjunto(int subConj){
        elem.push_back(subConj);
        is_elem[subConj] = true;
    }
};

class Elemento{
public:
    int id;//Posicao do subconjunto no vector conjuntosPrincipal.
    int n;
    bitset<nElem> vectorBits;

    Elemento(int id){
        this->id = id;
        vectorBits.reset();
    }

    Elemento(vector<int> vetor){
        vectorBits.reset();
        n = vetor.size();
        for (int i = 0; i < vetor.size(); i++)
            vectorBits.set((size_t) vetor[i]);
    }

    struct LessThanByAge{
      bool operator()(const Elemento& lhs, const Elemento& rhs) const{
          return lhs.vectorBits.count() > rhs.vectorBits.count();
      }
    };

    bool operator== (const Elemento& other) {
        return n == other.n;
    }
};

// simple edge list representation of the graph augmented with the conflicts
class Graph{
public:
    ~Graph();

    int tam_L;
    int tam_R;
    int k;

    vector<Elemento> conjuntosPrincipal, conjuntosPrincipalOrdenado;//Vizinhanca dos subconjuntos.

    bool read_file(const char *, int);   // parse instance from gcclib file
};

bitset<nElem> Interseccao(bitset<nElem> bs1, bitset<nElem> bs2);
int randValor(int linf, int lsup);
double randValorDouble(int linf, int lsup);
int diferenca_simetrica(const Solucao &sol1, const Solucao &sol2, int k);
int get_subconjunto(vector<bool> vetor, int p);
#endif // KMIS_GRAPH_H
