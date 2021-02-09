#include <iostream>
#include <bitset>
#include <vector>
#include <list>
#include <algorithm>
#include <time.h>
#include <stdio.h>
#include <string>

using namespace std;

#include "graph.h"
#include "grasp.h"
#include "heuristics.h"
#include "vns.h"

string path_dir = "..//Instances//Random//Journal_of_heuristics//";

/**
Run vns countTime times storing the value and time of each run.
**/
void execute_vns_repeat_armazenar(string file, Graph &graph, string nameFileOut, string nameFileOut2, int countTime){
    double tempo;
    clock_t t1, t2;
    FILE *txt;
    int valor_solucao = 0;
    double media_solucao = 0, media_tempo = 0;
    int melhor_solucao = -1, pior_solucao = graph.tam_R;

	Solucao solucaoH = heuristica_kinter_estendida_path_relinking(graph);
	int sol_entrada = solucaoH.vectorBits.count();
	vector<int> solucoes;
	vector<double> tempos;
    for(int i = 1; i <= countTime; i++){
        t1 = clock();
        Solucao solucao = solucaoH;
        int sol_heuristica = solucao.vectorBits.count();

        if(sol_heuristica < graph.tam_R){
			solucao = VNS_Reativo(graph, solucao);
		}
        valor_solucao = solucao.vectorBits.count();

        t2 = clock();
        tempo = (t2 - t1)/ (double) CLOCKS_PER_SEC;

		solucoes.push_back(valor_solucao);
		tempos.push_back(tempo);

        media_tempo += tempo;
        media_solucao += valor_solucao;

        if(melhor_solucao < valor_solucao) melhor_solucao = valor_solucao;
        if(pior_solucao > valor_solucao) pior_solucao = valor_solucao;
    }
    media_tempo /= countTime;
    media_solucao /= countTime;

    txt = fopen(nameFileOut.c_str(), "a+");
    fprintf(txt, "%s\t", file.c_str());
    fprintf(txt, "%d\t", graph.k);
	fprintf(txt, "%d\t", sol_entrada);
    fprintf(txt, "%f\t%.2f\t", media_tempo, media_solucao);
    fprintf(txt, "%d\t%d\n", pior_solucao, melhor_solucao);
	fclose(txt);

	txt = fopen(nameFileOut2.c_str(), "a+");
	for(int i = 0; i < countTime; i++){
		fprintf(txt, "%s\t", file.c_str());
		fprintf(txt, "%d\t%f\n", solucoes[i], tempos[i]);
	}
	fclose(txt);
}

/**
Run the vns 10 times generating the average of the solutions found, as well as the best, worst solution and average time.
**/
void execute_vns_repeat(string file, Graph &graph, string nameFileOut, int countTime){
    double tempo;
    clock_t t1, t2;
    FILE *txt;
    int valor_solucao = 0;
    double media_solucao = 0, media_tempo = 0;
    int melhor_solucao = -1, pior_solucao = graph.tam_R;

	Solucao solucaoH = heuristica_kinter_estendida_path_relinking(graph);
	int sol_entrada = solucaoH.vectorBits.count();
    for(int i = 1; i <= countTime; i++){
        t1 = clock();
        Solucao solucao = solucaoH;
        int sol_heuristica = solucao.vectorBits.count();

        if(sol_heuristica < graph.tam_R){
			solucao = VNS_Reativo(graph, solucao);
		}
        valor_solucao = solucao.vectorBits.count();
        t2 = clock();
        tempo = (t2 - t1)/ (double) CLOCKS_PER_SEC;

        media_tempo += tempo;
        media_solucao += valor_solucao;

        if(melhor_solucao < valor_solucao) melhor_solucao = valor_solucao;
        if(pior_solucao > valor_solucao) pior_solucao = valor_solucao;
    }
    media_tempo /= countTime;
    media_solucao /= countTime;

    txt = fopen(nameFileOut.c_str(), "a+");
    fprintf(txt, "%s\t", file.c_str());
    fprintf(txt, "%d\t", graph.k);
	fprintf(txt, "%d\t", sol_entrada);
    fprintf(txt, "%f\t%.2f\t", media_tempo, media_solucao);
    fprintf(txt, "%d\t%d\n", pior_solucao, melhor_solucao);
    fclose(txt);
}


