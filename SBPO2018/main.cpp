#include <iostream>
#include <bitset>
#include <vector>
#include <list>
#include <algorithm>
#include <time.h>
#include <stdio.h>
#include <string>

using namespace std;

#include "estruturasDados.cpp"
#include "grasp.cpp"
#include "heuristicas.cpp"
#include "vns.cpp"

void execute_heuristics_greedy(int j, int nL, int nR){
    double tempo;
    clock_t t1, t2;
    FILE *txt;
    int valor_solucao = 0;
    double media_solucao = 0, media_tempo = 0;
    int melhor_solucao = -1, pior_solucao = tam_R;

    t1 = clock();
    Solucao solucaoH1 = heuristica1();
    valor_solucao = solucaoH1.vectorBits.count();
    t2 = clock();
    tempo = (t2 - t1)/ (double) CLOCKS_PER_SEC;
    txt = fopen("Resultados1.txt", "a+");
    fprintf(txt, "classe_%d_%d_%d ", j, nL, nR);
    fprintf(txt, "%d ", k);
    fprintf(txt, "%f ", tempo);
    fprintf(txt, "%d\n", valor_solucao);
    fclose(txt);

    t1 = clock();
    Solucao solucaoH2 = heuristica2_path_relinking2();
    valor_solucao = solucaoH2.vectorBits.count();
    t2 = clock();
    tempo = (t2 - t1)/ (double) CLOCKS_PER_SEC;
    txt = fopen("ResultadosH2.txt", "a+");
    fprintf(txt, "classe_%d_%d_%d ", j, nL, nR);
    fprintf(txt, "%d ", k);
    fprintf(txt, "%f ", tempo);
    fprintf(txt, "%d\n", valor_solucao);
    fclose(txt);
}

void execute_VNS(int j, int nL, int nR){
    double tempo;
    clock_t t1, t2;
    FILE *txt;
    int valor_solucao = 0;
    double media_solucao = 0, media_tempo = 0;
    int melhor_solucao = -1, pior_solucao = tam_R;

	Solucao solucaoH2 = heuristica2_path_relinking2();
    for(int i = 1; i <= 10; i++){
        t1 = clock();
        Solucao solucao = solucaoH2;
        int sol_heuristica = solucao.vectorBits.count();
        if(sol_heuristica < tam_R) solucao = VNS_SO_Grasp_Aceitacao(solucao);
        valor_solucao = solucao.vectorBits.count();
        t2 = clock();
        tempo = (t2 - t1)/ (double) CLOCKS_PER_SEC;

        media_tempo += tempo;
        media_solucao += valor_solucao;

        if(melhor_solucao < valor_solucao) melhor_solucao = valor_solucao;
        if(pior_solucao > valor_solucao) pior_solucao = valor_solucao;
    }
    media_tempo /= 10.0;
    media_solucao /= 10.0;

    txt = fopen("ResultadosVNSV.txt", "a+");
    fprintf(txt, "classe_%d_%d_%d ", j, nL, nR);
    fprintf(txt, "%d ", k);
    fprintf(txt, "%f %.2f ", media_tempo, media_solucao);
    fprintf(txt, "%d %d\n", pior_solucao, melhor_solucao);
    fclose(txt);
}

void execute_grasp(int j, int nL, int nR){
    double tempo;
    clock_t t1, t2;
    FILE *txt;
    int valor_solucao = 0;
    double media_solucao = 0, media_tempo = 0;
    int melhor_solucao = -1, pior_solucao = tam_R;

    media_solucao = 0;
    media_tempo = 0;
    melhor_solucao = -1;
    pior_solucao = tam_R;

    for(int i = 1; i <= 10; i++){
        t1 = clock();
        Solucao sol = grasp_reativo();
        valor_solucao = sol.vectorBits.count();
        t2 = clock();
        tempo = (t2 - t1)/ (double) CLOCKS_PER_SEC;

        media_tempo += tempo;
        media_solucao += valor_solucao;

        if(melhor_solucao < valor_solucao) melhor_solucao = valor_solucao;
        if(pior_solucao > valor_solucao) pior_solucao = valor_solucao;
    }
    media_tempo /= 10.0;
    media_solucao /= 10.0;

    txt = fopen("ResultadosGrasp.txt", "a+");
    fprintf(txt, "classe_%d_%d_%d ", j, nL, nR);
    fprintf(txt, "%d ", k);
    fprintf(txt, "%f %f ", media_tempo, media_solucao);
    fprintf(txt, "%d %d\n", pior_solucao, melhor_solucao);
    fclose(txt);

}

string path_dir = "..//..//Instances//";
int execute_all(int type){
    int allN[] = {40, 60, 80, 100, 140, 180, 200, 240, 280, 300, 400, 500, 600, 800};
    for (int j = 1; j <= 9; j++) {
        for (int l = 0; l <= 9; l++) {
            int nL, nR;
            string caminho;
            if(type == 1){
                nL = allN[l];
                nR = allN[l];
                caminho = path_dir + "type1//classe_" + std::to_string(j) + "_" + std::to_string(nL) + "_" + std::to_string(nR) + ".txt";
            }else if(type == 2){
                nL = allN[l];
                nR = allN[l]*0.8;
                caminho = path_dir + "type1//classe_" + std::to_string(j) + "_" + std::to_string(nL) + "_" + std::to_string(nR) + ".txt";
            }else{
                nL = allN[l]*0.8;
                nR = allN[l];
                caminho = path_dir + "type1//classe_" + std::to_string(j) + "_" + std::to_string(nL) + "_" + std::to_string(nR) + ".txt";
            }

            int tem_solucao = Ler_arquivo(caminho, 2);
            if(tem_solucao){
                    execute_heuristics_greedy(j, nL, nR);
					execute_VNS(j, nL, nR);
					execute_grasp(j, nL, nR);
            }
            conjuntosPrincipal.clear();
            conjuntosPrincipalOrdenado.clear();
            //conjuntosPrincipal.erase(conjuntosPrincipal.begin(),conjuntosPrincipal.end());
            //conjuntoR.erase(conjuntoR.begin(),conjuntoR.end());
        }
    }
    return 0;
}

int main(){
    #ifdef __linux__
            srand48(time(NULL));
    #elif _WIN32
            srand(time(NULL));
    #else
            #error "OS not supported!"
    #endif
    execute_all(1);
    execute_all(2);
    execute_all(3);

    return 0;
}
