#include "vns.h"

Lista* criarLista(){
    Lista* fila = (Lista*)malloc(sizeof(Lista));
    fila->inicio = NULL;
    fila->tamanho = 0;
    return fila;
}

void addLista(Lista* fila, bitset<nElem> elem){
    Item *novoItem = (Item*)malloc(sizeof(Item));
    novoItem->chave = elem;
    novoItem->proximo = NULL;

	novoItem->proximo = fila->inicio;
	fila->inicio = novoItem;

    fila->tamanho++;
}

bool testList(Lista* fila, bitset<nElem> elem, int k){
	if(fila->inicio == NULL) return true;

	Item *atual = fila->inicio;
	while(atual != NULL){
		bitset<nElem> bit_aux = Interseccao(elem, atual->chave);
		int qtd = bit_aux.count();
		if(qtd == k) return false;
		atual = atual->proximo;
	}
	return true;
}

void liberarLista(Lista* fila){
    Item *ultimo = fila->inicio;
    while(ultimo != NULL){
       Item *pt = ultimo;
       ultimo = ultimo->proximo;
       free(pt);
    }
    free(fila);
}

/**
Implementa o algoritmo de segunda ordem na fase de agitação do VNS.
Versão Grasp Reativo. Apenas na seleção dos elementos.
**/
Solucao SO_Reativo(Graph &graph, Solucao &solucao, int q, double alfa){
    Solucao solucao_linha(graph.tam_L);
    solucao_linha.vectorBits.set();

    int new_q = q;
    if(q >= graph.tam_L - graph.k) new_q = graph.tam_L - graph.k;

    if(new_q < graph.k){
        int indices_solucao[graph.k], size_indices = graph.k;
        for(int i = 0; i < graph.k; i++) indices_solucao[i] = i;

        bitset<nElem> ja_Selecionado;//Não contem os indices dos subconjuntos, mas suas posiçoes no vetor elem da solução
        ja_Selecionado.reset();
        int j = 0;
        while(j < new_q){
            int index_ss = randValor(1, size_indices) - 1;//Seleciona aleatoriamente um indice desconsiderando os que já foram selecionados. Não é o indice real dentro de solucao.elem.
            int ss = indices_solucao[index_ss];

            indices_solucao[index_ss] = indices_solucao[size_indices - 1];
            size_indices--;

            ja_Selecionado.set((size_t)ss);
            j++;
        }

        /*Aqui eu gero a interseção sem os subconjuntos em sair que irão ser removidos na troca.*/
        for (j = 0; j < solucao.elem.size(); j++){
            if(!ja_Selecionado.test((size_t)j)){
                solucao_linha.vectorBits = Interseccao(solucao_linha.vectorBits, graph.conjuntosPrincipal[solucao.elem[j]].vectorBits);
                solucao_linha.elem.push_back(solucao.elem[j]);
                solucao_linha.is_elem[solucao.elem[j]] = true;
            }
        }
    }
    if(q < graph.tam_L - graph.k){
        //Custo incremental representando o valor da interseção se o subconjunto j entrar na solução.
        int custo_incremental[graph.tam_L], c_min, c_max;

        int i = 1;
        do{
            int t = 0;
            for (int j = 0; j < graph.tam_L; j++){
                if(solucao_linha.is_elem[j] == false && solucao.is_elem[j] == false){
                    bitset<nElem> vetor_aux = Interseccao(solucao_linha.vectorBits, graph.conjuntosPrincipal[j].vectorBits);
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

            vector<int> lrc;//Lista Restrita de Candidatos contendo os subconjuntos que não estão na solução e que podem entrar.
            /*
            Inferior indica o menor valor de custo que um sunconjunto deve ter para entra na lista lrc, ou seja,
            somente subconjuntos com custo incremental maior ou igual a inferior entrarão na lista, e que não estejam na solução.
            */
            int inferior = c_min + alfa*(c_max - c_min);
            int tam_lrc = 0;//Quantidade de subconjuntos na lista lrc. Uso para selecionar um subconjunto da lista de forma aleatória.
            for (int j = 0; j < graph.tam_L; j++){
                //Crio a lista lrc.
                if(solucao_linha.is_elem[j] == false && solucao.is_elem[j] == false && custo_incremental[j] >= inferior){
                    lrc.push_back(j);
                    tam_lrc++;
                }
            }

            /*
            Seleciona de forma aleatória um subconjunto da lista lrc.
            Chamo o "randValor(0, tam_lrc - 1)" que escolhe um número de 0 a tamanho da lista lrc.
            */
            int s = lrc[randValor(0, tam_lrc - 1)];

            solucao_linha.is_elem[s] = true;//Indico que está na solução
            solucao_linha.elem.push_back(s);//Add no vetor
            solucao_linha.vectorBits = Interseccao(solucao_linha.vectorBits, graph.conjuntosPrincipal[s].vectorBits);//Gero a interseção da solução parcial.

            i++;
        }while(i <= q);
    }else{
        for (int j = 0; j < graph.tam_L; j++){
            if(solucao.is_elem[j] == false){
                solucao_linha.vectorBits = Interseccao(solucao_linha.vectorBits, graph.conjuntosPrincipal[j].vectorBits);
                solucao_linha.elem.push_back(j);
                solucao_linha.is_elem[j] = true;
            }
        }
    }
    return solucao_linha;
}

/**
Dado o valor de q_max e o valor de q corrente, ele retorna em qual quartil de q_max o valor de q estar, sendo que o q_max será dividido em 4 quartil.
**/
double get_erro_quartil(int q_max, int q){
    int parte = q_max/4;
	parte = (parte == 0 ? 1 : parte);

    if(q <= parte) return 0.2;
    else if(q <= 2*parte) return 0.3;
    else if(q <= 3*parte) return 0.4;
    else return 0.5;
}


Solucao VNS_Reativo(Graph &graph, Solucao &solucao_entrada){
    Grasp grasp;
    Solucao solucao = solucao_entrada;
    Solucao solucao_candidata = solucao_entrada;
    //Inicializando os valores iniciais do grasp reativo
    for(int i = 0; i < NUMBER_VALUES; i++){
        grasp.cont[i] = 0;
        grasp.score[i] = 0.0;
        grasp.prob_X[i] = 1.0/NUMBER_VALUES;
    }

    int q_step_min = 1;
    int q_max = graph.k, q_step = q_step_min, seqs = 500, q, qtd_sem_melhorar = 0;
    int q_step_max = (floor(0.2*graph.k) < q_step_min ? q_step_min : floor(0.2*graph.k));

	if(graph.k <= 3){
		q_max = 1;
		q_step = 1;
		q_step_max = 1;
	}else if(graph.k < 10){
		q_max = 0.5*graph.k + 1;
		q_step = 1;
		q_step_max = 1;
	}
	Lista* lista = criarLista();
	addLista(lista, solucao_candidata.vectorBits);
    int count_total = 0, qtd_igual = 0;
    for (int i = 0; i < seqs; i++) {
        q = 1;
        qtd_sem_melhorar = 0;
		qtd_igual = 0;
        while(q <= q_max){
            int indice_alfa = grasp.get_alfa();
            double alfa = grasp.cons_X[indice_alfa];
            Solucao solucao_linha = SO_Reativo(graph, solucao_candidata, q, alfa);
			solucao_linha = VND(graph, solucao_linha);

            if(solucao_linha.vectorBits.count() > solucao.vectorBits.count()){
                solucao = solucao_linha;
                solucao_candidata = solucao_linha;
                q = 1;
                qtd_sem_melhorar = 0;
                q_step = (q_step - 1 < q_step_min ? q_step_min : q_step - 1);
            }else{
                int beta = floor(graph.k*get_erro_quartil(q_max, q));
                if(solucao_linha.vectorBits.count() > solucao_candidata.vectorBits.count() || (solucao_linha.vectorBits.count() == solucao_candidata.vectorBits.count() && diferenca_simetrica(solucao_linha, solucao_candidata, graph.k) > beta)){
                    solucao_candidata = solucao_linha;
                }
                if(q_step < q_step_max){
                    qtd_sem_melhorar++;
                    if(qtd_sem_melhorar >= floor(0.5*q_max)){
                        q_step++;
                        qtd_sem_melhorar = 0;
                    }
                }
                q = (q < q_max && q + q_step > q_max ? q_max : q + q_step);
            }

            grasp.cont[indice_alfa]++;
            grasp.score[indice_alfa] += solucao_linha.vectorBits.count();

            if(count_total % NUMBER_VALUES == 0){
                grasp.atualizacao_probabilidade(solucao.vectorBits.count());
            }
            count_total++;
        }
    }
	liberarLista(lista);

    return solucao;
}
