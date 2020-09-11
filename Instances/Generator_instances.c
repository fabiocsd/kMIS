#include <string.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAXINT 2147483647

void liberarMatrizInt(int **matriz, int tamanhoColuna){
    int i;
    for (i = 0; i < tamanhoColuna; i++){
        free(matriz[i]);
    }
    free(matriz);
}


int** criarMatrizInt(int tamanhoLinha, int tamanhoColuna, int valorInicial){
    int** matriz = (int**) malloc(tamanhoLinha*sizeof(int*));
    if (matriz == NULL) {
      printf ("SEM MEMORIA!\n");
      exit(1);
    }

    int i, j;
	for (i = 0; i < tamanhoLinha; i++){
		matriz[i] = (int*)malloc(tamanhoColuna*sizeof(int));
        if (matriz[i] == NULL) {
          printf ("SEM MEMORIA!\n");
          exit(1);
        }
		for (j = 0; j < tamanhoColuna; j++){
			matriz[i][j] = valorInicial;
		}
	}
	return matriz;
}

int randValor(int linf, int lsup){

    #ifdef __linux__
			return(linf + lrand48() % (lsup - linf + 1));
    #elif _WIN32
			return (linf + rand() % (lsup - linf + 1));
    #else
			#error "OS not supported!"
	#endif
}

double randValorDouble(int linf, int lsup){
    #ifdef __linux__
			int inteiro = (linf + lrand48() % (lsup - linf));
			return (inteiro + drand48());
	#elif _WIN32
			int inteiro = (linf + rand() % (lsup - linf));
			return (inteiro + rand());
    #else
			#error "OS not supported!"
	#endif
}


/**
Gera um grafo bipartite balanceado, ou seja, com a mesma quantidade de vértices em cada partição.
São criadas de acordo com Erdos and Renyi, 1960.
Serão instâncias para o problema da máxima interseção de k-subconjuntos.

nL: quantidade de subconjuntos;
nR: quantidade de elementos;
p: probabilidade das arestas serem escolhidas, com 0 <= p <= 1;
td: o tipo de densidade, sendo 1 para baixa, 2 para média e 3 para alta;
tk: o tipo de valor para k, sendo 1 para baixa, 2 para média e 3 para alta.
*/
int** gerar(int nL, int nR, double p, int td, int tk, int* qtdArestasE, int* kE){
    #ifdef __linux__
            srand48(time(NULL));
    #elif _WIN32
			srand(time(NULL));
    #else
			#error "OS not supported!"
    #endif
    /*
    A primeira linha representa os subconjuntos e a segunda os elementos.
    Será igual a 1 se o elemento pertencer ao sunconjunto e zero caso contrário.
    */
    int **M = criarMatrizInt(nL, nR, 0);

    int ubK = 0, lbK = 0;
    double ubD = 0, lbD = 0;

    if(tk == 1){
        lbK = floor(0.1*nL);
        ubK = ceil(0.3*nL);
    }else if(tk == 2){
        lbK = floor(0.4*nL);
        ubK = ceil(0.6*nL);
    }else{
        lbK = floor(0.7*nL);
        ubK = ceil(0.9*nL);
    }

    int k = randValor(lbK, ubK);

    int i, j, qtdArestas = 0;
    for(i = 0; i < nL; i++){//For each subset i
        for(j = 0; j < nR; j++){//For each element j
            double pp = 0.0;
            #ifdef __linux__
                    pp = drand48();
            #elif _WIN32
                    pp = rand();
            #else
                    #error "OS not supported!"
            #endif
            if(pp <= p){
                M[i][j] = 1;
                qtdArestas++;
            }
        }
    }


	for(j = 0; j < nR; j++){//For each element j
		for(i = 0; i < nL; i++){//For each subset i
			if(M[i][j] == 0){
				double pp = 0.0;
				#ifdef __linux__
						pp = drand48();
				#elif _WIN32
						pp = rand();
				#else
						#error "OS not supported!"
				#endif

				if(pp <= p){
					M[i][j] = 1;
					qtdArestas++;
				}
			}
		}
	}
    
    *qtdArestasE = qtdArestas;
    *kE = k;
    return M;
}

int tem_solucao(int** A, int tam_L, int tam_R, int k){
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

    return tem_solucao;
}


void gerarInstancias(int type){
    int td, tk, q, i, j;
    int n[] = {40, 60, 80, 100, 140, 180, 200, 240, 280, 300, 400, 500, 600, 800};
    double p[] = {0.3, 0.6, 0.9};

    for(tk = 1; tk <= 3; tk++){
        for(td = 1; td <= 9; td++){
            for(q = 0; q <= 9; q++){
                int classe = tk;

                if(td == 2) classe += 3;
                else if(td == 3) classe += 6;

                int qtdArestas = 0, k = 0;
                int nL = n[q], nR = n[q];

				if(type == 2) nR = 0.8*nR;
				else if(type == 3) nL = 0.8*nL;

                int** M;
                int qtd = 0, tem_sol = 1;
                do{
                        M = gerar(nL, nR, p[td - 1], td, tk, &qtdArestas, &k);
                        qtd++;
                        tem_sol = tem_solucao(M, nL, nR, k);
                        if(tem_sol == 0 && qtd < 100) liberarMatrizInt(M, nL);
                }while(tem_sol == 0 && qtd < 100);

                char nome[101];
				if(type == 1) sprintf(nome,"type1//classe_%d_%d_%d.txt", classe, nL, nR);
				else if(type == 2) sprintf(nome,"type2//classe_%d_%d_%d.txt", classe, nL, nR);
				else if(type == 3) sprintf(nome,"type3//classe_%d_%d_%d.txt", classe, nL, nR);

                FILE *arquivo = fopen(nome, "w");
                fprintf(arquivo,"%d %d %d %d\n", nL, nR, qtdArestas, k);
                for (i = 0; i < nL; i++)
                    for (j = 0; j < nR; j++)
                        if (M[i][j] == 1) fprintf(arquivo,"%d %d\n",i+1,j+1);
                fclose(arquivo);

                liberarMatrizInt(M, nL);
            }
        }
    }
	printf("Instances type %d was created \n", type);
}



int main(){
    #ifdef __linux__
            srand48(time(NULL));
    #elif _WIN32
            srand(time(NULL));
    #else
            #error "OS not supported!"
    #endif
    gerarInstancias(1);
	gerarInstancias(2);
	gerarInstancias(3);
}
