#include "vnd.cpp"

Solucao N_q_VNS(const Solucao &solucao, int q){
   vector<int> sair;//Não contem os indices dos subconjuntos, mas suas posiçoes no vetor elem da solução
    int j = 1;
    while(j <= q){
        int s = get_next_set(sair, solucao.elem.size() - 1);
        sair.push_back(s);
        j++;
    }

    vector<int> candidatos_entrar;//Ira contem todos os subconjuntos que não pertecem a solução passada como parametros. Os indices.
    for (j = 0; j < solucao.is_elem.size(); j++) if(solucao.is_elem[j] == false) candidatos_entrar.push_back(j);

   vector<int> entrar;//Não contem os indices dos subconjuntos, mas suas posiçoes no vetor candidatos_entrar
    j = 1;
    while(j <= q){
        int s = get_next_set(entrar, candidatos_entrar.size() - 1);
        entrar.push_back(s);
        j++;
    }

    Solucao solucao_linha;
    solucao_linha.vectorBits.set();
    /*Aqui eu gero a interseção sem os subconjuntos em sair que irão ser removidos na troca.*/
    for (j = 0; j < solucao.elem.size(); j++){
        if(!contem(sair, j)){
            solucao_linha.vectorBits = Interseccao(solucao_linha.vectorBits, conjuntosPrincipal[solucao.elem[j]].vectorBits);
            solucao_linha.elem.push_back(solucao.elem[j]);
            solucao_linha.is_elem[solucao.elem[j]] = true;
        }
    }

    for (int j = 0; j < entrar.size(); j++){
        int ind_entrar = candidatos_entrar[entrar[j]];
        solucao_linha.vectorBits = Interseccao(solucao_linha.vectorBits, conjuntosPrincipal[ind_entrar].vectorBits);
        solucao_linha.elem.push_back(ind_entrar);
        solucao_linha.is_elem[ind_entrar] = true;
    }
    return solucao_linha;
}

/**
Implementa o algoritmo de segunda ordem na fase de agitação do VNS.
Versão Grasp.
**/
Solucao SO_Grasp(const Solucao &solucao, int q, double alfa){
   vector<int> sair;//Não contem os indices dos subconjuntos, mas suas posiçoes no vetor elem da solução
    int j = 1;
    while(j <= q){
        int s = get_next_set(sair, solucao.elem.size() - 1);
        sair.push_back(s);
        j++;
    }
    Solucao solucao_linha;
    solucao_linha.vectorBits.set();
    /*Aqui eu gero a interseção sem os subconjuntos em sair que irão ser removidos na troca.*/
    for (j = 0; j < solucao.elem.size(); j++){
        if(!contem(sair, j)){
            solucao_linha.vectorBits = Interseccao(solucao_linha.vectorBits, conjuntosPrincipal[solucao.elem[j]].vectorBits);
            solucao_linha.elem.push_back(solucao.elem[j]);
            solucao_linha.is_elem[solucao.elem[j]] = true;
        }
    }

    vector<bool> lrc;//Lista Restrita de Candidatos contendo os subconjuntos que não estão na solução e que podem entrar.
    for (int j = 0; j < tam_L; j++) lrc.push_back(false);

    //Custo incremental representando o valor da interseção se o subconjunto j entrar na solução.
    int custo_incremental[tam_L], c_min, c_max;

    int i = 1;
    do{
        int t = 0;
        for (int j = 0; j < tam_L; j++){
            if(solucao_linha.is_elem[j] == false){
                bitset<nElem> vetor_aux = Interseccao(solucao_linha.vectorBits, conjuntosPrincipal[j].vectorBits);
                custo_incremental[j] = vetor_aux.count();

                if(t == 0){
                    c_min = custo_incremental[j];
                    c_max = custo_incremental[j];
                }else{
                    if(c_min > custo_incremental[j]) c_min = custo_incremental[j];
                    if(c_max < custo_incremental[j]) c_max = custo_incremental[j];
                }
                t++;
            }
        }

        /*
        Inferior indica o menor valor de custo que um sunconjunto deve ter para entra na lista lrc, ou seja,
        somente subconjuntos com custo incremental maior ou igual a inferior entrarão na lista, e que não estejam na solução.
        */
        int inferior = c_min + alfa*(c_max - c_min);
        int tam_lrc = 0;//Quantidade de subconjuntos na lista lrc. Uso para selecionar um subconjunto da lista de forma aleatória.
        for (int j = 0; j < tam_L; j++){
            //Crio a lista lrc.
            if(solucao_linha.is_elem[j] == false && custo_incremental[j] >= inferior){
                lrc[j] = true;
                tam_lrc++;
            }else lrc[j] = false;
        }

        /*
        Seleciona de forma aleatória um subconjunto da lista lrc.
        Chamo o "randValor(0, tam_lrc - 1)" que escolhe um número de 0 a tamanho da lista lrc.
        Depois chamo a função "int get_subconjunto(vector<bool> vetor, int p)" que devolve o índice do subconjunto escolhido da lista lrc.
        */
        int s = get_subconjunto(lrc, randValor(0, tam_lrc - 1));

        solucao_linha.is_elem[s] = true;//Indico que está na solução
        solucao_linha.elem.push_back(s);//Add no vetor
        solucao_linha.vectorBits = Interseccao(solucao_linha.vectorBits, conjuntosPrincipal[s].vectorBits);//Gero a interseção da solução parcial.

        i++;
    }while(i <= q);
    return solucao_linha;
}