void execute_grasp(string file, Graph &graph, int countTime){
    double tempo;
    clock_t t1, t2;
    FILE *txt;
    int valor_solucao = 0;
    double media_solucao = 0, media_tempo = 0;
    int melhor_solucao = -1, pior_solucao = graph.tam_R;

    media_solucao = 0;
    media_tempo = 0;
    melhor_solucao = -1;
    pior_solucao = graph.tam_R;

    Grasp grasp(graph);
    for(int i = 1; i <= countTime; i++){
        t1 = clock();
        Solucao sol = grasp.grasp_reativo();
        valor_solucao = sol.vectorBits.count();
        t2 = clock();
        tempo = (t2 - t1)/ (double) CLOCKS_PER_SEC;

        media_tempo += tempo;
        media_solucao += valor_solucao;

        if(melhor_solucao < valor_solucao) melhor_solucao = valor_solucao;
        if(pior_solucao > valor_solucao) pior_solucao = valor_solucao;
    }
    media_tempo /= countTime;
    media_solucao /= countTime;

    txt = fopen("ResultadosGrasp.txt", "a+");
    fprintf(txt, "%s\t", file.c_str());
    fprintf(txt, "%d\t", graph.k);
    fprintf(txt, "%f\t%.2f\t", media_tempo, media_solucao);
    fprintf(txt, "%d\t%d\n", pior_solucao, melhor_solucao);
    fclose(txt);

}

/**
Run vns countTime times storing the value and time of each run.
**/
void execute_grasp_armazenar(string file, Graph &graph, string nameFileOut, string nameFileOut2, int countTime){
    double tempo;
    clock_t t1, t2;
    FILE *txt;
    int valor_solucao = 0;
    double media_solucao = 0, media_tempo = 0;
    int melhor_solucao = -1, pior_solucao = graph.tam_R;

    media_solucao = 0;
    media_tempo = 0;
    melhor_solucao = -1;
    pior_solucao = graph.tam_R;

	Grasp grasp(graph);
	vector<int> solucoes;
	vector<double> tempos;
    for(int i = 1; i <= countTime; i++){
        t1 = clock();
        Solucao sol = grasp.grasp_reativo();
        valor_solucao = sol.vectorBits.count();
        t2 = clock();
        tempo = (t2 - t1)/ (double) CLOCKS_PER_SEC;

		solucoes.push_back(valor_solucao);
		tempos.push_back(tempo);

        media_tempo += tempo;
        media_solucao += valor_solucao;

        if(melhor_solucao < valor_solucao) melhor_solucao = valor_solucao;
        if(pior_solucao > valor_solucao) pior_solucao = valor_solucao;
    }
    media_tempo /= countTime;
    media_solucao /= countTime;

	txt = fopen(nameFileOut.c_str(), "a+");
    fprintf(txt, "%s\t", file.c_str());
    fprintf(txt, "%d\t", graph.k);
    fprintf(txt, "%f\t%.2f\t", media_tempo, media_solucao);
    fprintf(txt, "%d\t%d\n", pior_solucao, melhor_solucao);
    fclose(txt);

	txt = fopen(nameFileOut2.c_str(), "a+");
	for(int i = 0; i < countTime; i++){
		fprintf(txt, "%s\t", file.c_str());
		fprintf(txt, "%d\t%f\n", solucoes[i], tempos[i]);
	}
	fclose(txt);
}


int execute_all(int type, string nameDir){
    int allN[] = {40, 60, 80, 100, 140, 180, 200, 240, 280, 300};
    for (int j = 1; j <= 9; j++) {//Class
        for (int l = 0; l <= 9; l++) {//Size of instances
            int nL, nR;
            string caminho;
            if(type == 1){
                nL = allN[l];
                nR = allN[l];
                caminho = path_dir + nameDir + "//type1//classe_" + std::to_string(j) + "_" + std::to_string(nL) + "_" + std::to_string(nR) + ".txt";
            }else if(type == 2){
                nL = allN[l];
                nR = allN[l]*0.8;
                caminho = path_dir + nameDir + "//type2//classe_" + std::to_string(j) + "_" + std::to_string(nL) + "_" + std::to_string(nR) + ".txt";
            }else{
                nL = allN[l]*0.8;
                nR = allN[l];
                caminho = path_dir + nameDir + "//type3//classe_" + std::to_string(j) + "_" + std::to_string(nL) + "_" + std::to_string(nR) + ".txt";
            }
            Graph graph;
            int tem_solucao = graph.read_file(caminho.c_str());
            if(tem_solucao){
                string saida = "_G1.txt";
                if(type == 2) saida = "_G2.txt";
                else if(type == 3) saida = "_G3.txt";

                string classe = "classe_" + std::to_string(j) + "_" + std::to_string(nL) + "_" + std::to_string(nR);

                if(KMIS_E_CARD >= 1 && KMIS_E_CARD <= 3)
                    execute_vns_repeat_armazenar(classe, graph, "ResultadosVNS_Reativo" + saida, "ResultadosDetalhadosVNS_Reativo" + saida, 10);
                else execute_grasp_armazenar(classe, graph, "ResultadosGrasp" + saida, "ResultadosDetalhadosGrasp" + saida, 10);
            }
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

    string pathDir = "Gilbert//";

    if(KMIS_E_CARD == 1 || KMIS_E_CARD == 4)
		execute_all(1, pathDir);
    else if(KMIS_E_CARD == 2 || KMIS_E_CARD == 5)
		execute_all(2, pathDir);
    else execute_all(3, pathDir);

    return 0;
}
