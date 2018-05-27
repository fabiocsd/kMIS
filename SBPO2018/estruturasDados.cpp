
#define nElem 300

int k;
int tam_L = 0, tam_R = 0;

/**
Classe que representa uma solução do problema.
**/
class Solucao{
public:
    bitset<nElem> vectorBits;//Bitset da solução (interseção)
    vector<bool> is_elem;//Vetor de tamanho igual a quantidade de vertices, onde o índice j será true se o subconjunto j está na solução.
    vector<int> elem;//Vetor com os indices dos subconjuntos que pertecem a solução.

    Solucao(){
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
        for (int j = 0; j < tam_L; j++)//VARIAVEL GLOBAL: PERIGO!!!!!!!!!!!!!!!!!
            is_elem.push_back(false);
    }

    void addSubConjunto(int subConj){
        elem.push_back(subConj);
        is_elem[subConj] = true;
    }
};

class Elemento{
public:
    int id;
    int n;
    bitset<nElem> vectorBits;

    Elemento(int id){
        this->id = id;
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

vector<Elemento> conjuntosPrincipal, conjuntosPrincipalOrdenado;
vector<Elemento> conjuntoR;

bitset<nElem> Interseccao(bitset<nElem> bs1, bitset<nElem> bs2){
    bitset<nElem> result = bs1 & bs2;
    return result;
}

int Ler_arquivo(string caminho, int tipo){
    int qtd_instancias, conjunto, valor;
    FILE *arq;
    arq = fopen( caminho.c_str(),"r" );

    if(tipo == 1){
        fscanf(arq, "%d %d %d", &tam_L, &qtd_instancias, &k);
        tam_R = tam_L;
    }
    else fscanf(arq, "%d %d %d %d", &tam_L, &tam_R, &qtd_instancias, &k);

    for (int i = 0; i < tam_L; i++){
        conjuntosPrincipal.push_back(Elemento(i));
    }

    /*for (int i = 0; i < tam_R; i++){
        conjuntoR.push_back(Elemento(i));
    }*/

    /**
    Código para o Pre-processamento que elimina elementos que estão em quantidades de subconjuntos menor do que k.
    **/
    bool** A = (bool**)malloc(tam_L*sizeof(bool*));
    for(int i = 0; i < tam_L; i++) A[i] = (bool*)malloc(tam_R*sizeof(bool));

    for(int i = 0; i < tam_L; i++)
        for(int j = 0; j < tam_R; j++)
            A[i][j] = 0;

    for(int j = 0; j < qtd_instancias; j++){
        fscanf(arq, "%d %d", &conjunto, &valor);
        A[conjunto - 1][valor - 1] = 1;
    }

    int tem_solucao = 0;
    int inElemento[tam_R];//Vetor que indica se um elemento estar em pelo menos k subconjuntos.
    for(int i = 0; i < tam_R; i++){
        int vizinhanca = 0;
        for(int j = 0; j < tam_L; j++){
            if(A[j][i]) vizinhanca++;
        }
        if(vizinhanca < k){
            inElemento[i] = 0;
        }else{
            inElemento[i] = 1;
            tem_solucao++;
        }
    }

    for(int j = 0; j < tam_L; j++){
        Elemento e = conjuntosPrincipal[j];
        for(int i = 0; i < tam_R; i++){
            if(inElemento[i] && A[j][i]){
                e.vectorBits.set(i);
            }
        }
        conjuntosPrincipal[j] = e;
    }

    /*for(int i = 0; i < tam_R; i++){
        if(inElemento[i]){
            Elemento e = conjuntoR[i];
            for(int j = 0; j < tam_L; j++){
                if(A[j][i]){
                    e.vectorBits.set(j);
                }
            }
            conjuntoR[i] = e;
        }
    }*/

    fclose(arq);
    conjuntosPrincipalOrdenado = conjuntosPrincipal;
    std::sort(conjuntosPrincipalOrdenado.begin(), conjuntosPrincipalOrdenado.end(), Elemento::LessThanByAge());
    return tem_solucao;
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

