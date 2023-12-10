//
// Created by 35193 on 06/12/2023.
//

#ifndef IIAPT2_UTILS_H
#define IIAPT2_UTILS_H
#define DEFAULT_RUNS 10
#define PROB_RECUAR 0.05
#include "algoritmo.h"

void init_rand();
int **inicializar_Dados_Ficheiro(char *filename ,int *vert ,int *arestas, int *k, int *somaDoCustos);
void printMatriz(int** matriz, int lado);
int random_l_h(int min, int max);
void gera_sol_inicial(int *sol, int v, int k);
void escreve_sol(int *sol, int v);
float rand_01();
pchrom init_pop(struct info d);
int flip();
void evaluate(pchrom pop, struct info d, int **mat);
chrom get_best(pchrom pop, struct info d, chrom best);
void write_best(chrom x, struct info d);
void gera_sol_inicial2(int *sol, int v, int k, int **mat);

#endif //IIAPT2_UTILS_H
