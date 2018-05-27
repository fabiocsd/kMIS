
/**
Rotinas de pré-processamento como descrito no artigo "2014 - An Integer Programming Formulation for the Maximum k-Subset Intersection Problem"
**/
/**
Da maneira como a heurística 2 foi implementada, se o pré-processamento eliminar um subconjunto,
ao colocar o subconjunto da última posição mais a frente, esse pode nunca ser avaliado na heurística 2.
O interessante seria alterar as implementações tanto da heurística 2 como do pré-processamento antes de colocar o codigo no git.
TRABALHO PARA O ROBERTTY!!!
**/

void PreProcessamento(int _lambda = 0){
    bool melhorou = true;

    while(melhorou){
        melhorou = false;

        // parte 1
        for (int i = 0; i < tam_L; i++) {
            if(conjuntosPrincipal[i].vectorBits.count() < _lambda){
                melhorou = true;
                int id = conjuntosPrincipal[i].id;
                if(i < tam_L - 1){
                    conjuntosPrincipal[i] = conjuntosPrincipal[tam_L - 1];
                    conjuntosPrincipal[tam_L - 1].vectorBits.reset();
                    i--;
                }
                tam_L--;
                for (int j = 0; j < tam_R; j++) {
                    conjuntoR[j].vectorBits.reset(id);
                }
            }
        }
        for (int i = 0; i < tam_L; i++) {
            int qtd_Lu = 0;
            for (int j = 0; j < tam_L; j++) {
                if(j != i){
                    int inters = Interseccao(conjuntosPrincipal[i].vectorBits, conjuntosPrincipal[j].vectorBits).count();
                    if(inters < _lambda)
                        qtd_Lu++;
                }
            }
            if(tam_L - qtd_Lu < k){
                melhorou = true;
                int id = conjuntosPrincipal[i].id;
                if(i < tam_L - 1){
                    conjuntosPrincipal[i] = conjuntosPrincipal[tam_L - 1];
                    conjuntosPrincipal[tam_L - 1].vectorBits.reset();
                    i--;
                }
                tam_L--;
                for (int j = 0; j < tam_R; j++) {
                    conjuntoR[j].vectorBits.reset(id);
                }
            }
        }

        // parte2
        int vet_comp_R[tam_R], qtd_R_Removidos = 0;
        for (int j = 0; j < tam_R; j++){
             vet_comp_R[j] = conjuntoR[j].vectorBits.count();
             if(vet_comp_R[j] == 0) qtd_R_Removidos++;
        }

        for (int j = 0; j < tam_R; j++) {
            if(vet_comp_R[j] > 0 && vet_comp_R[j] < k){
                conjuntoR[j].vectorBits.reset();
                for (int i = 0; i < tam_L; i++) {
                    conjuntosPrincipal[i].vectorBits.reset(j);
                }
                vet_comp_R[j] = 0;
                qtd_R_Removidos++;
            }
        }

       for (int l = 0; l < tam_R; l++){
            if(vet_comp_R[l] > 0){
                int qtd_R_v = 0;
                for (int i = 0; i < tam_R; i++) {
                    if(i != l && vet_comp_R[i] > 0){
                        int interse = Interseccao(conjuntoR[i].vectorBits, conjuntoR[l].vectorBits).count();
                        if(interse < k) qtd_R_v++;
                    }
                }
                if(tam_R - qtd_R_Removidos - qtd_R_v < _lambda){
                    melhorou = true;
                    conjuntoR[l].vectorBits.reset();
                    for (int j = 0; j < tam_L; j++) {
                        conjuntosPrincipal[j].vectorBits.reset(l);
                    }
                    vet_comp_R[l] = 0;
                    qtd_R_Removidos++;
                }
            }
       }
    }
}


