//
// Created by 35193 on 30/11/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "algoritmo.h"
#include "funcao.h"


int calcula_fit(int *solucao,int **matriz, int v,int a,int k){
    int soma=0;
    int p = 0;
    int cost = -1;
    for(int i = 0; i<v; i++){
        p = 0;
        for(int j = 0; j<v;j++){
            if((matriz[i][j] > 0) && (solucao[j+1] == 1)){
                if((solucao[i+1] == 1) && (solucao[j+1] == 1) && (i+1!=j+1)){
                    soma += matriz[j][i];
                    p = 1;
                }
            }
        }
        if(solucao[i+1] != 0)
            if(p == 0){
                soma += 99999999;
            }
    }
    soma = soma/2;
    return soma;
}


void qualidadePop(int* pop[POP_SIZE], int qualidade[POP_SIZE], int** dist, int v, int a, int k) {
    int i;
    for (i = 0; i < POP_SIZE; i++)
    {
        qualidade[i] = calcula_fit(pop[i], dist,v,a,k);
    }
}
