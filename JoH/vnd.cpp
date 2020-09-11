#include "vnd.h"

/**
Verificar se um vetor de inteiros contem um determinado valor p.
*/
int contem(const vector<int> &vetor, int p){
    for (int j = 0; j < vetor.size(); j++){
        if(vetor[j] == p)
            return 1;
    }
    return 0;
}


void vizinhanca_t(Graph &graph, Solucao &solucao, int t, vector<int> &sair){
	int tam_L = graph.tam_L;

	int sol_atual = solucao.vectorBits.count();

	for (int i = 0; i < t; i++) solucao.is_elem[solucao.elem[sair[i]]] = false;

    bitset<nElem> bit_solucao_parcial;
    bit_solucao_parcial.set();
    /*Aqui eu gero a interseção sem os subconjuntos em sair que irão ser removidos na troca.*/
    for (int q = 0; q < solucao.elem.size(); q++){
        if(!contem(sair, q)){
            bit_solucao_parcial = Interseccao(bit_solucao_parcial, graph.conjuntosPrincipal[solucao.elem[q]].vectorBits);
        }
    }

    vector<int> entrar;
    for (int i = 0; i < t; i++){
        int val_indice = -1, indice = -1;
        for (int j1 = 0; j1 < tam_L; j1++){
            int indice_sub = j1;

            if(solucao.is_elem[indice_sub] == false && !contem(entrar, indice_sub)){
                bitset<nElem> bit_aux1 = Interseccao(bit_solucao_parcial, graph.conjuntosPrincipal[indice_sub].vectorBits);

                int valor_int = bit_aux1.count();
                if(valor_int > val_indice){
                    val_indice = valor_int;
                    indice = indice_sub;
                }
            }
        }
		if(val_indice <= sol_atual){
			bit_solucao_parcial.reset();
			break;
		}
        bit_solucao_parcial = Interseccao(bit_solucao_parcial, graph.conjuntosPrincipal[indice].vectorBits);
        entrar.push_back(indice);
    }
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

Solucao VND(Graph &graph, Solucao &solucao_entrada){
    Solucao solucao_linha = solucao_entrada;
    Solucao melhor_solucao = solucao_entrada;

    int t = 1;
	int profundidade_busca_local = 0.3*graph.k;
	if(profundidade_busca_local < 3) profundidade_busca_local = 3;

	if(graph.k == 2) profundidade_busca_local = 1;
	else if(graph.k == 3) profundidade_busca_local = 2;

	vector<bitset<nElem>> bits_sair;

	int diferentes = profundidade_busca_local;//profundidade_busca_local/3; 1;
	if(diferentes == 0) diferentes = 1;

	int aceitacao = profundidade_busca_local - diferentes;//No máximo essa quantidade de subconjuntos iguais

    while(t <= 3){
		bitset<nElem> bit_sair;
		bit_sair.reset();
		vector<int> sair;//Não contem os indices dos subconjuntos, mas suas posiçoes no vetor elem da solução

        int indices_solucao[graph.k], size_indices = graph.k;
        for(int i = 0; i < graph.k; i++) indices_solucao[i] = i;

		int j = 1;
		while(j <= profundidade_busca_local){
            int index_s = randValor(1, size_indices) - 1;//Seleciona aleatoriamente um indice desconsiderando os que já foram selecionados. Não é o indice real dentro de solucao.elem.
            int s = indices_solucao[index_s];

            indices_solucao[index_s] = indices_solucao[size_indices - 1];
            size_indices--;

			bit_sair.set((size_t)solucao_linha.elem[s]);

			sair.push_back(s);
			j++;
		}
		for(int i = 0; i < bits_sair.size(); i++){
			bitset<nElem> bit_sol = Interseccao(bit_sair, bits_sair[i]);
			if(bit_sol.count() > aceitacao) continue;
		}

		bits_sair.push_back(bit_sair);
		vizinhanca_t(graph, solucao_linha, profundidade_busca_local, sair);

        if(melhor_solucao.vectorBits.count() < solucao_linha.vectorBits.count()){
            melhor_solucao = solucao_linha;
        }
		t++;
    }

    return melhor_solucao;
}

