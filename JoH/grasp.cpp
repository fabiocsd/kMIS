#include "grasp.h"


Grasp::Grasp(Graph &graph){
    this->graph = graph;
}

Grasp::Grasp(){}

/**Função que controe uma solução viável para o grasp.**/
Solucao Grasp::construcao(double alfa){
    Solucao solucao(graph.tam_L);//Solução sendo construida.

    vector<bool> lrc;//Lista Restrita de Candidatos contendo os subconjuntos que não estão na solução e que podem entrar.
    //Custo incremental representando o valor da interseção se o subconjunto j entrar na solução.
    int custo_incremental[graph.tam_L], c_min, c_max;

    for (int j = 0; j < graph.tam_L; j++){
        solucao.is_elem.push_back(false);
        lrc.push_back(false);

        custo_incremental[j] = graph.conjuntosPrincipal[j].vectorBits.count();//Inicialmente o custo incremental é igual a quantidade de elementos do subconjunto. Supondo que a solução tenha apenas esse subconjunto.
        if(j == 0){
            c_min = custo_incremental[j];
            c_max = custo_incremental[j];
        }else{
            if(c_min > custo_incremental[j]) c_min = custo_incremental[j];
            if(c_max < custo_incremental[j]) c_max = custo_incremental[j];
        }
    }
    solucao.vectorBits.set();

    int i = 0;
    while(i < graph.k){
        /*
        Inferior indica o menor valor de custo que um sunconjunto deve ter para entra na lista lrc, ou seja,
        somente subconjuntos com custo incremental maior ou igual a inferior entrarão na lista, e que não estejam na solução.
        */
        int inferior = c_min + alfa*(c_max - c_min);
        int tam_lrc = 0;//Quantidade de subconjuntos na lista lrc. Uso para selecionar um subconjunto da lista de forma aleatória.
        for (int j = 0; j < graph.tam_L; j++){
            //Crio a lista lrc.
            if(solucao.is_elem[j] == false && custo_incremental[j] >= inferior){
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

        #ifdef DEBUG
            cout << "subconjunto escolhido = " << s << endl;
            cout << "custo incremental     = " << custo_incremental[s] << endl;
        #endif

        solucao.is_elem[s] = true;//Indico que está na solução
        solucao.elem.push_back(s);//Add no vetor
        solucao.vectorBits = Interseccao(solucao.vectorBits, graph.conjuntosPrincipal[s].vectorBits);//Gero a interseção da solução parcial.

        if(i + 1 == graph.k) break;

        //Atualizo os custos incrementais.
        int t = 0;
        for (int j = 0; j < graph.tam_L; j++){
            if(solucao.is_elem[j] == false){
                bitset<nElem> vetor_aux = Interseccao(solucao.vectorBits, graph.conjuntosPrincipal[j].vectorBits);
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

        i++;
    }
    return solucao;
}
/**
Realiza a busca local. Recebe uma solução viável e busca uma troca que melhore a solução.
A troca um subconjunto da solução por outro não pertecente a solução.
Devolve a primeira solução que melhora.
**/
void Grasp::busca_local(Solucao &solucao){
    Solucao solucao_linha(graph.tam_L);//Solução após a troca
    for (int j = 0; j < solucao.elem.size(); j++){
        int s = solucao.elem[j];//Subconjunto da solução que irá ser removido na troca.
        int s_linha;//Subconjunto que ira entrar na troca.
        for (int jj = 0; jj < graph.tam_L; jj++){
            if(solucao.is_elem[jj] == false){
                s_linha = jj;

                solucao_linha.limpar();//Zera os dados da solução.

                /*Aqui eu gero a interseção sem o subconjunto s que irá ser removido na troca.*/
                for (int q = 0; q < solucao.elem.size(); q++){
                    if(q != j){
                        solucao_linha.vectorBits = Interseccao(solucao_linha.vectorBits, graph.conjuntosPrincipal[solucao.elem[q]].vectorBits);
                        solucao_linha.elem.push_back(solucao.elem[q]);
                        solucao_linha.is_elem[solucao.elem[q]] = true;
                    }
                }
                //Realizo a troca.
                solucao_linha.vectorBits = Interseccao(solucao_linha.vectorBits, graph.conjuntosPrincipal[s_linha].vectorBits);
                solucao_linha.elem.push_back(s_linha);
                solucao_linha.is_elem[s_linha] = true;

                if(solucao_linha.vectorBits.count() > solucao.vectorBits.count()){//Comparo se com a troca houve melhoria na solução.
                    solucao.vectorBits = solucao_linha.vectorBits;
                    solucao.elem = solucao_linha.elem;
                    solucao.is_elem = solucao_linha.is_elem;
                    return;
                }
            }
        }
    }
}



/**
Path Relinking ele recebe duas soluções, origem e destino, e realiza trocas que transforma a solução origem na destino,
sendo que a cada troca verificar se melhorou o valor da solução.
**/
Solucao Grasp::path_relinking(Solucao &solucao){
    Solucao origem = solucao;//A solução passado como parâmetro será a solução origem.

    //Seleciono a solução destino de forma aleatória.
    int s = randValor(0, solucoes_elites.size() - 1);
    Solucao destino = solucoes_elites[s], melhor_solucao(graph.tam_L);

    //Melhor solução inicial será entre a solução origem e destino.
    int valor_melhor_solucao;
    if(origem.vectorBits.count() > destino.vectorBits.count()){
        melhor_solucao = origem;
        valor_melhor_solucao = origem.vectorBits.count();
    }
    else{
        melhor_solucao = destino;
        valor_melhor_solucao = destino.vectorBits.count();

        destino = origem;
        origem = melhor_solucao;
    }

    int difsi = diferenca_simetrica(origem, destino, graph.k);
    while(difsi > 0){//Enquanto a solução origem não foi transformada na solução destino, continue.
        /*Bitset com a Interseção dos subconjuntos que estão ao mesmo tempo da origem e destino. Ou caras que irão permanecer na origem*/
        bitset<nElem> inter_origem_destino;

        inter_origem_destino.set();
        vector<int> only_origem, only_destino;//Subconjuntos que precisam sair/entrar na origem, respectivamente.

        //Gero as lista que utilizo.
        for(int j = 0; j < destino.elem.size(); j++){
            int q = destino.elem[j];
            if(origem.is_elem[q] == false) only_destino.push_back(q);
        }
        for(int j = 0; j < origem.elem.size(); j++){
            int q = origem.elem[j];

            if(destino.is_elem[q] == false) only_origem.push_back(q);
            else inter_origem_destino = Interseccao(inter_origem_destino, graph.conjuntosPrincipal[q].vectorBits);//Interseção com os subconjuntos da origem que tbm estão no destino.
        }

        int melhor_movimento = -1, origem_sair, origem_entrar;//Valor do melhor movimento, subconjunto de vai sair e entrar na origem para o melhor movimento.
        bitset<nElem> bitset_movimento;
        for(int i = 0; i < only_origem.size(); i++){
            int p = only_origem[i];//Elemento que irá sair da origem.

            //Aqui gero a interseção com todos os elementos da origem sem o subconjunto p.
            bitset<nElem> vetor_aux = inter_origem_destino;
            for(int ii = 0; ii < only_origem.size(); ii++){
                if(p != only_origem[ii]){
                    vetor_aux = Interseccao(vetor_aux, graph.conjuntosPrincipal[ only_origem[ii] ].vectorBits);
                }
            }
            //Para cada subconjunto que está na solução destino e não na origem, calculo o custo do movimento.
            for(int j = 0; j < only_destino.size(); j++){
                int q = only_destino[j];

                bitset<nElem> movimento = Interseccao(vetor_aux, graph.conjuntosPrincipal[ q ].vectorBits);
                int aux = movimento.count();
                if(melhor_movimento < aux){
                    melhor_movimento = movimento.count();
                    origem_sair = p;
                    origem_entrar = q;
                    bitset_movimento = movimento;
                }
            }
        }
        //Efetivo o melhor movimento.
        origem.vectorBits = bitset_movimento;
        origem.is_elem[origem_entrar] = true;
        origem.is_elem[origem_sair] = false;
        for(int i = 0; i < origem.elem.size(); i++){
            if(origem.elem[i] == origem_sair){
                origem.elem[i] = origem_entrar;
                break;
            }
        }

        if(origem.vectorBits.count() > valor_melhor_solucao){//Melhorou?
            melhor_solucao = origem;
            valor_melhor_solucao = origem.vectorBits.count();
        }
        difsi = diferenca_simetrica(origem, destino, graph.k);

        //printf("difsi: %d \n", difsi);
    }
    return melhor_solucao;
}

int Grasp::get_alfa(){
    double prob_acumulativa[11];
    prob_acumulativa[0] = prob_X[0];
    for(int w = 1; w < 11; w++) prob_acumulativa[w] = prob_acumulativa[w - 1] + prob_X[w];

    double x;
    #ifdef __linux__
        x = drand48();//Linux
    #elif _WIN32
        x = (double)rand()/RAND_MAX;//Windons
    #else
		#error "OS not supported!"
    #endif

    for(int i = 0; i < 11; i++) if(x <= prob_acumulativa[i]) return i;
}


void Grasp::atualizacao_probabilidade(int Z_max){
	double Q[NUMBER_VALUES];
	double sigma = 0.0;

	#ifdef DEBUG
        cout << "ATUALIZACAO VETOR DE PROBABILIDADES " << endl;
	#endif


	bool falha = false;

	for(int k = 0; k < NUMBER_VALUES; k++){
		if(cont[k] == 0) {
			#ifdef DEBUG
                printf("FALHOU\n");
			#endif
			falha = true;
			break;
		}
		avg[k] = score[k]/cont[k];
		//Q[k] = melhor_solucao.vectorBits.count()/avg[k];
		Q[k] = avg[k]/Z_max;

		sigma += Q[k];

		#ifdef DEBUG
            printf("valor %d media %lf q %lf\n", k, avg[k], Q[k]);
		#endif

	}
	if(!falha){
        #ifdef DEBUG
            printf("PROCESSO BEM SUCEDIDO\n");
        #endif
        for(int k = 0; k < NUMBER_VALUES; k++){
            prob_X[k] = Q[k]/sigma;
            #ifdef DEBUG
                printf("prob[%d] = %lf\n", k, Q[k]/sigma);
            #endif

        }
	}else{
		#ifdef DEBUG
            printf("PROCESSO FALHOU\n");
		#endif
	}
}

Solucao Grasp::grasp(){
    #ifdef DEBUG
        cout << "-----------GRASP------" << endl;
    #endif

    solucoes_elites.clear();

    int custo_melhor_solucao = 0;

    Solucao melhor_solucao(graph.tam_L);

    melhor_solucao_construcao = -1;

    bool apicar_buscar_path = true;

    //Inicializando os valores iniciais do grasp reativo
    for(int i = 0; i < NUMBER_VALUES; i++){
			cont[i] = 0;
			score[i] = 0.0;
			prob_X[i] = 1.0/NUMBER_VALUES;
    }

    for(int i = 0; i < qtd_max_iteracoes; i++){

        #ifdef DEBUG
			cout << "Iteracao: " << i << endl;
        #endif

        int indice_alfa = get_alfa();
        double alfa = cons_X[indice_alfa];

        #ifdef DEBUG
            cout << "alpha = " << alfa << endl;
            cout << "FASE CONSTRUCAO " << endl;
        #endif


        Solucao solucao = construcao(alfa);


        #ifdef DEBUG
            cout << "Solucao size construcao : "  << solucao.vectorBits.count() << endl;
        #endif



        //No artigo eles relatam que só aplicam a busca local e path relinking
        //se a solução obtida pela construção é pelo menos 80% da melhor solução encontrada até agora.
        if(solucao.vectorBits.count() >= 0.8*melhor_solucao.vectorBits.count()) {

					busca_local(solucao);

					/*
					Aqui implemento a maneira que a solução encontrada na busca local será inserida na lista elite.
					Quando a quantidade máxima não é alcançada, adiciona na lista elite.
					Quando a quantidade máxima já é alcançada, trocamos a solução da lista elite mais parecida com a solução
					e que tenha custo pior que da solução.

					Nao coloquei isso dentro da propria função path_relinking, porque achei que ficaria mais complicado o retorno
					da função path_relinking.
					*/

					if(solucoes_elites.size() >= 1){
							solucao = path_relinking(solucao);
					}
					//Atualizo a lista elite.
					if(solucoes_elites.size() < max_elite) solucoes_elites.push_back(solucao);
					else{
							int menor_dif = graph.k + 12;
							int posicao_menor_dif = -1;
							for(int j = 0; j < solucoes_elites.size(); j++){
									if(solucao.vectorBits.count() > solucoes_elites[j].vectorBits.count()){
											int dif = diferenca_simetrica(solucao, solucoes_elites[j], graph.k);
											if(menor_dif > dif){
													menor_dif = dif;
													posicao_menor_dif = j;
											}
									}
							}
							if(posicao_menor_dif != -1){
									solucoes_elites[posicao_menor_dif] = solucao;
							}
                    }
        }

        if(i == 0) melhor_solucao = solucao;
        else if(solucao.vectorBits.count() > melhor_solucao.vectorBits.count()){
                melhor_solucao = solucao;
                if(melhor_solucao.vectorBits.count() == graph.tam_R) break;
        }

        cont[indice_alfa]++;
		score[indice_alfa] += solucao.vectorBits.count();

        #ifdef DEBUG
            printf("cont[%d] = %d, score[%d] = %lf\n", indice_alfa, cont[indice_alfa], indice_alfa, score[indice_alfa]);
        #endif


        #ifdef DEBUG
            cout << "Solucao size final : "  << solucao.vectorBits.count() << endl;
            cout << "--------------------------------------" << endl;
        #endif
    }
    return melhor_solucao;
}



Solucao Grasp::grasp_reativo(){
    #ifdef DEBUG
        cout << "-----------GRASP------" << endl;
    #endif

    solucoes_elites.clear();

    int custo_melhor_solucao = 0;

    Solucao melhor_solucao(graph.tam_L);

    melhor_solucao_construcao = -1;

    bool apicar_buscar_path = true;

    //Inicializando os valores iniciais do grasp reativo
    for(int i = 0; i < NUMBER_VALUES; i++){
        cont[i] = 0;
        score[i] = 0.0;
        prob_X[i] = 1.0/NUMBER_VALUES;
    }

    for(int i = 0; i < qtd_max_iteracoes; i++){
        #ifdef DEBUG
			cout << "Iteracao: " << i << endl;
        #endif

        int indice_alfa = get_alfa();
        double alfa = cons_X[indice_alfa];

        #ifdef DEBUG
            cout << "alpha = " << alfa << endl;
            cout << "FASE CONSTRUCAO " << endl;
        #endif

        Solucao solucao = construcao(alfa);

        #ifdef DEBUG
            cout << "Solucao size construcao : "  << solucao.vectorBits.count() << endl;
        #endif



        //No artigo eles relatam que só aplicam a busca local e path relinking
        //se a solução obtida pela construção é pelo menos 80% da melhor solução encontrada até agora.
        if(solucao.vectorBits.count() >= 0.8*melhor_solucao.vectorBits.count()) {

					busca_local(solucao);

					/*
					Aqui implemento a maneira que a solução encontrada na busca local será inserida na lista elite.
					Quando a quantidade máxima não é alcançada, adiciona na lista elite.
					Quando a quantidade máxima já é alcançada, trocamos a solução da lista elite mais parecida com a solução
					e que tenha custo pior que da solução.

					Nao coloquei isso dentro da propria função path_relinking, porque achei que ficaria mais complicado o retorno
					da função path_relinking.
					*/

					if(solucoes_elites.size() >= 1){
							solucao = path_relinking(solucao);
					}
					//Atualizo a lista elite.
					if(solucoes_elites.size() < max_elite) solucoes_elites.push_back(solucao);
					else{
							int menor_dif = graph.k + 12;
							int posicao_menor_dif = -1;
							for(int j = 0; j < solucoes_elites.size(); j++){
									if(solucao.vectorBits.count() > solucoes_elites[j].vectorBits.count()){
											int dif = diferenca_simetrica(solucao, solucoes_elites[j], graph.k);
											if(menor_dif > dif){
													menor_dif = dif;
													posicao_menor_dif = j;
											}
									}
							}
							if(posicao_menor_dif != -1){
                                solucoes_elites[posicao_menor_dif] = solucao;
							}
					}

        }

        if(i == 0) melhor_solucao = solucao;
        else if(solucao.vectorBits.count() > melhor_solucao.vectorBits.count()){
            melhor_solucao = solucao;
            if(melhor_solucao.vectorBits.count() == graph.tam_R) break;
        }

        cont[indice_alfa]++;
		score[indice_alfa] += solucao.vectorBits.count();

        #ifdef DEBUG
			printf("cont[%d] = %d, score[%d] = %lf\n", indice_alfa, cont[indice_alfa], indice_alfa, score[indice_alfa]);
		#endif


        if(i % NUMBER_VALUES == 0){
            atualizacao_probabilidade(melhor_solucao.vectorBits.count());
        }

        #ifdef DEBUG
            cout << "Solucao size final : "  << solucao.vectorBits.count() << endl;
            cout << "--------------------------------------" << endl;
        #endif
    }
    return melhor_solucao;
}