/**
Heurística gulosa do artigo do sbpo 2013.
**/
Solucao heuristica1(){
    Solucao solucao;
    vector<Elemento> conjuntos = conjuntosPrincipal;
    std::sort(conjuntos.begin(), conjuntos.end(), Elemento::LessThanByAge());
    Elemento e = conjuntos[0];
    solucao.addSubConjunto(e.id);
    bitset<nElem> vetor_solu_parcial = e.vectorBits;

    int kk = 1;
    while(kk < k){
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

Solucao heuristica2(){
    vector<Elemento> conjuntos, conjuntosPrincipalAux = conjuntosPrincipal;
    int metrica = 0;//Limite inferior
    bitset<nElem> bitset_melhor_solucao;
    vector<int> melhor_solucao;
    int t = 0;
    std::sort(conjuntosPrincipalAux.begin(), conjuntosPrincipalAux.end(), Elemento::LessThanByAge());
    Elemento x = conjuntosPrincipalAux[0];

    while(metrica < x.vectorBits.count() && t < tam_L){
        vector<int> solucao_parcial;
        conjuntos = conjuntosPrincipalAux;
        //Coloca o subconjunto inicial na primeira posição
        Elemento aux = conjuntos[0];
        conjuntos[0] = conjuntos[t];
        conjuntos[t] = aux;

        Elemento e = conjuntos[0];
        solucao_parcial.push_back(e.id);

        int kk = 1;
        while(kk < k){
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
            if(metrica == tam_R) break;
        }

        t++;
        if(t < tam_L) x = conjuntosPrincipalAux[t];

        conjuntos.erase(conjuntos.begin(),conjuntos.end());
    }
    conjuntosPrincipalAux.erase(conjuntosPrincipalAux.begin(),conjuntosPrincipalAux.end());

    Solucao solucao;
    solucao.vectorBits = bitset_melhor_solucao;
    solucao.elem = melhor_solucao;
    for(int i = 0; i < melhor_solucao.size(); i++){
        solucao.is_elem[melhor_solucao[i]] = true;
    }
    return solucao;
}

/**
Heurística 2 com o path_relinking do Grasp.
Nessa versão, ele adiciona na lista de soluções elites apenas se a solução melhora com relação a melhor solução anterior.
Não limita o tamanho da soluçoes elites, logo, adiciona todos que melhora.
**/
Solucao heuristica2_path_relinking(){
    solucoes_elites.clear();
    vector<Elemento> conjuntos, conjuntosPrincipalAux = conjuntosPrincipal;
    int metrica = 0;//Limite inferior
    bitset<nElem> bitset_melhor_solucao;
    vector<int> melhor_solucao;
    int t = 0;
    std::sort(conjuntosPrincipalAux.begin(), conjuntosPrincipalAux.end(), Elemento::LessThanByAge());
    Elemento x = conjuntosPrincipalAux[0];

    while(metrica < x.vectorBits.count() && t < tam_L){
        vector<int> solucao_parcial;
        conjuntos = conjuntosPrincipalAux;
        //Coloca o subconjunto inicial na primeira posição
        Elemento aux = conjuntos[0];
        conjuntos[0] = conjuntos[t];
        conjuntos[t] = aux;

        Elemento e = conjuntos[0];
        solucao_parcial.push_back(e.id);

        int kk = 1;
        int flag_break = 0;
        while(kk < k){
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

            if(e.vectorBits.count() < metrica){
                flag_break = 1;
                break;
            }
        }
        if(flag_break == 0){
            Solucao solucao_path;
            solucao_path.vectorBits = e.vectorBits;
            solucao_path.elem = solucao_parcial;
            for(int i = 0; i < solucao_parcial.size(); i++){
                solucao_path.is_elem[solucao_parcial[i]] = true;
            }

            if(e.vectorBits.count() > metrica){
                solucoes_elites.push_back(solucao_path);

                metrica = e.vectorBits.count();
                bitset_melhor_solucao = e.vectorBits;
                melhor_solucao = solucao_parcial;
                if(metrica == tam_R) break;
            }else{
                solucao_path = path_relinking(solucao_path);

                if(solucao_path.vectorBits.count() > metrica){
                    solucoes_elites.push_back(solucao_path);

                    metrica = solucao_path.vectorBits.count();
                    bitset_melhor_solucao = solucao_path.vectorBits;
                    melhor_solucao = solucao_path.elem;
                    if(metrica == tam_R) break;
                }
            }
        }

        t++;
        if(t < tam_L) x = conjuntosPrincipalAux[t];

        conjuntos.clear();
    }
    conjuntosPrincipalAux.clear();

    Solucao solucao;
    solucao.vectorBits = bitset_melhor_solucao;
    solucao.elem = melhor_solucao;
    for(int i = 0; i < melhor_solucao.size(); i++){
        solucao.is_elem[melhor_solucao[i]] = true;
    }
    return solucao;
}

/**
Heurística 2 com o path_relinking do Grasp.
Nessa versão, segue as mesmas regras do Grasp para adicionar na lista elite e limitando o seu tamanho.
**/
Solucao heuristica2_path_relinking2(){
    solucoes_elites.clear();
    vector<Elemento> conjuntos, conjuntosPrincipalAux = conjuntosPrincipal;
    int metrica = 0;//Limite inferior
    bitset<nElem> bitset_melhor_solucao;
    vector<int> melhor_solucao;
    int t = 0;
    std::sort(conjuntosPrincipalAux.begin(), conjuntosPrincipalAux.end(), Elemento::LessThanByAge());
    Elemento x = conjuntosPrincipalAux[0];

    while(metrica < x.vectorBits.count() && t < tam_L){
        vector<int> solucao_parcial;
        conjuntos = conjuntosPrincipalAux;
        //Coloca o subconjunto inicial na primeira posição
        Elemento aux = conjuntos[0];
        conjuntos[0] = conjuntos[t];
        conjuntos[t] = aux;

        Elemento e = conjuntos[0];
        solucao_parcial.push_back(e.id);

        int kk = 1;
        int flag_break = 0;
        while(kk < k){
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

            if(e.vectorBits.count() < metrica){
                flag_break = 1;
                break;
            }
        }
        if(flag_break == 0){
            Solucao solucao_path;
            solucao_path.vectorBits = e.vectorBits;
            solucao_path.elem = solucao_parcial;
            for(int i = 0; i < solucao_parcial.size(); i++){
                solucao_path.is_elem[solucao_parcial[i]] = true;
            }

            if(e.vectorBits.count() > metrica){
                metrica = e.vectorBits.count();
                bitset_melhor_solucao = e.vectorBits;
                melhor_solucao = solucao_parcial;
                if(metrica == tam_R) break;
            }

            if(solucoes_elites.size() >= 1){
                    solucao_path = path_relinking(solucao_path);
            }
            if(solucoes_elites.size() < max_elite) solucoes_elites.push_back(solucao_path);
            else{
                    int menor_dif = k + 12;
                    int posicao_menor_dif = -1;
                    for(int j = 0; j < solucoes_elites.size(); j++){
                            if(solucao_path.vectorBits.count() > solucoes_elites[j].vectorBits.count()){
                                    int dif = diferenca_simetrica(solucao_path, solucoes_elites[j]);
                                    if(menor_dif > dif){
                                            menor_dif = dif;
                                            posicao_menor_dif = j;
                                    }
                            }
                    }
                    if(posicao_menor_dif != -1){
                            solucoes_elites[posicao_menor_dif] = solucao_path;
                    }
            }

            if(solucao_path.vectorBits.count() > metrica){
                metrica = solucao_path.vectorBits.count();
                bitset_melhor_solucao = solucao_path.vectorBits;
                melhor_solucao = solucao_path.elem;
                if(metrica == tam_R) break;
            }

        }

        t++;
        if(t < tam_L) x = conjuntosPrincipalAux[t];

        conjuntos.clear();
    }
    conjuntosPrincipalAux.clear();

    Solucao solucao;
    solucao.vectorBits = bitset_melhor_solucao;
    solucao.elem = melhor_solucao;
    for(int i = 0; i < melhor_solucao.size(); i++){
        solucao.is_elem[melhor_solucao[i]] = true;
    }
    return solucao;
}
