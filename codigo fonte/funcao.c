//
// Created by 35193 on 06/12/2023.
//

#include "funcao.h"
#include <stdio.h>


/*int calcula_fit(int *solucao,int **matriz, int vert, int aresta, int k){
    int soma=0;
    int p;

    for(int i = 0; i<vert; i++){
        p = 0;
        for(int j = 0; j<vert;j++){
            if((matriz[i][j] > 0) && (solucao[j] == 1)){
                if((solucao[i] == 1) && (solucao[j] == 1) && (i!=j)){
                    soma += matriz[j][i];
                    p = 1;
                }
            }
        }
        if(solucao[i] != 0)
            if(p == 0){
                soma += 999;
            }
    }

    soma = soma/2;

    return soma;
}*/
int calcula_fit(int *solucao, int **matriz, int vert, int aresta, int k) {
    int soma = 0, p;

    for (int i = 0; i < vert; i++) {

        p = 0;
        if(solucao[i] != 0) {

            for (int j = 0; j < vert; j++) {

                if (matriz[i][j] > 0 && solucao[i] == 1 && solucao[j] == 1 && i != j) {
                    soma += matriz[i][j];
                    p = 1;
                }
            }
        }

        if (solucao[i] != 0 && p == 0) {
            soma += 99999;
        }
    }

    soma /= 2;  // Move the division by 2 outside the loop

    return soma;
}
