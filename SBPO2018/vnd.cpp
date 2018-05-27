
/**
Verificar se um vetor de inteiros contem um determinado valor p.
**/
int contem(const vector<int> &vetor, int p){
    for (int j = 0; j < vetor.size(); j++){
        if(vetor[j] == p)
            return 1;
    }
    return 0;
}

int get_next_set(vector<int> ja_escolhidos, int maximo){
        int s = randValor(0, (maximo - ja_escolhidos.size()));
        int j = 0;
        for(int i = 0; i <= maximo; i++){
            if(!contem(ja_escolhidos, i)) j++;

            if(j == s){
                s = i;
                break;
            }
        }
        return s;
}

/**
Implementa a 3_vizinhanca.
A seleção dos 3 subconjuntos a serem removidos é feita de forma aleatória.

Para selecionar os 3 subconjuntos que irão entrar na solução, essa implementação verificar cada tripla de subconjuntos
que não pertence a solução passada como parâmetro, verificando se essa troca for efetivada, o valor da solução melhora, caso melhore,
devolve essa solução e finaliza.
**/
void vizinhanca_3(Solucao &solucao){
    int i1, i2, i3; //Não contém os indices dos subconjuntos que irão sair, mas suas posições no vetor elem da solução.
    vector<int> sair;//Não contem os indices dos subconjuntos, mas suas posiçoes no vetor elem da solução
    int j = 1;
    while(j <= 3){
        int s = get_next_set(sair, solucao.elem.size() - 1);
        sair.push_back(s);
        j++;
    }
    i1 = sair[0];
    i2 = sair[1];
    i3 = sair[2];

    bitset<nElem> bit_solucao_parcial;
    bit_solucao_parcial.set();
    /*Aqui eu gero a interseção sem os subconjuntos em sair que irão ser removidos na troca.*/
    for (int q = 0; q < solucao.elem.size(); q++){
        if(q != i1 && q != i2 && q != i3){
            bit_solucao_parcial = Interseccao(bit_solucao_parcial, conjuntosPrincipal[solucao.elem[q]].vectorBits);
        }
    }

    for (int j1 = 0; j1 < tam_L - 2; j1++){
         int indice_sub1 = conjuntosPrincipalOrdenado[j1].id;

        if(solucao.is_elem[indice_sub1] == false){
            bitset<nElem> bit_aux1 = bit_solucao_parcial;
            bit_aux1 = Interseccao(bit_aux1, conjuntosPrincipal[indice_sub1].vectorBits);

            for (int j2 = j1 + 1; j2 < tam_L - 1; j2++){
                int indice_sub2 = conjuntosPrincipalOrdenado[j2].id;

                if(solucao.is_elem[indice_sub2] == false){
                    bitset<nElem> bit_aux2 = bit_aux1;
                    bit_aux2 = Interseccao(bit_aux2, conjuntosPrincipal[indice_sub2].vectorBits);

                    for (int j3 = j2 + 1; j3 < tam_L; j3++){
                        int indice_sub3 = conjuntosPrincipalOrdenado[j3].id;

                        if(solucao.is_elem[indice_sub3] == false){
                            bitset<nElem> bit_aux3 = bit_aux2;
                            bit_aux3 = Interseccao(bit_aux3, conjuntosPrincipal[indice_sub3].vectorBits);

                            if(bit_aux3.count() > solucao.vectorBits.count()){//Comparo se com a troca houve melhoria na solução.
                                solucao.vectorBits = bit_aux3;

                                solucao.is_elem[solucao.elem[i1]] = false;
                                solucao.is_elem[solucao.elem[i2]] = false;
                                solucao.is_elem[solucao.elem[i3]] = false;

                                solucao.is_elem[indice_sub1] = true;
                                solucao.is_elem[indice_sub2] = true;
                                solucao.is_elem[indice_sub3] = true;

                                solucao.elem[i1] = indice_sub1;
                                solucao.elem[i2] = indice_sub2;
                                solucao.elem[i3] = indice_sub3;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
Implementa a 2_vizinhanca.
A seleção dos 2 subconjuntos a serem removidos é feita de forma aleatória.

Para selecionar os 2 subconjuntos que irão entrar na solução, essa implementação verificar cada par de subconjuntos
que não pertence a solução passada como parâmetro, verificando se essa troca for efetivada, o valor da solução melhora, caso melhore,
devolve essa solução e finaliza.
**/
void vizinhanca_2(Solucao &solucao){
    int i1, i2; //Não contém os indices dos subconjuntos que irão sair, mas suas posições no vetor elem da solução.
    vector<int> sair;//Não contem os indices dos subconjuntos, mas suas posiçoes no vetor elem da solução
    int j = 1;
    while(j <= 2){
        int s = get_next_set(sair, solucao.elem.size() - 1);
        sair.push_back(s);
        j++;
    }
    i1 = sair[0];
    i2 = sair[1];

    bitset<nElem> bit_solucao_parcial;
    bit_solucao_parcial.set();
    /*Aqui eu gero a interseção sem os subconjuntos em sair que irão ser removidos na troca.*/
    for (int q = 0; q < solucao.elem.size(); q++){
        if(q != i1 && q != i2){
            bit_solucao_parcial = Interseccao(bit_solucao_parcial, conjuntosPrincipal[solucao.elem[q]].vectorBits);
        }
    }

    for (int j1 = 0; j1 < tam_L - 1; j1++){
        int indice_sub1 = conjuntosPrincipalOrdenado[j1].id;

        if(solucao.is_elem[indice_sub1] == false){
            bitset<nElem> bit_aux1 = bit_solucao_parcial;
            bit_aux1 = Interseccao(bit_aux1, conjuntosPrincipal[indice_sub1].vectorBits);

            for (int j2 = j1 + 1; j2 < tam_L; j2++){
                int indice_sub2 = conjuntosPrincipalOrdenado[j2].id;
                if(solucao.is_elem[indice_sub2] == false){

                    bitset<nElem> bit_aux2 = bit_aux1;
                    bit_aux2 = Interseccao(bit_aux2, conjuntosPrincipal[indice_sub2].vectorBits);

                    if(bit_aux2.count() > solucao.vectorBits.count()){//Comparo se com a troca houve melhoria na solução.
                        solucao.vectorBits = bit_aux2;

                        solucao.is_elem[solucao.elem[i1]] = false;
                        solucao.is_elem[solucao.elem[i2]] = false;
                        solucao.is_elem[indice_sub1] = true;
                        solucao.is_elem[indice_sub2] = true;

                        solucao.elem[i1] = indice_sub1;
                        solucao.elem[i2] = indice_sub2;
                        return;
                    }
                }
            }
        }
    }
}

/**
Implementa a 1_vizinhanca.
A seleção do subconjunto a ser removido é feita de forma aleatória.

Para selecionar o subconjunto que irá entrar na solução, essa implementação verificar cada subconjunto
que não pertence a solução passada como parâmetro, verificando se essa troca for efetivada, o valor da solução melhora, caso melhore,
devolve essa solução e finaliza.
**/
void vizinhanca_1(Solucao &solucao){
    int i1; //Não contém os indices dos subconjuntos que irão sair, mas suas posições no vetor elem da solução.
    i1 = randValor(0, solucao.elem.size() - 1);

    bitset<nElem> bit_solucao_parcial;
    bit_solucao_parcial.set();
    /*Aqui eu gero a interseção sem os subconjuntos em sair que irão ser removidos na troca.*/
    for (int q = 0; q < solucao.elem.size(); q++){
        if(q != i1){
            bit_solucao_parcial = Interseccao(bit_solucao_parcial, conjuntosPrincipal[solucao.elem[q]].vectorBits);
        }
    }

    for (int j1 = 0; j1 < tam_L; j1++){
        int indice_sub = conjuntosPrincipalOrdenado[j1].id;

        if(solucao.is_elem[indice_sub] == false){
            bitset<nElem> bit_aux1 = bit_solucao_parcial;
            bit_aux1 = Interseccao(bit_aux1, conjuntosPrincipal[indice_sub].vectorBits);

            if(bit_aux1.count() > solucao.vectorBits.count()){//Comparo se com a troca houve melhoria na solução.
                solucao.vectorBits = bit_aux1;

                solucao.is_elem[solucao.elem[i1]] = false;
                solucao.is_elem[indice_sub] = true;

                solucao.elem[i1] = indice_sub;
                return;
            }
        }
    }
}

/**
Seleciona aleatoriamente um subconjunto para sair da solução e percorre os subconjuntos que não
pertencem a solução comparando se ele entrar na solução ocorre uma melhoria da solução.
Caso ocorra, a troca é efetuada.

Esse processo se repete t vezes.
**/
void busca_local_t(Solucao &solucao, int t){
    vector<int> sair;
    int j = 0;
    while(j < solucao.elem.size() && t > 0){
        int ss = get_next_set(sair, solucao.elem.size() - 1);
        sair.push_back(ss);
        int s = solucao.elem[ss];//Subconjunto da solução que irá ser removido na troca.


        bitset<nElem> bit_aux;
        bit_aux.set();
        /*Aqui eu gero a interseção sem o subconjunto s que irá ser removido na troca.*/
        for (int q = 0; q < solucao.elem.size(); q++){
            if(q != ss)
                bit_aux = Interseccao(bit_aux, conjuntosPrincipal[solucao.elem[q]].vectorBits);
        }

        int s_linha;//Subconjunto que ira entrar na troca.
        for (int jj = 0; jj < tam_L; jj++){
            int indice_sub = conjuntosPrincipalOrdenado[jj].id;

            if(solucao.is_elem[indice_sub] == false){
                s_linha = indice_sub;

                //Realizo a troca.
                bitset<nElem> bit_aux2 = bit_aux;
                bit_aux2 = Interseccao(bit_aux2, conjuntosPrincipal[s_linha].vectorBits);

                if(bit_aux2.count() > solucao.vectorBits.count()){//Comparo se com a troca houve melhoria na solução.
                    solucao.vectorBits = bit_aux2;

                    solucao.is_elem[s] = false;
                    solucao.is_elem[s_linha] = true;

                    solucao.elem[ss] = s_linha;
                }
            }
        }
        t--;
        j++;
    }
}

/**
Outra implementação da t_vizinhança.
Remove os t subconjuntos de forma aleatória.

Seleciona dos subconjuntos que não pertecem a solução parcial (inclusive os t subconjuntos removidos), aquele que possui a maior interseção
com a solução parcial. Adiciona na solução.
Repete esse processo t vezes.

Se a nova solução viável é melhor que a solução passada como parâmetro, atualiza.
**/
void vizinhanca_t(Solucao &solucao, int t){
   vector<int> sair;//Não contem os indices dos subconjuntos, mas suas posiçoes no vetor elem da solução
    int j = 1;
    while(j <= t){
        int s;
        do{
            s = randValor(0, solucao.elem.size() - 1);
        }while(contem(sair, s));

        sair.push_back(s);
        j++;

        solucao.is_elem[solucao.elem[s]] = false;
    }

    bitset<nElem> bit_solucao_parcial;
    bit_solucao_parcial.set();
    /*Aqui eu gero a interseção sem os subconjuntos em sair que irão ser removidos na troca.*/
    for (int q = 0; q < solucao.elem.size(); q++){
        if(!contem(sair, q)){
            bit_solucao_parcial = Interseccao(bit_solucao_parcial, conjuntosPrincipal[solucao.elem[q]].vectorBits);
        }
    }

    vector<int> entrar;
    for (int i = 0; i < t; i++){
        int val_indice = -1, indice = -1;
        for (int j1 = 0; j1 < tam_L; j1++){
            int indice_sub = conjuntosPrincipalOrdenado[j1].id;

            if(solucao.is_elem[indice_sub] == false && !contem(entrar, indice_sub)){
                bitset<nElem> bit_aux1 = Interseccao(bit_solucao_parcial, conjuntosPrincipal[indice_sub].vectorBits);

                int valor_int = bit_aux1.count();
                if(valor_int > val_indice){
                    val_indice = valor_int;
                    indice = indice_sub;
                }
            }
        }
        bit_solucao_parcial = Interseccao(bit_solucao_parcial, conjuntosPrincipal[indice].vectorBits);
        entrar.push_back(indice);
    }
    //printf("%d \n", bit_solucao_parcial.count());
    if(bit_solucao_parcial.count() > solucao.vectorBits.count()){
        solucao.vectorBits = bit_solucao_parcial;
        for (int i = 0; i < t; i++){
            solucao.elem[sair[i]] = entrar[i];
            solucao.is_elem[entrar[i]] = true;
        }
    }else{
        for (int i = 0; i < t; i++) solucao.is_elem[solucao.elem[sair[i]]] = true;
    }
}

Solucao VND(const Solucao &solucao_entrada){
    Solucao solucao_linha = solucao_entrada;
    Solucao melhor_solucao = solucao_entrada;

    int t = 1;
    int profundidade_busca_local = 3;

    while(t <= profundidade_busca_local){//Linha usado apenas quando a implementação d t_vizinhanca for a função "vizinhanca_t"
        //vizinhanca_t(solucao_linha, t);
        busca_local_t(solucao_linha, t);
        /*if(t == 1) vizinhanca_1(solucao_linha);
        else if(t == 2) vizinhanca_2(solucao_linha);
        else vizinhanca_3(solucao_linha);*/

        if(melhor_solucao.vectorBits.count() < solucao_linha.vectorBits.count()){
            melhor_solucao = solucao_linha;
            t = 1;
        }else t++;
    }

    return melhor_solucao;
}
