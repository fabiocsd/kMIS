#include <cstdio>
#include <cfloat>     // for DBL_MAX
#include <cmath>      // for floor

#include "heuristics.h"

/**
Heurística gulosa do artigo do sbpo 2013.
Heurística kInter.
**/
Solucao heuristica_kinter(Graph &graph){
    Solucao solucao(graph.tam_L);
    vector<Elemento> conjuntos = graph.conjuntosPrincipal;

    std::sort(conjuntos.begin(), conjuntos.end(), Elemento::LessThanByAge());
    Elemento e = conjuntos[0];
    solucao.addSubConjunto(e.id);
    bitset<nElem> vetor_solu_parcial = e.vectorBits;

    int kk = 1;
    while(kk < graph.k){
        for (int i = kk; i < conjuntos.size(); i++) {
            Elemento aux = conjuntos[i];
            bitset<nElem> vetor_aux = Interseccao(vetor_solu_parcial, aux.vectorBits);
            aux.vectorBits = vetor_aux;
            aux.n = vetor_aux.count();
            conjuntos[i] = aux;
        }

        std::sort(conjuntos.begin() + kk, conjuntos.end(), Elemento::LessThanByAge());
        Elemento aux = conjuntos[kk];
        vetor_solu_parcial = aux.vectorBits;
        solucao.addSubConjunto(aux.id);
        kk++;
    }
    solucao.vectorBits = vetor_solu_parcial;
    conjuntos.erase(conjuntos.begin(),conjuntos.end());

    return solucao;
}

Solucao heuristica_kinter_estendida(Graph &graph){
    vector<Elemento> conjuntos, conjuntosPrincipalAux = graph.conjuntosPrincipal;
    int metrica = 0;//Limite inferior
    bitset<nElem> bitset_melhor_solucao;
    vector<int> melhor_solucao;
    int t = 0;
    std::sort(conjuntosPrincipalAux.begin(), conjuntosPrincipalAux.end(), Elemento::LessThanByAge());
    Elemento x = conjuntosPrincipalAux[0];

    while(metrica < x.vectorBits.count() && t < graph.tam_L){
        vector<int> solucao_parcial;
        conjuntos = conjuntosPrincipalAux;
        //Coloca o subconjunto inicial na primeira posição
        Elemento aux = conjuntos[0];
        conjuntos[0] = conjuntos[t];
        conjuntos[t] = aux;

        Elemento e = conjuntos[0];
        solucao_parcial.push_back(e.id);

        int kk = 1;
        while(kk < graph.k){
            for (int i = kk; i < conjuntos.size(); i++) {
                Elemento aux = conjuntos[i];
                bitset<nElem> vetor_aux = Interseccao(e.vectorBits, aux.vectorBits);
                aux.vectorBits = vetor_aux;
                aux.n = vetor_aux.count();
                conjuntos[i] = aux;
            }

            std::sort(conjuntos.begin() + kk, conjuntos.end(), Elemento::LessThanByAge());
            Elemento aux = conjuntos[kk];
            e.vectorBits = aux.vectorBits;
            solucao_parcial.push_back(aux.id);
            kk++;

            if(e.vectorBits.count() < metrica) break;
        }
        if(e.vectorBits.count() > metrica){
            metrica = e.vectorBits.count();
            bitset_melhor_solucao = e.vectorBits;
            melhor_solucao = solucao_parcial;
            if(metrica == graph.tam_R) break;
        }

        t++;
        if(t < graph.tam_L) x = conjuntosPrincipalAux[t];

        conjuntos.erase(conjuntos.begin(),conjuntos.end());
    }
    conjuntosPrincipalAux.erase(conjuntosPrincipalAux.begin(),conjuntosPrincipalAux.end());

    Solucao solucao(graph.tam_L);
    solucao.vectorBits = bitset_melhor_solucao;
    solucao.elem = melhor_solucao;
    for(int i = 0; i < melhor_solucao.size(); i++){
        solucao.is_elem[melhor_solucao[i]] = true;
    }
    return solucao;
}



