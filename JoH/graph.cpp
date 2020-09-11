#include "graph.h"

/**
Dado um vetor representando um subconjunto de L e um inteiro p,
ele devolve o indice de L do elemento na posição p nesse vetor.
**/
int get_subconjunto(vector<bool> vetor, int p){
    int q = 0;
    for (int j = 0; j < vetor.size(); j++){
        if(vetor[j] == true){
            if(q == p) return j;

            q++;
        }
    }
    return -1;
}


/**
Diferença simetrica de sol1 para sol2, ou seja, quantos movimentos(trocas) são necessários para transformar sol1 na sol2.
Artigo An Integer Programming Formulation for the Maximum k-Subset Intersection Problem, 2014
**/
int diferenca_simetrica(const Solucao &sol1, const Solucao &sol2, int k){
    int iguais = 0;
    for(int j = 0; j < sol1.elem.size(); j++){
        int q = sol1.elem[j];
        if(sol2.is_elem[q] == true) iguais++;
    }
    return (k - iguais);
}

Graph::~Graph(){
    conjuntosPrincipal.clear();
    conjuntosPrincipalOrdenado.clear();
}

/**
filename: caminho da instancia
tipo: 1 apenas instancias do tipo 1 com |L| = |R| e na primeira linha temos |L|, qtd arestas e k.
tipo: 2 instancias dos três tipos e na primeira linha temos |L|, |R|, qtd arestas e k.
tipo: 3 instancias dos três tipos e na primeira linha temos |L|, |R|, qtd arestas. O valor de k deve ser setado fora do metodo e antes de chamar o metodo.
**/
bool Graph::read_file(const char *filename, int tipo){
    ifstream input_fh(filename);
    int tem_solucao = 0;
    if (input_fh.is_open()){
        int qtd_instancias, conjunto, valor;

        if(tipo == 1){
            input_fh >> this->tam_L;
            input_fh >> qtd_instancias;
            input_fh >> this->k;
            this->tam_R = this->tam_L;
        }
        else if(tipo == 1){
            input_fh >> this->tam_L;
            input_fh >> this->tam_R;
            input_fh >> qtd_instancias;
            input_fh >> this->k;
        }else{
            input_fh >> this->tam_L;
            input_fh >> this->tam_R;
            input_fh >> qtd_instancias;
        }

        for (int i = 0; i < this->tam_L; i++){
            this->conjuntosPrincipal.push_back(Elemento(i));
        }

        /**
        Código para o Pre-processamento que elimina elementos que estão em quantidades de subconjuntos menor do que k.
        Não entendi o código do Robberty que faz esse pre-processamento na heurística 3. Fiz do meu jeito.
        **/
        bool** A = (bool**)malloc(this->tam_L*sizeof(bool*));
        for(int i = 0; i < this->tam_L; i++) A[i] = (bool*)malloc(this->tam_R*sizeof(bool));

        for(int i = 0; i < this->tam_L; i++)
            for(int j = 0; j < this->tam_R; j++)
                A[i][j] = 0;

        for(int j = 0; j < qtd_instancias; j++){
            input_fh >> conjunto;
            input_fh >> valor;
            A[conjunto - 1][valor - 1] = 1;
        }
        int inElemento[this->tam_R];//Vetor que indica se um elemento estar em pelo menos k subconjuntos.
        for(int i = 0; i < this->tam_R; i++){
            int vizinhanca = 0;
            for(int j = 0; j < this->tam_L; j++){
                if(A[j][i]) vizinhanca++;
            }
            if(vizinhanca < this->k){
                inElemento[i] = 0;
            }else{
                inElemento[i] = 1;
                tem_solucao++;
            }
        }

        for(int j = 0; j < this->tam_L; j++){
            Elemento e = this->conjuntosPrincipal[j];
            for(int i = 0; i < this->tam_R; i++){
                if(inElemento[i] && A[j][i]){
                    e.vectorBits.set(i);
                }
            }
            this->conjuntosPrincipal[j] = e;
        }

        this->conjuntosPrincipalOrdenado = this->conjuntosPrincipal;
        std::sort(this->conjuntosPrincipalOrdenado.begin(), this->conjuntosPrincipalOrdenado.end(), Elemento::LessThanByAge());

        input_fh.close();
    }
    else{
        cerr << "ERROR: could not open file (might not exist)." << endl;
        return false;
    }

    return (tem_solucao == 0 ? false : true);
}

/**
 * @brief Realiza a interseção entre dois bitset.
 * @param bs1
 * @param bs2
 * @return
 */
bitset<nElem> Interseccao(bitset<nElem> bs1, bitset<nElem> bs2){
    bitset<nElem> result = bs1 & bs2;
    return result;
}

int randValor(int linf, int lsup){
    #ifdef __linux__
			return(linf + lrand48() % (lsup - linf + 1));//Aqui quando for executar no linux.
    #elif _WIN32
			return (linf + rand() % (lsup - linf + 1));//Aqui no windows
    #else
			#error "OS not supported!"
    #endif
}

double randValorDouble(int linf, int lsup){
	#ifdef __linux__
			int inteiro = (linf + lrand48() % (lsup - linf));//Aqui quando for executar no linux.
			return (inteiro + drand48());
    #elif _WIN32
			int inteiro = (linf + rand() % (lsup - linf));//Aqui no windows
			return (inteiro + rand());
	#else
			#error "OS not supported!"
		#endif
}