/**
Implementa a heurística 1 sendo que recebe uma solução parcial, com quantidade de subconjuntos menor do que k,
e gera uma solução viável usando a estratégia da heurística 1.
**/
void heuristica1_parcial(Solucao &solucao){
    vector<Elemento> conjuntos;
    bitset<nElem> vetor_solu_parcial;
    if(solucao.elem.size() > 0){
        for(int j = 0; j < conjuntosPrincipal.size(); j++){
                Elemento ele = conjuntosPrincipal[j];
                if(solucao.is_elem[ele.id] == false) conjuntos.push_back(ele);
        }
        vetor_solu_parcial = solucao.vectorBits;
    }else{
        conjuntos = conjuntosPrincipal;
        std::sort(conjuntos.begin(), conjuntos.end(), Elemento::LessThanByAge());
        Elemento e = conjuntos[0];
        solucao.addSubConjunto(e.id);
        vetor_solu_parcial = e.vectorBits;
    }

    int kkk = solucao.elem.size();
    int kk = 0;
    while(kk < k - kkk){
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
}

/**
Algoritmo de segunda ordem com a mesma estratégia de
SOUZA, M. C., AND MARTINS, P. Skewed vns enclosing second order algorithm for the degree constrained minimum spanning tree problem.
**/
Solucao SO_RD(const Solucao &solucao, int q){
   vector<int> sair;//Não contem os indices dos subconjuntos, mas suas posiçoes no vetor elem da solução
    int j = 1;
    /*Removemos q subconjuntos de forma aleatória.*/
    while(j <= q){
        int s = get_next_set(sair, solucao.elem.size() - 1);
        sair.push_back(s);
        j++;
    }
    Solucao solucao_linha;
    solucao_linha.vectorBits.set();
    /*Aqui eu gero a interseção sem os subconjuntos em sair que irão ser removidos na troca.*/
    for (j = 0; j < solucao.elem.size(); j++){
        if(!contem(sair, j)){
            solucao_linha.vectorBits = Interseccao(solucao_linha.vectorBits, conjuntosPrincipal[solucao.elem[j]].vectorBits);
            solucao_linha.elem.push_back(solucao.elem[j]);
            solucao_linha.is_elem[solucao.elem[j]] = true;
        }
    }

    vector<int> candidatos_entrar;//Ira contem todos os subconjuntos que não pertecem a solução passada como parametros. Os indices.
    for (j = 0; j < solucao_linha.is_elem.size(); j++) if(solucao_linha.is_elem[j] == false) candidatos_entrar.push_back(j);

   vector<int> entrar;//Não contem os indices dos subconjuntos, mas suas posiçoes no vetor candidatos_entrar

    int t = 0;
    while(t < q){
        entrar.clear();//Necessário a partir da segunda iteração.
        j = 1;
        /*Selecionamos q subconjuntos canditados a entrar na solução de forma aleatória.*/
        while(j <= q - t){
            int s = get_next_set(entrar, candidatos_entrar.size() - 1);
            entrar.push_back(s);
            j++;
        }

        //printf("ok1: %d %d\n", entrar.size(), q - t);

        int melhor_indice, indice_candidatos_entrar;
        bitset<nElem> melhor_bitset;
        melhor_bitset.reset();
        for(int i = 0; i < entrar.size(); i++){
            int indice_sub = candidatos_entrar[entrar[i]];
            Solucao solucao_sub = solucao_linha;
            //Add indice_sub no problema restrito
            solucao_sub.vectorBits = Interseccao(conjuntosPrincipal[indice_sub].vectorBits, solucao_sub.vectorBits);
            solucao_sub.is_elem[indice_sub] = true;
            solucao_sub.elem.push_back(indice_sub);
            heuristica1_parcial(solucao_sub);

            //printf("ok2: %d %d\n", solucao_sub.vectorBits.count(), melhor_bitset.count());
            if(solucao_sub.vectorBits.count() >= melhor_bitset.count()){
                //printf("entrou \n");
                melhor_bitset = solucao_sub.vectorBits;
                melhor_indice = indice_sub;
                indice_candidatos_entrar = entrar[i];
            }
        }
        //printf("ok3: %d %d %d \n", melhor_indice, indice_candidatos_entrar, melhor_bitset.count());
        solucao_linha.vectorBits = melhor_bitset;
        solucao_linha.is_elem[melhor_indice] = true;
        solucao_linha.elem.push_back(melhor_indice);

        candidatos_entrar[indice_candidatos_entrar] = candidatos_entrar[candidatos_entrar.size() - 1];
        candidatos_entrar.pop_back();

        t++;
    }
    return solucao_linha;
}

/**
Dado o valor de q_max e o valor de q corrente, ele retorna em qual quartil de q_max o valor de q estar, sendo que o q_max será dividido em 4 quartil.
**/
double get_erro_quartil(int q_max, int q){
    int parte = q_max/4;

    if(q <= parte) return 0.2;
    else if(q <= 2*parte) return 0.3;
    else if(q <= 3*parte) return 0.4;
    else return 0.5;
}

/**
VNS com critério de aceitação permitido solucao candidata pior mais com diferença simetrica maior.
**/
Solucao VNS_Aceitacao(const Solucao &solucao_entrada){
    Solucao solucao = solucao_entrada;
    Solucao solucao_candidata = solucao_entrada;
    int q_max = k - 1, q_step = 1, seqs = 500, q;

    if(k >= tam_L/2) q_max = tam_L - k - 4;
    else if(k < tam_L/2 && k + 4 >= tam_L/2) q_max = k - 4;

    double beta_erro = 0.8;

    for (int i = 0; i <= seqs; i++) {
        q = 1;
        while(q <= q_max){
            Solucao solucao_linha = N_q_VNS(solucao_candidata, q);
            //Solucao solucao_linha = SO_RD(solucao_candidata, q);
            VND(solucao_linha);

            if(solucao_linha.vectorBits.count() > solucao.vectorBits.count()){
                solucao = solucao_linha;
                solucao_candidata = solucao_linha;
                q = 1;
            }else{
                int beta = k*get_erro_quartil(q_max, q);
                if(solucao_linha.vectorBits.count() > solucao_candidata.vectorBits.count() ||
                   (solucao_linha.vectorBits.count() == solucao_candidata.vectorBits.count() && diferenca_simetrica(solucao_linha, solucao_candidata) > beta)){
                    solucao_candidata = solucao_linha;
                    q = 1;
                }else
                q += q_step;
            }
        }
    }

    return solucao;
}

Solucao VNS_SO_Grasp_Aceitacao(const Solucao &solucao_entrada){
    Solucao solucao = solucao_entrada;
    Solucao solucao_candidata = solucao_entrada;
    //Inicializando os valores iniciais do grasp reativo
    for(int i = 0; i < NUMBER_VALUES; i++){
        cont[i] = 0;
        score[i] = 0.0;
        prob_X[i] = 1.0/NUMBER_VALUES;
    }

    int q_max = k - 1, q_step = 1, seqs = 500, q;

    if(k >= tam_L/2) q_max = tam_L - k - 4;
    else if(k < tam_L/2 && k + 4 >= tam_L/2) q_max = k - 4;

    int t = 0;
    for (int i = 0; i < seqs; i++) {
        q = 1;
        int qtd_sem_melhorar = 0;
        while(q <= q_max){
            int indice_alfa = get_alfa();
            double alfa = cons_X[indice_alfa];
            Solucao solucao_linha = SO_Grasp(solucao_candidata, q, alfa);
            solucao_linha = VND(solucao_linha);

            if(solucao_linha.vectorBits.count() > solucao.vectorBits.count()){
                solucao = solucao_linha;
                solucao_candidata = solucao_linha;
                q = 1;
            }else{
                int beta = k*get_erro_quartil(q_max, q);
                if(solucao_linha.vectorBits.count() > solucao_candidata.vectorBits.count() ||
                   (solucao_linha.vectorBits.count() == solucao_candidata.vectorBits.count() &&
                    diferenca_simetrica(solucao_linha, solucao_candidata) > beta)){
                    solucao_candidata = solucao_linha;
                }
                qtd_sem_melhorar++;
                if(qtd_sem_melhorar >= 0.4*q_max){
                    q_step++;
                    qtd_sem_melhorar = 0;
                }
                q += q_step;
            }

            cont[indice_alfa]++;
            score[indice_alfa] += solucao_linha.vectorBits.count();

            if(t % NUMBER_VALUES == 0){
                atualizacao_probabilidade(solucao.vectorBits.count());
            }
            t++;
        }
    }

    return solucao;
}

/**
Algoritmo VNS utilizando o SO_Grasp na fase de agitação.
**/
Solucao VNS_SO_Grasp(const Solucao &solucao_entrada){
    Solucao solucao = solucao_entrada;
    //Inicializando os valores iniciais do grasp reativo
    for(int i = 0; i < NUMBER_VALUES; i++){
        cont[i] = 0;
        score[i] = 0.0;
        prob_X[i] = 1.0/NUMBER_VALUES;
    }

    int q_max = k - 1, q_step = 1, seqs = 500, q;

    if(k >= tam_L/2) q_max = tam_L - k - 4;
    else if(k < tam_L/2 && k + 4 >= tam_L/2) q_max = k - 4;

    int t = 0;
    for (int i = 0; i < seqs; i++) {
        q = 1;
        int qtd_sem_melhorar = 0;
        while(q <= q_max){
            int indice_alfa = get_alfa();
            double alfa = cons_X[indice_alfa];
            Solucao solucao_linha = SO_Grasp(solucao, q, alfa);
            solucao_linha = VND(solucao_linha);

            if(solucao_linha.vectorBits.count() > solucao.vectorBits.count()){
                solucao = solucao_linha;
                q = 1;
            }else{
                qtd_sem_melhorar++;
                if(qtd_sem_melhorar >= 0.4*q_max){
                    q_step++;
                    qtd_sem_melhorar = 0;
                }
                q += q_step;
            }

            cont[indice_alfa]++;
            score[indice_alfa] += solucao_linha.vectorBits.count();

            if(t % NUMBER_VALUES == 0){
                atualizacao_probabilidade(solucao.vectorBits.count());
            }
            t++;
        }
    }
    return solucao;
}

/**
VNS Tradicional com atualização
**/
Solucao VNS_Atualizacao(const Solucao &solucao_entrada){
    Solucao solucao = solucao_entrada;
    int q_max = k - 1, q_step = 1, seqs = 500, q;

    if(k >= tam_L/2) q_max = tam_L - k - 4;
    else if(k < tam_L/2 && k + 4 >= tam_L/2) q_max = k - 4;

    for (int i = 0; i <= seqs; i++) {
        q = 1;
        int qtd_sem_melhorar = 0;
        while(q <= q_max){
            Solucao solucao_linha = N_q_VNS(solucao, q);
            //Solucao solucao_linha = SO_RD(solucao, q);
            solucao_linha = VND(solucao_linha);

            if(solucao_linha.vectorBits.count() > solucao.vectorBits.count()){
                solucao = solucao_linha;
                q = 1;
            }else{
                qtd_sem_melhorar++;
                if(qtd_sem_melhorar >= 0.5*q_max){
                    q_step++;
                    qtd_sem_melhorar = 0;
                }
                q += q_step;
            }

        }
    }
    return solucao;
}

/**
VNS Tradicional
**/
Solucao VNS(const Solucao &solucao_entrada){
    Solucao solucao = solucao_entrada;
    int q_max = k - 1, q_step = 1, seqs = 500, q;

    if(k >= tam_L/2) q_max = tam_L - k - 4;
    else if(k < tam_L/2 && k + 4 >= tam_L/2) q_max = k - 4;

    for (int i = 0; i <= seqs; i++) {
        q = 1;
        while(q <= q_max){
            Solucao solucao_linha = N_q_VNS(solucao, q);
            //Solucao solucao_linha = SO_RD(solucao_linha, q);
            solucao_linha = VND(solucao_linha);

            if(solucao_linha.vectorBits.count() > solucao.vectorBits.count()){
                solucao = solucao_linha;
                q = 1;
            }else  q += q_step;
        }
    }
    return solucao;
}