/**
Heurística kInter estendida sendo que sempre gero a solução mesmo que a solução parcial tenha valor
menor que a melhor até agora encotrado.
**/
Solucao heuristica_kinter_estendida_path_relinking(Graph &graph){
    Grasp grasp(graph);
    //solucoes_elites.clear();
    vector<Elemento> conjuntos, conjuntosPrincipalAux = graph.conjuntosPrincipal;
    int custo_melhor_solucao = 0;
    bitset<nElem> bitset_melhor_solucao;
    vector<int> melhor_solucao;
    int t = 0;
    std::sort(conjuntosPrincipalAux.begin(), conjuntosPrincipalAux.end(), Elemento::LessThanByAge());
    Elemento x = conjuntosPrincipalAux[0];

    while(t < graph.tam_L){
        vector<int> solucao_parcial;
        conjuntos = conjuntosPrincipalAux;
        //Coloca o subconjunto inicial na primeira posição
        Elemento aux = conjuntos[0];
        conjuntos[0] = conjuntos[t];
        conjuntos[t] = aux;

        Elemento e = conjuntos[0];
        solucao_parcial.push_back(e.id);

        int kk = 1;
        while(kk < graph.k){
            for (int i = kk; i < conjuntos.size(); i++) {
                Elemento aux = conjuntos[i];
                bitset<nElem> vetor_aux = Interseccao(e.vectorBits, aux.vectorBits);
                aux.vectorBits = vetor_aux;
                aux.n = vetor_aux.count();
                conjuntos[i] = aux;
            }

            std::sort(conjuntos.begin() + kk, conjuntos.end(), Elemento::LessThanByAge());
            Elemento aux = conjuntos[kk];
            e.vectorBits = aux.vectorBits;
            solucao_parcial.push_back(aux.id);
            kk++;
        }

        Solucao solucao_path(graph.tam_L);
        solucao_path.vectorBits = e.vectorBits;
        solucao_path.elem = solucao_parcial;
        for(int i = 0; i < solucao_parcial.size(); i++){
            solucao_path.is_elem[solucao_parcial[i]] = true;
        }

        if(grasp.solucoes_elites.size() >= 1){
                solucao_path = grasp.path_relinking(solucao_path);
        }
        if(grasp.solucoes_elites.size() < grasp.max_elite) grasp.solucoes_elites.push_back(solucao_path);
        else{
                int menor_dif = graph.k + 12;
                int posicao_menor_dif = -1;
                for(int j = 0; j < grasp.solucoes_elites.size(); j++){
                        if(solucao_path.vectorBits.count() > grasp.solucoes_elites[j].vectorBits.count()){
                                int dif = diferenca_simetrica(solucao_path, grasp.solucoes_elites[j], graph.k);
                                if(menor_dif > dif){
                                        menor_dif = dif;
                                        posicao_menor_dif = j;
                                }
                        }
                }
                if(posicao_menor_dif != -1){
                        grasp.solucoes_elites[posicao_menor_dif] = solucao_path;
                }
        }

        if(solucao_path.vectorBits.count() > custo_melhor_solucao){
            custo_melhor_solucao = solucao_path.vectorBits.count();
            bitset_melhor_solucao = solucao_path.vectorBits;
            melhor_solucao = solucao_path.elem;
            if(custo_melhor_solucao == graph.tam_R) break;
        }

        t++;
        if(t < graph.tam_L) x = conjuntosPrincipalAux[t];

        conjuntos.clear();

    }
    conjuntosPrincipalAux.clear();

    Solucao solucao(graph.tam_L);
    solucao.vectorBits = bitset_melhor_solucao;
    solucao.elem = melhor_solucao;
    for(int i = 0; i < melhor_solucao.size(); i++){
        solucao.is_elem[melhor_solucao[i]] = true;
    }
    return solucao;
}
